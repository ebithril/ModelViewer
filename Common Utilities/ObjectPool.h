#pragma once

#include "BaseObjectPool.h"
#include "PoolPointer.h"
#include <new>
#include "ObjectPoolData.h"
#include "Heap.h"
#include "MemoryChunkLesser.h"
#include "SharedPointer.h"
#include <mutex>

#define OBJECT_POOL_TEMPLATE template<unsigned int SizeInBytes>
#define OBJECT_POOL_OBJECT ObjectPool<SizeInBytes>

//#define TYPE_TEMPLATE template<typename Type, typename ReturnType>
#define VARIADIC_TYPE_TEMPLATE template<typename Type, typename ReturnType, typename... Arguments>


namespace CommonUtilities
{
	OBJECT_POOL_TEMPLATE
	class ObjectPool : public BaseObjectPool
	{
	public:
		ObjectPool();
		~ObjectPool();

		void Init();

		VARIADIC_TYPE_TEMPLATE
			PoolPointer<ReturnType> Allocate(Arguments... someArguments);

		const unsigned int FreeSizeInBytes()const; //Expensive;
	protected:

		void MergeAdjacentFreeMemory(const MemoryChunk& aMemoryChunk, CU::GrowingArray<MemoryChunk, int>& someChunks);

		void DeAllocate(const ObjectPoolData& someData)override;
		unsigned int GetIndexWithEnoughMemoryAndReserve(const unsigned int aSizeInBytes);
		virtual	void* GetData(const unsigned int aMemoryID) override;

		char* myAllocatedMemory;

		int myMergeDeletedChunksSize;
		CU::GrowingArray<MemoryChunk, int> myChunks;

		SharedPointer<bool> myIsActive;

		std::mutex myLock;

	};

	OBJECT_POOL_TEMPLATE
		OBJECT_POOL_OBJECT::ObjectPool()
		: myAllocatedMemory(nullptr)
	{
		
	}

	OBJECT_POOL_TEMPLATE
		void OBJECT_POOL_OBJECT::Init()
	{
		myAllocatedMemory = new char[SizeInBytes];
		myChunks.Init(512);
		myChunks.Add(MemoryChunk(SizeInBytes, 0));

		myIsActive = new bool;
		*myIsActive = true;
		myMergeDeletedChunksSize = 10;
	}

	OBJECT_POOL_TEMPLATE
		const unsigned int OBJECT_POOL_OBJECT::FreeSizeInBytes()const
	{
		unsigned int freeSize = 0;

		for (int i = 0; i < myChunks.Size() ; i++)
		{
		 	freeSize += myChunks[i].mySize;
		}

		return freeSize;
	}

	OBJECT_POOL_TEMPLATE
		OBJECT_POOL_OBJECT::~ObjectPool()
	{
		SAFE_DELETE_ARRAY(myAllocatedMemory);
		*myIsActive = false;
	}

	OBJECT_POOL_TEMPLATE
	VARIADIC_TYPE_TEMPLATE
		PoolPointer<ReturnType> OBJECT_POOL_OBJECT::Allocate(Arguments... someArguments)
	{
		if (myAllocatedMemory == nullptr)
			DL_ASSERT("Object pool tried to allocate objects before being initialized.");

		unsigned int index = GetIndexWithEnoughMemoryAndReserve(sizeof(Type));
		Type* adress = new (&(myAllocatedMemory[index])) Type(someArguments...);
		adress;

		PoolPointer<ReturnType> poolPointer(this, myIsActive, index, sizeof(Type));

		return poolPointer;
	}

	OBJECT_POOL_TEMPLATE
		void OBJECT_POOL_OBJECT::MergeAdjacentFreeMemory(const MemoryChunk& aMemoryChunk, CU::GrowingArray<MemoryChunk, int>& someChunks)
	{
		MemoryChunk currentMemoryChunk = aMemoryChunk;
		bool mergedUp = false;
		bool mergedDown = false;

		unsigned int indexToLookFor = currentMemoryChunk.myMemoryIndex + currentMemoryChunk.mySize;

		for (int i = 0; i < someChunks.Size(); i++)
		{
			
			if ( indexToLookFor == myChunks[i].myMemoryIndex)
			{
				someChunks[i].mySize += currentMemoryChunk.mySize;
				someChunks[i].myMemoryIndex = currentMemoryChunk.myMemoryIndex;
				mergedDown = true;
				currentMemoryChunk = someChunks[i];
				someChunks.RemoveCyclicAtIndex(i);
				break;
			}
		}

		indexToLookFor = currentMemoryChunk.myMemoryIndex;

		for (unsigned short i = 0; i < someChunks.Size(); i++)
		{
			
			if (someChunks[i].mySize + someChunks[i].myMemoryIndex == indexToLookFor)
			{
				someChunks[i].mySize += currentMemoryChunk.mySize;
				mergedUp = true;
				currentMemoryChunk = someChunks[i];
				someChunks.RemoveCyclicAtIndex(i);
				break;
			}

		}

		if (mergedUp == false && mergedDown == false)
		{
			someChunks.Add(currentMemoryChunk);
		}
		else
		{
			MergeAdjacentFreeMemory(currentMemoryChunk, someChunks);
		}
	}

	OBJECT_POOL_TEMPLATE
		unsigned int OBJECT_POOL_OBJECT::GetIndexWithEnoughMemoryAndReserve(const unsigned int aSizeInBytes)
	{
		unsigned int memoryIndex = 0;
		if (myChunks.Size() < 1)
		{
			__debugbreak();
			DL_ASSERT("ObjectPool ran out of space.");
		}

		myLock.lock();
		for (int i = myChunks.Size() - 1; i >= 0; i--)
		{
			if (myChunks[i].mySize >= aSizeInBytes)
			{
				memoryIndex = myChunks[i].myMemoryIndex;
				myChunks[i].mySize -= aSizeInBytes;

				if (myChunks[i].mySize > 0)
				{
					myChunks[i].myMemoryIndex += aSizeInBytes;
				}
				else
				{
					myChunks.RemoveCyclicAtIndex(i);
				}
				myLock.unlock();
				return memoryIndex;
			}
		}

		myLock.unlock();
		DL_ASSERT("ObjectPool ran out of space.");
		return memoryIndex;
	}

	OBJECT_POOL_TEMPLATE
		void OBJECT_POOL_OBJECT::DeAllocate(const ObjectPoolData& someData)
	{
		MemoryChunk chunk(someData.mySizeInBytes, someData.myMemoryIndex);
		myLock.lock();
		MergeAdjacentFreeMemory(chunk, myChunks);
		myLock.unlock();
	}

	OBJECT_POOL_TEMPLATE
		void* OBJECT_POOL_OBJECT::GetData(const unsigned int aMemoryIndex)
	{
		return reinterpret_cast<void*>(&(myAllocatedMemory[aMemoryIndex]));
	}
}

#undef OBJECT_POOL_OBJECT
#undef OBJECT_POOL_TEMPLATE
#undef TYPE_TEMPLATE

namespace CU = CommonUtilities;


#include "Common Utilities_Precompiled.h"
#include "MemoryWrapper.h"
#include "MemoryStackWalker.h"
#include <stdlib.h>
#include <mutex>

unsigned int locSizeInBytes = 1073741824;
std::mutex locMutex;

MemoryWrapper* MemoryWrapper::ourInstance = nullptr;

//void * operator new(std::size_t aSizeInBytes)
//{
//	if (MemoryWrapper::GetInstance() == nullptr)
//	{
//		MemoryWrapper::Create();
//	}
//	locMutex.lock();
//	void * pointer = MemoryWrapper::GetInstance()->Allocate(aSizeInBytes);
//	locMutex.unlock();
//	return pointer;
//}
//
//void* operator new[](std::size_t aSizeInBytes)
//{
//	if (MemoryWrapper::GetInstance() == nullptr)
//	{
//		MemoryWrapper::Create();
//	}
//	locMutex.lock();
//	void * pointer = MemoryWrapper::GetInstance()->Allocate(aSizeInBytes);
//	locMutex.unlock();
//	return pointer;
//}
//
//void operator delete(void * aPointer)
//{
//	locMutex.lock();
//	MemoryWrapper::GetInstance()->DeAllocate(aPointer);
//	locMutex.unlock();
//}
//
//void operator delete[](void* aPointer)
//{
//	locMutex.lock();
//	MemoryWrapper::GetInstance()->DeAllocate(aPointer);
//	locMutex.unlock();
//}

MemoryWrapper::MemoryWrapper()
{
	myStackWalkerIsInitialized = false;
}

MemoryWrapper::~MemoryWrapper()
{
	Destroy();
}

void MemoryWrapper::Create()
{
	ourInstance = static_cast<MemoryWrapper*>(malloc(sizeof(MemoryWrapper)));

	ourInstance->myMemory = static_cast<char*>(malloc(locSizeInBytes));

	CU::MemoryChunk chunk;
	chunk.myMemoryIndex = 0;
	chunk.mySize = locSizeInBytes;
//	chunk.myStackCall = "Original Memory";
	ourInstance->myFreeMemoryChunks = static_cast<CU::VectorOnStack<CU::MemoryChunk, 65536, unsigned int>*>(malloc(sizeof(CU::VectorOnStack<CU::MemoryChunk, 65536, unsigned int>)));
	ourInstance->myFreeMemoryChunks->Init();
	ourInstance->myFreeMemoryChunks->Add(chunk);
	ourInstance->myAllocatedChunks = static_cast<CU::VectorOnStack<CU::MemoryChunk, 65536, unsigned int>*>(malloc(sizeof(CU::VectorOnStack<CU::MemoryChunk, 65536, unsigned int>)));
	ourInstance->myAllocatedChunks->Init();

	
	ourInstance->myStackWalker = new MemoryStackWalker();
	ourInstance->myStackWalkerIsInitialized = true;
}

MemoryWrapper* MemoryWrapper::GetInstance()
{
	return MemoryWrapper::ourInstance;
}

void MemoryWrapper::Destroy()
{
	free(myMemory);
	free(ourInstance);
	free(myAllocatedChunks);
	free(myFreeMemoryChunks);
}

void MemoryWrapper::DeAllocate(void* aPointer)
{
	if (aPointer != nullptr)
	{
		unsigned int memoryIndex = static_cast<char*>(aPointer)-myMemory;
		if (static_cast<char*>(aPointer) < myMemory || memoryIndex > locSizeInBytes)
		{
			if (DL_Debug::Debug::GetInstance() != nullptr)
			{
				DL_PRINT("Trying to deallocate a corrupt pointer.");
			}
			return;
		}

		CU::MemoryChunk chunk = RemoveAndFetchAllocatedChunk(memoryIndex);
		AddChunk<65536>(chunk, *myFreeMemoryChunks, true);
	}
}

void* MemoryWrapper::Allocate(std::size_t aSizeInBytes)
{
	unsigned short i = 0;
	bool memoryReserved = false;
	CU::MemoryChunk reservedMemoryChunk;

	if (myStackWalkerIsInitialized == true)
	{
//		myStackWalker->ShowCallstack();
	}
//	reservedMemoryChunk.myStackCall = myStackWalkText;

	unsigned int byte4Aligned = aSizeInBytes % 4;
	if (byte4Aligned != 0)
	{
		aSizeInBytes += 4 - (aSizeInBytes % 4);
	}

	reservedMemoryChunk.mySize = aSizeInBytes;

	while (memoryReserved == false)
	{
		if (myFreeMemoryChunks->Size() <= i)
		{
			DL_ASSERT("Out of memory in memorywrapper :,(");
		}

		if ((*myFreeMemoryChunks)[i].mySize >= aSizeInBytes)
		{
			memoryReserved = true;
			reservedMemoryChunk.myMemoryIndex = (*myFreeMemoryChunks)[i].myMemoryIndex;

			(*myFreeMemoryChunks)[i].mySize -= aSizeInBytes;
			if ((*myFreeMemoryChunks)[i].mySize > 0)
			{
				(*myFreeMemoryChunks)[i].myMemoryIndex += aSizeInBytes;
			}
			else
			{
				myFreeMemoryChunks->RemoveAt(i);
			}
		}
		i++;
	}
	AddChunk<65536>(reservedMemoryChunk, *myAllocatedChunks, false);
	return myMemory + reservedMemoryChunk.myMemoryIndex;

}

void MemoryWrapper::StackWalkOutputText(char* someText)
{
	myStackWalkText = someText;
}



CU::MemoryChunk MemoryWrapper::RemoveAndFetchAllocatedChunk(unsigned int aMemoryIndex)
{
	unsigned int currentIndex = 0;
	unsigned int currentLowestIndex = 0;
	unsigned int currentHighestIndex = myAllocatedChunks->Size() - 1;
	bool chunkFound = false;
	while (chunkFound == false)
	{
		unsigned int difference = currentHighestIndex - currentLowestIndex;

		currentIndex = static_cast<unsigned int>(((difference)* 0.5f) + currentLowestIndex);

		if ((*myAllocatedChunks)[currentIndex].myMemoryIndex == aMemoryIndex)
		{
			CU::MemoryChunk chunk = (*myAllocatedChunks)[currentIndex];
			(*myAllocatedChunks).RemoveAt(currentIndex);
			chunkFound = true;
			return chunk;
		}
		if (difference == 1)
		{
			if ((*myAllocatedChunks)[currentIndex + 1].myMemoryIndex == aMemoryIndex)
			{
				CU::MemoryChunk chunk = (*myAllocatedChunks)[currentIndex + 1];
				myAllocatedChunks->RemoveAt(currentIndex + 1);
				chunkFound = true;
				return chunk;
			}
		}

		if ((*myAllocatedChunks)[currentIndex].myMemoryIndex > aMemoryIndex)
		{
			currentHighestIndex = currentIndex;
		}
		else
		{
			currentLowestIndex = currentIndex;
		}
	}
	DL_ASSERT("Couldn't find allocated memory by index");
	return CU::MemoryChunk();
}

bool MemoryWrapper::MergeNeighbours(CU::MemoryChunk& aChunk, unsigned int anIndex)
{
	bool merged = false;

	if (aChunk.myMemoryIndex + aChunk.mySize == (*myFreeMemoryChunks)[anIndex].myMemoryIndex)
	{
		(*myFreeMemoryChunks)[anIndex].mySize += aChunk.mySize;
		(*myFreeMemoryChunks)[anIndex].myMemoryIndex = aChunk.myMemoryIndex;
		aChunk = (*myFreeMemoryChunks)[anIndex];
		merged = true;
	}
	if (anIndex != 0 && (*myFreeMemoryChunks)[anIndex - 1].mySize + (*myFreeMemoryChunks)[anIndex - 1].myMemoryIndex == aChunk.myMemoryIndex)
	{
		(*myFreeMemoryChunks)[anIndex - 1].mySize += aChunk.mySize;
		if (merged == true)
		{
			myFreeMemoryChunks->RemoveAt(anIndex);
		}
		merged = true;
	}

	return merged;
}


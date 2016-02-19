#pragma once

#include "BaseObjectPool.h"
#include "DL_Assert.h"
#include "ObjectPoolData.h"
#include <atomic>
#include "SharedPointer.h"

#define OOP_TEMPLATE template<typename Type>
#define OOP_OBJECT PoolPointer<Type>

namespace CommonUtilities
{
	OOP_TEMPLATE
	class PoolPointer
	{
		template<unsigned int SizeInBytes>
		friend class ObjectPool;

	public:

		~PoolPointer();
		Type* operator->();
		const Type* operator->() const;
		PoolPointer& operator=(const PoolPointer& aPoolPointer);
		PoolPointer& operator=(nullptr_t);
		bool operator==(const PoolPointer& aPoolPointer) const;
		bool operator==(nullptr_t) const;
		bool operator!=(nullptr_t) const;
		PoolPointer();
		PoolPointer(const PoolPointer& aPoolPointer);
		PoolPointer(nullptr_t);
		Type& operator*();
		int GetReferenceCount() const;
		const Type& operator*() const;


		static PoolPointer& GetNullObject();

	private:

		PoolPointer(BaseObjectPool* anObjectPool, SharedPointer<bool>& aObjectPoolIsActive, const unsigned int aMemoryIndex, const unsigned int aSizeOfAllocation);

		void SubtractPointerCount();
		void AddPointerCount();

		BaseObjectPool* myObjectPool;

		volatile unsigned int myMemoryIndex;
		std::atomic<int>* myReferenceCount;
		//int* myReferenceCount;

		static PoolPointer ourPoolPointer;

		SharedPointer<bool> myObjectPoolIsActive;

		Type* myData;
		unsigned int myAllocationSize;
	};

	OOP_TEMPLATE
		void OOP_OBJECT::SubtractPointerCount()
	{
		if (myObjectPool != nullptr)
		{
			if (myObjectPoolIsActive != nullptr && *myObjectPoolIsActive != false)
			{
				if (--(*myReferenceCount) == 0)
				{
					//Type* data = static_cast<Type*> (myObjectPool->GetData(myMemoryIndex));

					if (myData != NULL)
					{
						myData->~Type(); //If warning C2027: use of undefined type; You need to include Type in cpp who is deleting poolpointer.
					}


					ObjectPoolData poolData(myAllocationSize, myMemoryIndex);
					myObjectPool->DeAllocate(poolData);

					SAFE_DELETE(myReferenceCount);
					myObjectPool = nullptr;
					myObjectPoolIsActive = nullptr;
					myMemoryIndex = UINT_MAX;

					myData = nullptr;
				}
			}
		}
	}

	OOP_TEMPLATE
		void OOP_OBJECT::AddPointerCount()
	{
		if (myObjectPool != nullptr)
		{
			*myReferenceCount += 1;
		}
	}

	OOP_TEMPLATE
		OOP_OBJECT OOP_OBJECT::ourPoolPointer = OOP_OBJECT::PoolPointer();

	OOP_TEMPLATE
		OOP_OBJECT::PoolPointer()
	{
		myReferenceCount = nullptr;
		myMemoryIndex = UINT_MAX;
		myObjectPool = nullptr;
		myObjectPoolIsActive = nullptr;
		myAllocationSize = 0;
		myData = nullptr;
	}

	OOP_TEMPLATE
		OOP_OBJECT::PoolPointer(nullptr_t)
	{
		SubtractPointerCount();
		myObjectPool = nullptr;
		myReferenceCount = nullptr;
		myObjectPoolIsActive = nullptr;
		myMemoryIndex = 0;
		myAllocationSize = 0;

		myData = nullptr;
	}

	OOP_TEMPLATE
		OOP_OBJECT::~PoolPointer()
	{
		SubtractPointerCount();
		myObjectPool = nullptr;
		myReferenceCount = nullptr;
		myObjectPoolIsActive = nullptr;
		myAllocationSize = 0;
		myMemoryIndex = 0;

	}

	OOP_TEMPLATE
		OOP_OBJECT::PoolPointer(const PoolPointer& aPoolPointer)
	{
		//SubtractPointerCount();

		myObjectPool = aPoolPointer.myObjectPool;
		myMemoryIndex = aPoolPointer.myMemoryIndex;
		myReferenceCount = aPoolPointer.myReferenceCount;
		myObjectPoolIsActive = aPoolPointer.myObjectPoolIsActive;
		myAllocationSize = aPoolPointer.myAllocationSize;
		AddPointerCount();

		myData = aPoolPointer.myData;
	}

	OOP_TEMPLATE
		OOP_OBJECT::PoolPointer(BaseObjectPool* anObjectPool, SharedPointer<bool>& aObjectPoolIsActive, const unsigned int aMemoryIndex, const unsigned int aSizeOfAllocation)
	{
		myObjectPool = anObjectPool;
		myObjectPoolIsActive = aObjectPoolIsActive;
		myMemoryIndex = aMemoryIndex;
		myAllocationSize = aSizeOfAllocation;

		myReferenceCount = new std::atomic < int > ;
		*myReferenceCount = 0;
		AddPointerCount();

		if (*myObjectPoolIsActive == true)
		{
			myData = reinterpret_cast<Type*>(myObjectPool->GetData(myMemoryIndex));
		}
		else
		{
			myData = nullptr;
		}
	}

	OOP_TEMPLATE
		OOP_OBJECT& OOP_OBJECT::operator=(const PoolPointer& aPoolPointer)
	{
		SubtractPointerCount();

		myObjectPoolIsActive = aPoolPointer.myObjectPoolIsActive;
		myObjectPool = aPoolPointer.myObjectPool;
		myMemoryIndex = aPoolPointer.myMemoryIndex;
		myReferenceCount = aPoolPointer.myReferenceCount;
		myAllocationSize = aPoolPointer.myAllocationSize;

		AddPointerCount();

		myData = aPoolPointer.myData;

		return *this;
	}

	OOP_TEMPLATE
		Type* OOP_OBJECT::operator->()
	{
		if (myObjectPool != nullptr)
		{
			return myData;
		}
		else
		{
			return nullptr;
		}
	}

	OOP_TEMPLATE
		Type& OOP_OBJECT::operator*()
	{
		return *myData;
	}
	OOP_TEMPLATE
		const Type& OOP_OBJECT::operator*() const
	{
		return *myData;
	}

	OOP_TEMPLATE
		int OOP_OBJECT::GetReferenceCount() const
	{
		return *myReferenceCount;
	}

	OOP_TEMPLATE
		const Type* OOP_OBJECT::operator->() const
	{
		if (myObjectPool != nullptr)
		{
			return myData;
		}
		else
		{
			return nullptr;
		}
	}

	OOP_TEMPLATE
		OOP_OBJECT& OOP_OBJECT::operator = (nullptr_t)
	{
		SubtractPointerCount();
		myMemoryIndex = UINT_MAX;
		myObjectPool = nullptr;
		myReferenceCount = nullptr;
		myObjectPoolIsActive = nullptr;
		myAllocationSize = 0;
		myData = nullptr;

		return *this;
	}

	OOP_TEMPLATE
		bool OOP_OBJECT::operator != (nullptr_t) const
	{
		if (myObjectPool == nullptr)
		{
			return false;
		}
		else if (myObjectPoolIsActive != nullptr && *myObjectPoolIsActive == false)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	OOP_TEMPLATE
		bool OOP_OBJECT::operator == (nullptr_t) const
	{
		if (myObjectPool == nullptr)
		{
			return true;
		}
		else if (myObjectPoolIsActive != nullptr && *myObjectPoolIsActive == false)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	OOP_TEMPLATE
		bool OOP_OBJECT::operator == (const PoolPointer& aPoolPointer) const
	{
		if (myMemoryIndex == aPoolPointer.myMemoryIndex)
		{
			return true;
		}
		else
		{
			return false;
		}
	}


	OOP_TEMPLATE
		OOP_OBJECT& OOP_OBJECT::GetNullObject()
	{
		return ourPoolPointer;
	}
}

#undef OOP_TEMPLATE
#undef OOP_OBJECT
#undef TYPE_TEMPLATE

namespace CU = CommonUtilities;
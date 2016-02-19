#pragma once
#include <new>
#include "DL_Debug.h"
#include "MemoryChunk.h"
#include "MemoryChunkLesser.h"

namespace CommonUtilities
{
#define MEMORY_BUCKET_TEMPLATE template<unsigned int ByteSize>
#define MEMORY_BUCKET_TYPE_TEMPLATE template<typename Type>
#define MEMORY_BUCKET_OBJECT MemoryBucket<ByteSize>

	template<unsigned int ByteSize = 1024>
	class MemoryBucket
	{
	public:
		MemoryBucket();
		~MemoryBucket();
		void Clear();

		MEMORY_BUCKET_TYPE_TEMPLATE
			Type* Allocate(const Type& someData);

		MemoryBucket& operator=(const MemoryBucket& aMemoryBucket);

	private:

		char myAllocatedMemory[ByteSize];

		unsigned int myIndexAdress;
	};
	MEMORY_BUCKET_TEMPLATE
		void MEMORY_BUCKET_OBJECT::Clear()
	{
		myIndexAdress = 0;
	}

	MEMORY_BUCKET_TEMPLATE
		MEMORY_BUCKET_OBJECT::MemoryBucket()
	{
		myIndexAdress = 0;
	}

	MEMORY_BUCKET_TEMPLATE
		MEMORY_BUCKET_OBJECT::~MemoryBucket()
	{
	}

	MEMORY_BUCKET_TEMPLATE
		MEMORY_BUCKET_TYPE_TEMPLATE
		Type* MEMORY_BUCKET_OBJECT::Allocate(const Type& someData)
	{
		unsigned int reservedAdress = myIndexAdress;

		myIndexAdress += sizeof(Type);

		if (myIndexAdress >= ByteSize)
		{
			DL_ASSERT("Memory bucket out of space!");
		}

		Type* adress = new (&(myAllocatedMemory[reservedAdress])) Type(someData);

		return adress;
	}


	MEMORY_BUCKET_TEMPLATE
		MEMORY_BUCKET_OBJECT& MEMORY_BUCKET_OBJECT::operator = (const MEMORY_BUCKET_OBJECT& aMemoryBucket)
	{
		myAllocatedMemory = aMemoryBucket.myAllocatedMemory;
		myIndexAdress = aMemoryBucket.myIndexAdress;
		return *this;
	}

#undef MEMORY_BUCKET_TEMPLATE
#undef MEMORY_BUCKET_OBJECT
#undef MEMORY_BUCKET_TYPE_TEMPLATE
}

namespace CU = CommonUtilities;
#pragma once
#include "MemoryBucket.h"
#include "StaticArray.h"

namespace CommonUtilities
{
#define ENUM_DICTIONARY_TEMPLATE template<typename Key, unsigned int ByteSize , int BucketAmount>
#define ENUM_DICTIONARY_TYPE_TEMPLATE template<typename Type>
#define ENUM_DICTIONARY_OBJECT EnumDictionary<Key, ByteSize, BucketAmount>


	template<typename Key, unsigned int ByteSize = 1024, int BucketAmount = 512>
	class EnumDictionary
	{
	public:
		EnumDictionary();
		~EnumDictionary();

		void Clear();

		ENUM_DICTIONARY_TYPE_TEMPLATE
			void SetValue(const Key& aKey, const Type& someData);

		ENUM_DICTIONARY_TYPE_TEMPLATE
			Type* GetValue(const Key& aKey);

		ENUM_DICTIONARY_TYPE_TEMPLATE
			const Type* GetValue(const Key& aKey) const;

		ENUM_DICTIONARY_TYPE_TEMPLATE
			const bool ValueExists(const Key& aKey) const;

		EnumDictionary& operator=(const EnumDictionary& aDictionaryObject);

	private:
		CU::StaticArray<void*, BucketAmount> myPointers;
		CU::MemoryBucket<ByteSize> myMemoryBucket;
	};

	ENUM_DICTIONARY_TEMPLATE
		ENUM_DICTIONARY_OBJECT& ENUM_DICTIONARY_OBJECT::operator = (const ENUM_DICTIONARY_OBJECT& aDictionaryObject)
	{
		return *this;
	}

	ENUM_DICTIONARY_TEMPLATE
		void ENUM_DICTIONARY_OBJECT::Clear()
	{
		myPointers.DeleteAll();
		myMemoryBucket.Clear();
	}


	ENUM_DICTIONARY_TEMPLATE
		ENUM_DICTIONARY_OBJECT::EnumDictionary()
	{
		for (int i = 0; i < BucketAmount; i++)
		{
			myPointers[i] = nullptr;
		}
	}

	ENUM_DICTIONARY_TEMPLATE
		ENUM_DICTIONARY_OBJECT::~EnumDictionary()
	{

	}

	ENUM_DICTIONARY_TEMPLATE
		ENUM_DICTIONARY_TYPE_TEMPLATE
		void ENUM_DICTIONARY_OBJECT::SetValue(const Key& aKey, const Type& someData)
	{
		int index = static_cast<int>(aKey);

		if (myPointers[index] != nullptr)
		{
			Type* storedData = static_cast<Type*>(myPointers[index]);

			*storedData = someData;
		}
		else
		{
			myPointers[index] = myMemoryBucket.Allocate(someData);
		}
	}

	ENUM_DICTIONARY_TEMPLATE
		ENUM_DICTIONARY_TYPE_TEMPLATE
		Type* ENUM_DICTIONARY_OBJECT::GetValue(const Key& aKey)
	{
		return static_cast<Type*>(myPointers[static_cast<int>(aKey)]);
	}

	ENUM_DICTIONARY_TEMPLATE
		ENUM_DICTIONARY_TYPE_TEMPLATE
		const Type* ENUM_DICTIONARY_OBJECT::GetValue(const Key& aKey) const
	{
		return static_cast<Type*>(myPointers[static_cast<int>(aKey)]);
	}

	ENUM_DICTIONARY_TEMPLATE
		ENUM_DICTIONARY_TYPE_TEMPLATE
		const bool ENUM_DICTIONARY_OBJECT::ValueExists(const Key& aKey) const
	{
		if (myPointers[static_cast<int>(aKey)] == nullptr)
		{
			return false;
		}
		return true;
	}

#undef ENUM_DICTIONARY_TEMPLATE
#undef ENUM_DICTIONARY_OBJECT
#undef ENUM_DICTIONARY_TYPE_TEMPLATE
#ifdef _DEBUG_DICTIONARY
#undef _DEBUG_DICTIONARY
#endif

}

namespace CU = CommonUtilities;
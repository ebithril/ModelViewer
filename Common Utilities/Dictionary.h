#pragma once
#include "Map.h"
#include "MemoryBucket.h"

//#define _DEBUG_DICTIONARY

#ifdef _DEBUG_DICTIONARY
#include <typeinfo>
#endif

namespace CommonUtilities
{
#define DICTIONARY_TEMPLATE template<typename Key, unsigned int ByteSize , unsigned short BucketAmount>
#define DICTIONARY_TYPE_TEMPLATE template<typename Type>
#define DICTIONARY_OBJECT Dictionary<Key, ByteSize, BucketAmount>


	template<typename Key, unsigned int ByteSize = 1024, unsigned short BucketAmount = 512>
	class Dictionary
	{
	public:
		Dictionary();
		~Dictionary();

		void Clear();

		DICTIONARY_TYPE_TEMPLATE
			void SetValue(const Key& aKey, const Type& someData);

		DICTIONARY_TYPE_TEMPLATE
			Type* GetValue(const Key& aKey);

		DICTIONARY_TYPE_TEMPLATE
			bool ValueExists(const Key& aKey);

		Dictionary& operator=(const Dictionary& aDictionaryObject);

	private:
		CU::Map<Key, void*, BucketAmount> myMap;
		CU::MemoryBucket<ByteSize> myMemoryBucket;

#ifdef _DEBUG_DICTIONARY
		CU::Map<std::string, const std::type_info*, BucketAmount> myTypeInfoMap;

		DICTIONARY_TYPE_TEMPLATE
			void CheckType(const Key& aKey, const Type& someData);
#endif

	};

	DICTIONARY_TEMPLATE
		DICTIONARY_OBJECT& DICTIONARY_OBJECT::operator = (const DICTIONARY_OBJECT& aDictionaryObject)
	{
		return *this;
	}

	DICTIONARY_TEMPLATE
		void DICTIONARY_OBJECT::Clear()
	{
		myMap.Clear();
		myMemoryBucket.Clear();

#ifdef _DEBUG_DICTIONARY
		myTypeInfoMap.Clear();

#endif
	}


	DICTIONARY_TEMPLATE
		DICTIONARY_OBJECT::Dictionary()
	{

	}

	DICTIONARY_TEMPLATE
		DICTIONARY_OBJECT::~Dictionary()
	{

	}

	DICTIONARY_TEMPLATE
		DICTIONARY_TYPE_TEMPLATE
		void DICTIONARY_OBJECT::SetValue(const Key& aKey, const Type& someData)
	{
		if (myMap.KeyExists(aKey) == true)
		{
			Type* storedData = static_cast<Type*>(myMap.Get(aKey));

			*storedData = someData;

#ifdef _DEBUG_DICTIONARY
			CheckType(aKey, someData);
#endif
		}
		else
		{
			myMap.Insert(aKey, myMemoryBucket.Allocate(someData));

#ifdef _DEBUG_DICTIONARY
			myTypeInfoMap.Insert(aKey, &typeid(someData));
#endif		
		}
	}

	DICTIONARY_TEMPLATE
		DICTIONARY_TYPE_TEMPLATE
		Type* DICTIONARY_OBJECT::GetValue(const Key& aKey)
	{
		if (myMap.KeyExists(aKey) == true)
		{

#ifdef _DEBUG_DICTIONARY
			CheckType(aKey, Type());
#endif
			return static_cast<Type*>(myMap.Get(aKey));
		}
		else
		{
			//std::string errorText = "No key found with value: " + aKey;
			//DL_ASSERT(errorText.c_str());
			return nullptr;
		}
	}

	DICTIONARY_TEMPLATE
		DICTIONARY_TYPE_TEMPLATE
		bool DICTIONARY_OBJECT::ValueExists(const Key& aKey)
	{
		if (myMap.KeyExists(aKey) == true)
		{
			return true;
		}
		return false;
	}


#ifdef _DEBUG_DICTIONARY

	DICTIONARY_TEMPLATE
		DICTIONARY_TYPE_TEMPLATE
		void DICTIONARY_OBJECT::CheckType(const Key& aKey, const Type& someData)
	{
		const std::type_info* type = myTypeInfoMap.Get(aKey);
		std::string typeName = type->name();
		std::string dataTypeName = typeid(someData).name();

		if (typeName != dataTypeName)
		{
			std::string errortext = "Dictionary, key typeid *";
			errortext += typeid(myTypeInfoMap.Get(aKey)).name();
			errortext += "* doesn't match argument typeid *";
			errortext += typeid(someData).name();
			errortext += "*";
			DL_ASSERT(errortext.c_str());
		}
	}

#endif

#undef DICTIONARY_TEMPLATE
#undef DICTIONARY_OBJECT
#undef DICTIONARY_TYPE_TEMPLATE
#ifdef _DEBUG_DICTIONARY
#undef _DEBUG_DICTIONARY
#endif
}

namespace CU = CommonUtilities;


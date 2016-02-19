#pragma once
#include "GrowingArray.h"
#include "MurmurHash3.h"
#include "StaticArray.h"
#include <sstream>

namespace std
{
	inline string to_string(string aString);
	string to_string(string aString)
	{
		return aString;
	}
}

namespace CommonUtilities
{
#define MAP_TEMPLATE template <typename Key, typename Value, unsigned int BucketAmount = 1000>
#define MAP_OBJECT Map<Key, Value, BucketAmount>


	MAP_TEMPLATE
	struct KeyValue
	{
	public:
		Key myKey;
		Value myValue;
	};

	MAP_TEMPLATE
	class Map
	{
	public:
		Map();
		~Map();

		void Clear();

		void Insert(const Key &aKey, const Value &aValue);
		Value& Get(const Key &aKey);
		void Delete(const Key &aKey);
		bool KeyExists(const Key &aKey);
		Value& operator[](const Key &aKey);
		Map& operator=(const Map& aMap);

	private:

		CU::StaticArray<CU::GrowingArray<KeyValue<Key, Value>, unsigned int>, BucketAmount> myBuckets;
		const unsigned int mySeed = 1337;

		const unsigned int GetHashKey(const Key &aKey) const;
		const bool SearchBucketWithKey(const unsigned int aBucketIndex, const Key &aKey, Value*& anOutValue);

	};

	MAP_TEMPLATE
		MAP_OBJECT::Map()
	{
		for (unsigned int i = 0; i < BucketAmount; i++)
		{
			myBuckets[i].Init(4);
		}
	}

	MAP_TEMPLATE
		MAP_OBJECT::~Map()
	{

	}

	MAP_TEMPLATE
		void MAP_OBJECT::Clear()
	{
		for (unsigned int i = 0; i < BucketAmount; i++)
		{
			myBuckets[i].RemoveAll();
		}
	}


	MAP_TEMPLATE
		void MAP_OBJECT::Insert(const Key &aKey, const Value &aValue)
	{
		if (KeyExists(aKey) == true)
		{
			Get(aKey) = aValue;
		}
		else
		{
			KeyValue<Key, Value> newKeyValue;
			newKeyValue.myKey = aKey;
			newKeyValue.myValue = aValue;

			unsigned int bucketIndex = GetHashKey(aKey);
			myBuckets[static_cast<int>(bucketIndex)].Add(newKeyValue);
		}
	}

	MAP_TEMPLATE
		Value& MAP_OBJECT::Get(const Key &aKey)
	{
		unsigned int bucketIndex = GetHashKey(aKey);
		Value* newValue = nullptr;

		bool foundValue = SearchBucketWithKey(bucketIndex, aKey, newValue);

		if (foundValue == false)
		{
			std::string errorText = "Couldn't find key ";
			errorText += std::to_string(aKey);
			errorText += " in map.";
			DL_ASSERT(errorText.c_str());
		}

		return *newValue;
	}

	MAP_TEMPLATE
		void MAP_OBJECT::Delete(const Key &aKey)
	{
		unsigned int bucketIndex = GetHashKey(aKey);

		for (unsigned int i = 0; i < myBuckets[bucketIndex].Size(); i++)
		{
			if (myBuckets[bucketIndex][i].myKey == aKey)
			{
				myBuckets[bucketIndex].RemoveCyclicAtIndex(i);
				return;
			}
		}

		std::string errorText = "No key to delete with key " + aKey;
		DL_ASSERT(errorText.c_str());
	}

	MAP_TEMPLATE
		bool MAP_OBJECT::KeyExists(const Key &aKey)
	{
		unsigned int bucketIndex = GetHashKey(aKey);
		Value* newValue;

		return SearchBucketWithKey(bucketIndex, aKey, newValue);
	}

	MAP_TEMPLATE
		Value& MAP_OBJECT::operator[](const Key &aKey)
	{
		if (KeyExists(aKey) == true)
		{
			return Get(aKey);
		}
		else
		{
			Insert(aKey, Value());
			return Get(aKey);
		}
	}


	MAP_TEMPLATE
		MAP_OBJECT& MAP_OBJECT::operator = (const MAP_OBJECT& aMap)
	{
		mySeed = aMap.mySeed;
		myBuckets = aMap.myBuckets;
		return *this;
	}

	MAP_TEMPLATE
		const unsigned int MAP_OBJECT::GetHashKey(const Key &aKey) const
	{

		std::stringstream ss;
		ss << aKey;

		unsigned int index = 0;
		MurmurHash3_x86_32(ss.str().c_str(), ss.str().length(), mySeed, &index);
		index = index % BucketAmount;
		return index;
	}

	MAP_TEMPLATE
		const bool MAP_OBJECT::SearchBucketWithKey(const unsigned int aBucketIndex, const Key &aKey, Value*& anOutValue)
	{
		for (unsigned int i = 0; i < myBuckets[aBucketIndex].Size(); i++)
		{
			if (myBuckets[aBucketIndex][i].myKey == aKey)
			{
				anOutValue = &myBuckets[aBucketIndex][i].myValue;
				return true;
			}
		}

		return false;
	}

#undef MAP_TEMPLATE
#undef MAP_OBJECT
}

namespace CU = CommonUtilities;

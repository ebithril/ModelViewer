#pragma once

namespace CommonUtilities
{
	struct ObjectPoolData
	{
		inline ObjectPoolData();
		inline ObjectPoolData( const unsigned int aSizeInBytes, const unsigned int aMemoryIndex);
		inline ~ObjectPoolData();
	
		unsigned int mySizeInBytes;
		unsigned int myMemoryIndex;
	};

	ObjectPoolData::ObjectPoolData()
	{

	}

	ObjectPoolData::ObjectPoolData(const unsigned int aSizeInBytes, const unsigned int aMemoryIndex)
	{
		mySizeInBytes = aSizeInBytes;
		myMemoryIndex = aMemoryIndex;
	}

	ObjectPoolData::~ObjectPoolData()
	{

	}

}

namespace CU = CommonUtilities;
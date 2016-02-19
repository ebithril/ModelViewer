#pragma once
#include "MemoryChunk.h"

namespace CommonUtilities
{
	class MemoryChunkLesser
	{
	public:

		inline const bool operator()(const MemoryChunk& someLeftData, const MemoryChunk& someRightData) const;

	private:
	};

	
	const bool MemoryChunkLesser::operator()(const MemoryChunk& someLeftData, const MemoryChunk& someRightData) const
	{
		if (someLeftData.mySize < someRightData.mySize)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

}

namespace CU = CommonUtilities;


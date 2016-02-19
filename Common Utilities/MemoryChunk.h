#pragma once

#include "CUString.h"

namespace CommonUtilities
{
	struct MemoryChunk
	{
		inline MemoryChunk();
		inline MemoryChunk(unsigned int aSize, unsigned int anIndex);

		unsigned int mySize;
		unsigned int myMemoryIndex;
		//CU::String<1024> myStackCall;
	};

	MemoryChunk::MemoryChunk()
	{

	}

	MemoryChunk::MemoryChunk(unsigned int aSize, unsigned int anIndex)
	{
		mySize = aSize;
		myMemoryIndex = anIndex;
	}
}



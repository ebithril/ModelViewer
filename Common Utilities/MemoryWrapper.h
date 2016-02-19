#pragma once
#include "VectorOnStack.h"
#include "MemoryChunk.h"
#include "CUString.h"

class MemoryStackWalker;

class MemoryWrapper
{
public:
	~MemoryWrapper();

	static void Create();
	static MemoryWrapper* GetInstance();
	void Destroy();

	void* Allocate(std::size_t aSizeInBytes);
	void DeAllocate(void* aPointer);

	void StackWalkOutputText(char* someText);

private:
	MemoryWrapper();

	template <unsigned int Size>
	void AddChunk(const CU::MemoryChunk& aChunk, CU::VectorOnStack<CU::MemoryChunk, Size, unsigned int>& anArray, bool aShouldMerge);
	CU::MemoryChunk RemoveAndFetchAllocatedChunk(unsigned int aMemoryIndex);

	bool MergeNeighbours(CU::MemoryChunk& chunk, unsigned int anIndex);

	static MemoryWrapper* ourInstance;

	char* myMemory;

	MemoryStackWalker* myStackWalker;
	bool myStackWalkerIsInitialized;

	CU::String<1024> myStackWalkText;
	CU::VectorOnStack<CU::MemoryChunk, 65536, unsigned int>* myFreeMemoryChunks;
	CU::VectorOnStack<CU::MemoryChunk, 65536, unsigned int>* myAllocatedChunks;
};

template <unsigned int Size>
void MemoryWrapper::AddChunk(const CU::MemoryChunk& aChunk, CU::VectorOnStack<CU::MemoryChunk, Size, unsigned int>& anArray, bool aShouldMerge)
{
	if (anArray.Size() == 0 || anArray[anArray.Size() - 1].myMemoryIndex < aChunk.myMemoryIndex)
	{
		anArray.Add(aChunk);
		return;
	}

	unsigned int currentIndex = 0;
	unsigned int currentLowestIndex = 0;
	unsigned int currentHighestIndex = anArray.Size() - 1;
	bool chunkPlaced = false;
	while (chunkPlaced == false)
	{
		unsigned int difference = currentHighestIndex - currentLowestIndex;
		if (difference <= 1)
		{
			CU::MemoryChunk chunk = aChunk;
			if (anArray[currentLowestIndex].myMemoryIndex > aChunk.myMemoryIndex)
			{
				if (aShouldMerge == true)
				{
					if (MergeNeighbours(chunk, currentLowestIndex) == false)
					{
						anArray.Insert(currentLowestIndex, chunk);
					}
				}
				else
				{
					anArray.Insert(currentLowestIndex, chunk);

				}
				
			}
			else if (anArray[currentHighestIndex].myMemoryIndex > chunk.myMemoryIndex)
			{
				if (aShouldMerge == true)
				{
					if (MergeNeighbours(chunk, currentHighestIndex) == false)
					{
						anArray.Insert(currentHighestIndex, chunk);
					}
				}
				else
				{
					anArray.Insert(currentHighestIndex, chunk);
				}
			}
			else
			{			
				if (anArray.Size() > currentHighestIndex + 1)
				{
					if (aShouldMerge == true)
					{
						if (MergeNeighbours(chunk, currentLowestIndex) == true)
						{
							anArray.Insert(currentHighestIndex + 1, chunk);
						}
					}
					else
					{
						anArray.Insert(currentHighestIndex + 1, chunk);

					}		
				}
				else
				{
					if (MergeNeighbours(chunk, currentHighestIndex) == true)
					{
						anArray.Add(chunk);
					}
				}
			}
			chunkPlaced = true;
			return;
		}

		currentIndex = static_cast<unsigned int>(((difference)* 0.5f) + currentLowestIndex);

		if (anArray[currentIndex].myMemoryIndex > aChunk.myMemoryIndex)
		{
			currentHighestIndex = currentIndex;
		}
		else
		{
			currentLowestIndex = currentIndex;
		}
	}
}
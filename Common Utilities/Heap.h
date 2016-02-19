#pragma once
#include "GrowingArray.h"

namespace CommonUtilities
{
	template<typename DataType, typename Comparer>
	class Heap
	{
	public:

		Heap();
		Heap(GrowingArray<DataType> anArray);
		~Heap();

		void Enqueue(const DataType& someData);
		DataType Dequeue();
		bool IsEmpty() const;
		void FloydOrganize();

	private:

		void SwitchIndexPositions(unsigned short aFirstIndex, unsigned short aSecondIndex);
		void RecursiveDownTreeUntilNotTrue(unsigned short anIndex);
		void PlaceCorrectlyUp(unsigned short anIndex);
		void PlaceCorrectlyDown(unsigned short anIndex);


		GrowingArray<DataType> myData;
		Comparer myComparer;
	};

	template<typename DataType, typename Comparer>
	Heap<DataType, Comparer>::Heap()
	{
		myData.Init(8);
	}


	template<typename DataType, typename Comparer>
	Heap<DataType, Comparer>::~Heap()
	{

	}

	template<typename DataType, typename Comparer>
	Heap<DataType, Comparer>::Heap(GrowingArray<DataType> anArray)
	{
		myData = anArray;
		FloydOrganize();
	}



	template<typename DataType, typename Comparer>
	void Heap<DataType, Comparer>::Enqueue(const DataType& someData)
	{
		myData.Add(someData);
		PlaceCorrectlyUp(unsigned short(myData.Size() - 1));
	}

	template<typename DataType, typename Comparer>
	DataType Heap<DataType, Comparer>::Dequeue()
	{
		assert(myData.Size() > 0 && "Can't dequeue an empty Heap");
		unsigned short lastIndexPos = unsigned short((myData.Size() - 1));
		DataType returnData = myData[0];
		SwitchIndexPositions(0, lastIndexPos);
		myData.RemoveCyclicAtIndex(lastIndexPos);
		RecursiveDownTreeUntilNotTrue(0);
		//PlaceCorrectlyDown(0);
		return returnData;
	}



	template<typename DataType, typename Comparer>
	bool Heap<DataType, Comparer>::IsEmpty() const
	{
		return (myData.Size() == 0);
	}


	template<typename DataType, typename Comparer>
	void Heap<DataType, Comparer>::FloydOrganize()
	{
		short indexOfLastNonLeafNode = short((myData.Size() - 1) / 2);

		for (short i = indexOfLastNonLeafNode; i >= 0; i--)
		{
			unsigned short pos = i;
			RecursiveDownTreeUntilNotTrue(pos);
		}

	}


	template<typename DataType, typename Comparer>
	void Heap<DataType, Comparer>::SwitchIndexPositions(unsigned short aFirstIndex, unsigned short aSecondIndex)
	{
		DataType dataHolder = myData[aFirstIndex];
		myData[aFirstIndex] = myData[aSecondIndex];
		myData[aSecondIndex] = dataHolder;
	}


	template<typename DataType, typename Comparer>
	void Heap<DataType, Comparer>::RecursiveDownTreeUntilNotTrue(unsigned short anIndex)
	{
		unsigned short leftChildIndex = anIndex * 2 + 1;
		unsigned short rightChildIndex = anIndex * 2 + 2;


		if (rightChildIndex < myData.Size())
		{
			if (myComparer(myData[leftChildIndex], myData[rightChildIndex]) == true &&
				myComparer(myData[leftChildIndex], myData[anIndex]) == true)
			{
				SwitchIndexPositions(leftChildIndex, anIndex);
				RecursiveDownTreeUntilNotTrue(leftChildIndex);
			}
			else if (myComparer(myData[rightChildIndex], myData[anIndex]) == true)
			{
				SwitchIndexPositions(rightChildIndex, anIndex);
				RecursiveDownTreeUntilNotTrue(rightChildIndex);
			}

		}
		else if (leftChildIndex < myData.Size())
		{
			if (myComparer(myData[leftChildIndex], myData[anIndex]) == true)
			{
				SwitchIndexPositions(leftChildIndex, anIndex);
				RecursiveDownTreeUntilNotTrue(leftChildIndex);
			}
		}

	}

	template<typename DataType, typename Comparer>
	void Heap<DataType, Comparer>::PlaceCorrectlyUp(unsigned short anIndex)
	{
		unsigned short parentIndex = (anIndex - 1) / 2;

		if (parentIndex >= 0 && parentIndex != anIndex)
		{
			if (myComparer(myData[anIndex], myData[parentIndex]) == true)
			{
				SwitchIndexPositions(anIndex, parentIndex);
				PlaceCorrectlyUp(parentIndex);
			}
		}
	}

	template<typename DataType, typename Comparer>
	void Heap<DataType, Comparer>::PlaceCorrectlyDown(unsigned short anIndex)
	{
		unsigned short leftChildIndex = anIndex * 2 + 1;
		unsigned short rightChildIndex = anIndex * 2 + 2;

 		if (rightChildIndex < myData.Size())
		{
			if (myComparer(&myData[leftChildIndex], &myData[rightChildIndex]) == true &&
				myComparer(&myData[leftChildIndex], &myData[anIndex]) == true)
			{
				SwitchIndexPositions(anIndex, rightChildIndex);
				PlaceCorrectlyDown(rightChildIndex);
			}
			else if (myComparer(&myData[leftChildIndex], &myData[anIndex]) == true)
			{
				SwitchIndexPositions(anIndex, leftChildIndex);
				PlaceCorrectlyDown(leftChildIndex);
			}
		}
		else if (leftChildIndex < myData.Size())
		{
			if (myComparer(&myData[anIndex], &myData[leftChildIndex]) == true)
			{
				SwitchIndexPositions(anIndex, leftChildIndex); 
				PlaceCorrectlyDown(rightChildIndex);
			}
		}
	}
}

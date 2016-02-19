#pragma once
#include "Macros.h"
#include <assert.h>

namespace CommonUtilities
{
	template <typename Type, int Size>
	class StaticArray
	{
	public:
		StaticArray();
		StaticArray(const StaticArray& aStaticArray);

		~StaticArray();

		StaticArray& operator=(const StaticArray& aStaticArray);

		inline const Type& operator[](const int& aIndex) const;
		inline Type& operator[](const int& aIndex);


		// Utility functions
		inline void Insert(const int aIndex,const Type& aObject);
		inline void DeleteAll();

	private:
		Type myArray[Size];
	};

	template <typename Type, int Size>
	StaticArray<Type, Size>::StaticArray() 
	{
	}

	template <typename Type, int Size>
	StaticArray<Type, Size>::
		StaticArray(const StaticArray<Type, Size> &aStaticArray)
	{
		*this = aStaticArray;
	}

	template <typename Type, int Size>
	StaticArray<Type, Size>::
		~StaticArray()
	{
	}

	template <typename Type, int Size>
	StaticArray<Type, Size>& 
		StaticArray<Type, Size>::
		operator=(const StaticArray<Type, Size> &aStaticArray)
	{
		for (int i = 0; i < Size; i++)
		{
			myArray[i] = aStaticArray.myArray[i];
		}

		return *this;
	}

	template <typename Type, int Size>
	inline const Type& StaticArray<Type, Size>::
		operator[](const int& aIndex) const
	{
		assert(aIndex < Size && aIndex >= 0 && "Out of bounds");
		return myArray[aIndex];
	}

	template <typename Type, int Size>
	inline Type& StaticArray<Type, Size>::
		operator[](const int& aIndex)
	{
		assert(aIndex < Size && aIndex >= 0 && "Out of bounds");
		return myArray[aIndex];
	}

	// Utility functions
	template <typename Type, int Size>
	inline void StaticArray<Type, Size>::
		Insert(const int aIndex,const Type& aObject)
	{
		assert(Size > 1 && "Array is not big enough to insert.");
		assert(aIndex < Size && aIndex >= 0 && "Index out of bounds.");
		//start on second from end
		for (int i = (Size - 1); i > aIndex; i--)
		{
			myArray[i] = myArray[i - 1];
		}
		myArray[aIndex] = aObject;
	}

	template <typename Type, int Size>
	inline void StaticArray<Type, Size>::
		DeleteAll()
	{
		for (unsigned i = 0; i < Size; i++)
		{
			SAFE_DELETE(myArray[i])
		}
	}
}

namespace CU = CommonUtilities;
#pragma once
//Constant expression in CopyVector function Warning lvl 4 doesn't like.
//Is constant in each separate class but differs in the different template classes.
#pragma warning(disable : 4127) 


#include <assert.h>
#include "Macros.h"
#include "DL_Assert.h"

namespace CommonUtilities
{
	template <typename Type, int MaxSize, typename CountType = unsigned short, bool UseSafeModeFlag = true>
	class VectorOnStack
	{
	public:
		VectorOnStack();
		VectorOnStack(const VectorOnStack& aVectorOnStack);
	

		~VectorOnStack();

		VectorOnStack& operator=(const VectorOnStack & aVectorOnStack);

		inline void Init();
		inline const Type& operator[](const CountType & aIndex) const;
		inline Type& operator[](const CountType & aIndex);

		inline void Add(const Type& aObject);
		inline void Insert(const CountType aIndex, const Type& aObject);
		inline void DeleteCyclic(const Type& aObject);
		inline void DeleteCyclicAtIndex(const CountType aItemNumber);
		inline void RemoveCyclic(const Type& aObject);
		inline void RemoveCyclicAtIndex(const CountType aItemNumber);
		inline void RemoveAt(const CountType anIndex);
		inline void CopyDownUntilEnd(const CountType aItemNumber);

		inline void Clear();
		inline void DeleteAll();

		inline CountType Size() const;

	private:
		Type myArray[MaxSize];
		CountType myCurrentSize;

#ifdef _DEBUG
		CountType mySize;
#endif // _DEBUG

	};

	template <typename Type, int MaxSize, typename CountType = unsigned short, bool UseSafeModeFlag = true>
	void VectorOnStack<Type, MaxSize, CountType, UseSafeModeFlag>::
	Init()
	{
		myCurrentSize = 0;
#ifdef _DEBUG
		mySize = MaxSize;
#endif // _DEBUG
	}


	template <typename Type, int MaxSize, typename CountType = unsigned short, bool UseSafeModeFlag = true>
	VectorOnStack<Type, MaxSize, CountType, UseSafeModeFlag>::VectorOnStack()


	{
		myCurrentSize = 0;

#ifdef _DEBUG
		mySize = MaxSize;
#endif // _DEBUG
	}

	template <typename Type, int MaxSize, typename CountType = unsigned short, bool UseSafeModeFlag = true>
	VectorOnStack<Type, MaxSize, CountType, UseSafeModeFlag>::
		VectorOnStack(
		const VectorOnStack<Type, MaxSize, CountType, UseSafeModeFlag>& aVectorOnStack)
	{
		*this = aVectorOnStack;
	}

	template <typename Type, int MaxSize, typename CountType = unsigned short, bool UseSafeModeFlag = true>
	VectorOnStack<Type, MaxSize, CountType, UseSafeModeFlag>::
		~VectorOnStack()
	{

	}

	template <typename Type, int MaxSize, typename CountType = unsigned short, bool UseSafeModeFlag = true>
	VectorOnStack<Type, MaxSize, CountType, UseSafeModeFlag>&
		VectorOnStack<Type, MaxSize, CountType, UseSafeModeFlag>::
		operator=(const VectorOnStack & aVectorOnStack)
	{
		if (UseSafeModeFlag == true)
		{
			for (int i = 0; i < MaxSize; i++)
			{
				myArray[i] = aVectorOnStack.myArray[i];
			}
			myCurrentSize = aVectorOnStack.Size();
			
#ifdef _DEBUG
			mySize = MaxSize;
#endif // _DEBUG
		}
		else
		{
			std::memcpy(this, &aVectorOnStack, sizeof(*this));
			myCurrentSize = aVectorOnStack.Size();
			
#ifdef _DEBUG
			mySize = MaxSize;
#endif // _DEBUG
		}
		return *this;
	}

	template <typename Type, int MaxSize, typename CountType = unsigned short, bool UseSafeModeFlag = true>
	inline const Type& VectorOnStack<Type, MaxSize, CountType, UseSafeModeFlag>::
		operator[](const CountType & aIndex) const
	{
		if (aIndex > Size() || aIndex < 0)
		{
			DL_ASSERT("Index out of bounds");
		}
		return myArray[aIndex];
	}

	template <typename Type, int MaxSize, typename CountType = unsigned short, bool UseSafeModeFlag = true>
	inline Type& VectorOnStack<Type, MaxSize, CountType, UseSafeModeFlag>::
		operator[](const CountType & aIndex)
	{
		if (aIndex > Size() || aIndex < 0)
		{
			DL_ASSERT("Index out of bounds");
		}
		return myArray[aIndex];
	}

	template <typename Type, int MaxSize, typename CountType = unsigned short, bool UseSafeModeFlag = true>
	inline void VectorOnStack<Type, MaxSize, CountType, UseSafeModeFlag>::
		Add(const Type& aObject)
	{
		if (MaxSize <= Size())
		{
			DL_ASSERT("Array is full! Can't add any more data.");
		}

		myArray[Size()] = aObject;
		myCurrentSize++;
	}

	template <typename Type, int MaxSize, typename CountType = unsigned short, bool UseSafeModeFlag = true>
	inline void VectorOnStack<Type, MaxSize, CountType, UseSafeModeFlag>::
		Insert(const CountType aIndex, const Type& aObject)
	{
		if (aIndex > Size() || aIndex < 0)
		{
			DL_ASSERT("Index out of bounds");
		}
		
		if (MaxSize <= Size())
		{
			DL_ASSERT("Array is full! Can't add any more data.");
		}

		for (CountType i = (Size()); i > aIndex; i--)
		{
			myArray[i] = myArray[i-1];
		}
		myArray[aIndex] = aObject;
		myCurrentSize++;
	}

	template <typename Type, int MaxSize, typename CountType = unsigned short, bool UseSafeModeFlag = true>
	inline void VectorOnStack<Type, MaxSize, CountType, UseSafeModeFlag>::
		DeleteCyclic(const Type& aObject)
	{
		bool objectDeleted = false;
		CountType iterator = 0;
		while (objectDeleted == false)
		{
			if (iterator >= Size())
			{
				DL_ASSERT("Couldn't find data to delete.");
			}
			if (myArray[iterator] == aObject)
			{
				SAFE_DELETE(myArray[iterator])
					myArray[iterator] = myArray[Size() - 1];
				myCurrentSize--;
				objectDeleted = true;
			}
		}
	}

	template <typename Type, int MaxSize, typename CountType = unsigned short, bool UseSafeModeFlag = true>
	inline void VectorOnStack<Type, MaxSize, CountType, UseSafeModeFlag>::
		DeleteCyclicAtIndex(const CountType aItemNumber)
	{
		if (aItemNumber > Size() || aItemNumber < 0)
		{
			DL_ASSERT("Index out of bounds");
		}
		if (myCurrentSize == 0)
		{
			DL_ASSERT("Vector Empty. Remove out of bounds.");
		}
		SAFE_DELETE(myArray[aItemNumber])
			myArray[aItemNumber] = myArray[myCurrentSize - 1];
		myCurrentSize--;
	}

	template <typename Type, int MaxSize, typename CountType = unsigned short, bool UseSafeModeFlag = true>
	inline void VectorOnStack<Type, MaxSize, CountType, UseSafeModeFlag>::
		RemoveCyclic(const Type& aObject)
	{
		bool objectDeleted = false;
		CountType iterator = 0;
		while (objectDeleted == false)
		{
			if (iterator >= Size())
			{
				DL_ASSERT("Couldn't find data to remove.");
			}
			if (myArray[iterator] == aObject)
			{
				myArray[iterator] = myArray[Size() - 1];
				myCurrentSize--;
				objectDeleted = true;
			}
			else
			{
				iterator++;
			}
		}

	}

	template <typename Type, int MaxSize, typename CountType = unsigned short, bool UseSafeModeFlag = true>
	inline void VectorOnStack<Type, MaxSize, CountType, UseSafeModeFlag>::
		RemoveCyclicAtIndex(const CountType aItemNumber)
	{
		if (aItemNumber > Size() || aItemNumber < 0)
		{
			DL_ASSERT("Index out of bounds");
		}
		if (myCurrentSize == 0)
		{
			DL_ASSERT("Vector Empty. Remove out of bounds.");
		}
		myArray[aItemNumber] = myArray[Size() - 1];
		myCurrentSize--;
	}

	template <typename Type, int MaxSize, typename CountType = unsigned short, bool UseSafeModeFlag = true>
	inline void VectorOnStack<Type, MaxSize, CountType, UseSafeModeFlag>::
		RemoveAt(const CountType anIndex)
	{
		CopyDownUntilEnd(anIndex + 1);
		myCurrentSize--;
	}



	template <typename Type, int MaxSize, typename CountType = unsigned short, bool UseSafeModeFlag = true>
	inline void VectorOnStack<Type, MaxSize, CountType, UseSafeModeFlag>::
	CopyDownUntilEnd(const CountType aItemNumber)
	{
		for (CountType i = aItemNumber; i < myCurrentSize; i++)
		{
			myArray[i - 1] = myArray[i];
		}
	}

	template <typename Type, int MaxSize, typename CountType = unsigned short, bool UseSafeModeFlag = true>
	inline void VectorOnStack<Type, MaxSize, CountType, UseSafeModeFlag>::
		Clear()
	{
		myCurrentSize = 0;
	}

	template <typename Type, int MaxSize, typename CountType = unsigned short, bool UseSafeModeFlag = true>
	inline void VectorOnStack<Type, MaxSize, CountType, UseSafeModeFlag>::
		DeleteAll()
	{
		for (unsigned i = 0; i < Size(); i++)
		{
			SAFE_DELETE(myArray[i])
		}
		myCurrentSize = 0;
	}

	template <typename Type, int MaxSize, typename CountType = unsigned short, bool UseSafeModeFlag = true>
	inline CountType VectorOnStack<Type, MaxSize, CountType, UseSafeModeFlag>::
		Size() const
	{
		return myCurrentSize;
	}
}

namespace CU = CommonUtilities;
#pragma once

#include "Macros.h"
#include "DL_Debug.h"

#define GROWING_ARRAY_TEMPLATE template <typename Type, typename CountType = unsigned short>
#define GROWING_ARRAY_OBJECT GrowingArray<Type, CountType>

namespace CommonUtilities
{
	GROWING_ARRAY_TEMPLATE

	class GrowingArray
	{
	public:
		GrowingArray();
		GrowingArray(CountType aStartSize, bool aUseSafeFlagMode = true);
		GrowingArray(const GrowingArray& aGrowingArray);

		~GrowingArray();

		typedef Type* iterator;
		typedef const Type* const_iterator;
		iterator begin() { return &myArray[0]; }
		const_iterator begin() const { return &myArray[0]; }
		iterator end() { return &myArray[myCurrentSize]; }
		const_iterator end() const { return &myArray[myCurrentSize]; }

		GrowingArray& operator=(const GrowingArray& aGrowingArray);

		inline const Type& operator[](const CountType& aIndex) const;
		inline Type& operator[](const CountType& aIndex);

		void Init(CountType aStartSize, bool aUseSafeFlagMode = true);
		void ReInit(CountType aStartSize, bool aUseSafeFlagMode = true);

		inline void Add(const Type& aObject);
		inline void Add(const GrowingArray<Type, CountType>& someObjects);
		inline void Insert(const CountType aIndex, const Type& aObject);
		inline void DeleteCyclic(const Type& aObject);
		inline void DeleteCyclicAtIndex(const CountType aItemNumber);
		inline void RemoveCyclic(const Type& aObject);
		inline void RemoveCyclicAtIndex(const CountType aItemNumber);
		inline void RemoveAtIndex(const CountType aItemNumber);
		inline void DeleteAtIndex(const CountType aItemNumber);
		inline CountType Find(const Type& aObject);
		inline Type& GetLast();
		inline const Type& GetLast() const;

		inline void RemoveAll();
		inline void DeleteAll();
		inline void DeleteLast();
		inline void RemoveLast();
		bool IsInited() const;
		inline CountType GetFoundNone();
		static const CountType FoundNone = static_cast<CountType>(-1);
		void Optimize();

		__forceinline CountType Size() const;

	private:
		Type *myArray;
		CountType myCurrentSize;
		CountType myMaxSize;



		bool myUseSafeFlagMode;

		inline void IncreaseSize();
		inline void ChangeSize(CountType aNewSize);
		inline void CopyDownUntilEnd(const CountType aItemNumber);

		bool myInit;

	};

	GROWING_ARRAY_TEMPLATE
		bool GROWING_ARRAY_OBJECT::IsInited() const
	{
		return myInit;
	}

	GROWING_ARRAY_TEMPLATE
		GROWING_ARRAY_OBJECT::
		GrowingArray()
	{

		myInit = false;

		myMaxSize = 0;
		myCurrentSize = 0;
		myUseSafeFlagMode = true;
		myArray = nullptr;
	}

	GROWING_ARRAY_TEMPLATE
		void GROWING_ARRAY_OBJECT::Init(CountType aStartSize, bool aUseSafeFlagMode = true)
	{
		if (myInit == true)
		{
			ReInit(aStartSize, aUseSafeFlagMode);
			return;
		}
		myInit = true;

		myMaxSize = aStartSize;
		myCurrentSize = 0;
		myUseSafeFlagMode = aUseSafeFlagMode;
		myArray = new Type[myMaxSize];
	}

	GROWING_ARRAY_TEMPLATE
		void GROWING_ARRAY_OBJECT::ReInit(CountType aStartSize, bool aUseSafeFlagMode = true)
	{

		if (myInit == false)
		{
			DL_ASSERT("Growing array not initialized");
		}
		RemoveAll();
		SAFE_DELETE_ARRAY(myArray);
		ChangeSize(aStartSize);
		myUseSafeFlagMode = aUseSafeFlagMode;
	}

	GROWING_ARRAY_TEMPLATE
		GROWING_ARRAY_OBJECT::
		GrowingArray(CountType aStartSize, bool aUseSafeFlagMode)
	{
		myInit = true;

		myMaxSize = aStartSize;
		myCurrentSize = 0;
		myUseSafeFlagMode = aUseSafeFlagMode;
		myArray = new Type[myMaxSize];
	}

	GROWING_ARRAY_TEMPLATE
		GROWING_ARRAY_OBJECT::GrowingArray(const GROWING_ARRAY_OBJECT& aGrowingArray)
	{
		myMaxSize = 0;
		myCurrentSize = 0;
		myArray = nullptr;
		*this = aGrowingArray;
	}

	GROWING_ARRAY_TEMPLATE
		GROWING_ARRAY_OBJECT::~GrowingArray()
	{
		if (myInit == true)
		{
			RemoveAll();
		}
		SAFE_DELETE_ARRAY(myArray);
		myMaxSize = 0;
		myCurrentSize = 0;
	}

	GROWING_ARRAY_TEMPLATE
		GROWING_ARRAY_OBJECT&
		GROWING_ARRAY_OBJECT::operator=(const GROWING_ARRAY_OBJECT& aGrowingArray)
	{
		if (aGrowingArray.myInit == true)
		{
			if (myMaxSize < aGrowingArray.myMaxSize)
			{
				myMaxSize = aGrowingArray.myMaxSize;
				SAFE_DELETE_ARRAY(myArray);
				myArray = new Type[aGrowingArray.myMaxSize];
			}
			else
			{
				myInit = true;
				RemoveAll();
			}

			myInit = true;
			myUseSafeFlagMode = aGrowingArray.myUseSafeFlagMode;
			myCurrentSize = aGrowingArray.Size();

			if (myUseSafeFlagMode == true)
			{

				for (CountType i = 0; i < Size(); i++)
				{
					myArray[i] = aGrowingArray.myArray[i];
				}
			}
			else
			{
				memcpy(myArray, aGrowingArray.myArray, sizeof(Type) * aGrowingArray.Size());
			}
		}
		else
		{
			//	DL_ASSERT("Right value growing array not initialized.");
			myInit = false;
			myUseSafeFlagMode = aGrowingArray.myUseSafeFlagMode;
		}
		return *this;
	}

	GROWING_ARRAY_TEMPLATE
		inline const Type& GROWING_ARRAY_OBJECT::operator[](const CountType & aIndex) const
	{
		if (myInit == false)
		{
			DL_ASSERT("Growing array not initialized.");
		}
		if (aIndex > Size() && aIndex < 0)
		{
			DL_ASSERT("Index out of bounds");
		}
		return myArray[aIndex];
	}

	GROWING_ARRAY_TEMPLATE
		inline Type& GROWING_ARRAY_OBJECT::operator[](const CountType & aIndex)
		{
		if (myInit == false)
		{
			DL_ASSERT("Growing array not initialized.");
		}
		if (aIndex > Size() || aIndex < 0)
		{
			DL_ASSERT("Index out of bounds");
		}
		return myArray[aIndex];
	}

	GROWING_ARRAY_TEMPLATE
		inline void GROWING_ARRAY_OBJECT::Add(const Type& aObject)
	{
		if (myInit == false)
		{
			DL_ASSERT("Growing array not initialized.");
		}
		if (Size() >= myMaxSize)
		{
			IncreaseSize();
		}

		myArray[Size()] = aObject;

		myCurrentSize++;
	}

	GROWING_ARRAY_TEMPLATE
		inline void GROWING_ARRAY_OBJECT::Add(const GrowingArray<Type, CountType>& someObjects)
	{
		if (myInit == false)
		{
			DL_ASSERT("Growing array not initialized.");
		}
		if (someObjects.myInit == false)
		{
			DL_ASSERT("Rightvalue Growingarray not inintialized.");
		}

		for (CountType i = 0; i < someObjects.Size(); i++)
		{
			Add(someObjects[i]);
		}
	}

	GROWING_ARRAY_TEMPLATE
		inline void GROWING_ARRAY_OBJECT::Insert(const CountType aIndex, const Type& aObject)
	{
		if (myInit == false)
		{
			DL_ASSERT("Growing array not initialized.");
		}

		if (aIndex > Size() || aIndex < 0)
		{
			DL_ASSERT("Index out of bounds.");
		}

		if (Size() >= myMaxSize)
		{
			IncreaseSize();
		}

		for (int i = (Size()); i > aIndex; i--)
		{
			myArray[i] = myArray[i - 1];
		}
		myArray[aIndex] = aObject;
		myCurrentSize++;
	}

	GROWING_ARRAY_TEMPLATE
		inline void GROWING_ARRAY_OBJECT::DeleteCyclic(const Type& aObject)
	{
		if (myInit == false)
		{
			DL_ASSERT("Growing array not initialized.");
		}

		CountType indexOfItem = Find(aObject);
		assert(indexOfItem != FoundNone);

		if (indexOfItem == FoundNone)
		{
			DL_ASSERT("Didn't find item to delete in DeleteCyclic.");
		}

		DeleteCyclicAtIndex(indexOfItem);
	}

	GROWING_ARRAY_TEMPLATE
		inline void GROWING_ARRAY_OBJECT::DeleteCyclicAtIndex(const CountType aItemNumber)
	{
		if (myInit == false)
		{
			DL_ASSERT("Growing array not initialized.");
		}

		if (aItemNumber > myCurrentSize || aItemNumber < 0)
		{
			DL_ASSERT("Index out of bounds")
		}

		if (myCurrentSize == 0)
		{
			DL_ASSERT("Cant Delete from an empty growing array.");
		}

		SAFE_DELETE(myArray[aItemNumber]);
		myArray[aItemNumber] = myArray[myCurrentSize - 1];
		myCurrentSize--;
	}

	GROWING_ARRAY_TEMPLATE
		inline void GROWING_ARRAY_OBJECT::RemoveCyclic(const Type& aObject)
	{
		if (myInit == false)
		{
			DL_ASSERT("Growing array not initialized.");
		}

		CountType indexOfItem = Find(aObject);
		if (indexOfItem == FoundNone)
		{
			DL_ASSERT("Didn't find item to Remove in RemoveCyclic.");
		}
		RemoveCyclicAtIndex(indexOfItem);
	}

	GROWING_ARRAY_TEMPLATE
		inline void GROWING_ARRAY_OBJECT::RemoveCyclicAtIndex(const CountType aItemNumber)
	{
		if (myInit == false)
		{
			DL_ASSERT("Growing array not initialized.");
		}

		if (aItemNumber > myCurrentSize || aItemNumber < 0)
		{
			DL_ASSERT("Index out of bounds");
		}

		if (myCurrentSize == 0)
		{
			DL_ASSERT("Cant Remove from an empty growing array.");
		}

		myArray[aItemNumber] = myArray[Size() - 1];
		myArray[myCurrentSize - 1] = Type();
		myCurrentSize--;

	}

	GROWING_ARRAY_TEMPLATE
		inline void GROWING_ARRAY_OBJECT::RemoveAtIndex(const CountType aItemNumber)
	{
		CopyDownUntilEnd(aItemNumber + 1);
		myArray[myCurrentSize - 1] = Type();

		myCurrentSize--;
	}

	GROWING_ARRAY_TEMPLATE
		inline void GROWING_ARRAY_OBJECT::DeleteAtIndex(const CountType aItemNumber)
	{
		SAFE_DELETE(myArray[aItemNumber]);
		CopyDownUntilEnd(aItemNumber + 1);
		myCurrentSize--;

	}

	GROWING_ARRAY_TEMPLATE
		inline void GROWING_ARRAY_OBJECT::CopyDownUntilEnd(const CountType aItemNumber)
	{
		for (CountType i = aItemNumber; i < myCurrentSize; i++)
		{
			myArray[i - 1] = myArray[i];
		}
	}


	GROWING_ARRAY_TEMPLATE
		inline void GROWING_ARRAY_OBJECT::RemoveAll()
	{
		if (myInit == false)
		{
			DL_ASSERT("Growing array not initialized.");
		}

		for (CountType i = 0; i < myCurrentSize; i++)
		{
			myArray[i] = Type();
		}

		myCurrentSize = 0;
	}

	GROWING_ARRAY_TEMPLATE
		inline void GROWING_ARRAY_OBJECT::DeleteAll()
	{
		if (myInit == true)
		{
			for (unsigned i = 0; i < Size(); i++)
			{
				SAFE_DELETE(myArray[i])
			}
			myCurrentSize = 0;
		}
	}

	GROWING_ARRAY_TEMPLATE
		inline void GROWING_ARRAY_OBJECT::ChangeSize(CountType aNewSize)
	{
		if (myInit == false)
		{
			DL_ASSERT("Growing array not initialized.");
		}

		if (aNewSize < Size())
		{
			DL_ASSERT("Loss of data if resized to lower than current size.");
		}

		Type *newArray = new Type[aNewSize];
		if (myUseSafeFlagMode == true)
		{
			for (CountType i = 0; i < myCurrentSize; i++)
			{
				newArray[i] = myArray[i];
			}
		}
		else if (Size() > 0)
		{
			std::memcpy(newArray, myArray, sizeof(Type) * (myCurrentSize));
		}
		for (CountType i = 0; i < myCurrentSize; i++)
		{
			newArray[i] = myArray[i];
		}

		SAFE_DELETE_ARRAY(myArray);

		myArray = newArray;

		myMaxSize = aNewSize;
	}

	GROWING_ARRAY_TEMPLATE
		inline void GROWING_ARRAY_OBJECT::IncreaseSize()
	{
		if (myInit == false)
		{
			DL_ASSERT("Growing array not initialized.");
		}

		if (static_cast<CountType>(myMaxSize * static_cast<CountType>(2)) < myMaxSize)
		{
			DL_ASSERT("Growing array size increase wrapped! Please use a larger counttype!");
		}

		Type *newArray = new Type[myMaxSize * 2];
		if (myUseSafeFlagMode == true)
		{
			for (CountType i = 0; i < myMaxSize; i++)
			{
				newArray[i] = myArray[i];
			}
		}
		else
		{
			std::memcpy(newArray, myArray, sizeof(Type) * (myCurrentSize));
		}
		for (CountType i = 0; i < myCurrentSize; i++)
		{
			newArray[i] = myArray[i];
		}

		SAFE_DELETE_ARRAY(myArray);

		myArray = newArray;
		myMaxSize *= 2;
	}
	GROWING_ARRAY_TEMPLATE
		inline CountType GROWING_ARRAY_OBJECT::Find(const Type& aObject)
	{
		if (myInit == false)
		{
			DL_ASSERT("Growing array not initialized.");
		}
		for (CountType i = 0; i < Size(); i++)
		{
			if (myArray[i] == aObject)
			{
				return i;
			}
		}
		return FoundNone;
	}

	GROWING_ARRAY_TEMPLATE
		inline Type& GROWING_ARRAY_OBJECT::GetLast()
	{
		if (myInit == false)
		{
			DL_ASSERT("Growing array not initialized.");
		}

		if (Size() == 0)
		{
			DL_ASSERT("Growing array has no objects, can't get last");
		}
		return myArray[Size() - 1];
	}

	GROWING_ARRAY_TEMPLATE
		inline const Type& GROWING_ARRAY_OBJECT::GetLast() const
	{
		if (myInit == false)
		{
			DL_ASSERT("Growing array not initialized.");
		}
		return myArray[Size() - 1];
	}

	GROWING_ARRAY_TEMPLATE
		inline void GROWING_ARRAY_OBJECT::DeleteLast()
	{
		if (myInit == false)
		{
			DL_ASSERT("Growing array not initialized.");
		}
		if (Size() == 0)
		{
			DL_ASSERT("Growing array has no objects, can't delete last");
		}
		SAFE_DELETE(myArray[Size() - 1]);
		myCurrentSize--;
	}

	GROWING_ARRAY_TEMPLATE
		inline void GROWING_ARRAY_OBJECT::RemoveLast()
	{
		if (myInit == false)
		{
			DL_ASSERT("Growing array not initialized.");
		}
		if (Size() == 0)
		{
			DL_ASSERT("Growing array has no objects, can't remove last");
		}
		myArray[myCurrentSize - 1] = Type();
		myCurrentSize--;
	}

	GROWING_ARRAY_TEMPLATE
		void GROWING_ARRAY_OBJECT::Optimize()
	{
		if (myInit == false)
		{
			DL_ASSERT("Growing array not initialized.");
		}
		if (myMaxSize == myCurrentSize)
		{
			return;
		}
		ChangeSize(Size());
	}

	GROWING_ARRAY_TEMPLATE
		__forceinline CountType GROWING_ARRAY_OBJECT::Size() const
	{
		if (myInit == false)
		{
			return 0;
		}
		return myCurrentSize;
	}

	GROWING_ARRAY_TEMPLATE
		inline CountType GROWING_ARRAY_OBJECT::GetFoundNone()
	{
		return FoundNone;
	}


}

namespace CU = CommonUtilities;
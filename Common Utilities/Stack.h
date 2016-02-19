#pragma once

#include "GrowingArray.h"

#define STACK_TEMPLATE template <typename Type>
#define STACK_OBJECT Stack<Type>

namespace CommonUtilities
{

	STACK_TEMPLATE
	class Stack
	{
	public:
		Stack();
		Stack(unsigned short aSize);
		~Stack();

		Type Pop();
		const Type& Top();
		int	Size() const;
		void Push(const Type& aValue);
		inline void Clear();
	private:

		CU::GrowingArray<Type> myData;
		void SizeTest();

	};

	STACK_TEMPLATE
		void STACK_OBJECT::SizeTest()
	{
		if (myData.Size() == 0)
		{
			DL_ASSERT("Stack is empty!")
		}
	}

	STACK_TEMPLATE
		STACK_OBJECT::Stack()
	{
		myData.Init(4);
	}

	STACK_TEMPLATE
		STACK_OBJECT::Stack(unsigned short aSize) : myData(aSize)
	{

	}

	STACK_TEMPLATE
		STACK_OBJECT::~Stack()
	{

	}

	STACK_TEMPLATE
		Type STACK_OBJECT::Pop()
	{
		SizeTest();

		Type data = myData.GetLast();
		myData.RemoveLast();

		return data;
	}

	STACK_TEMPLATE
		const Type& STACK_OBJECT::Top()
	{
		SizeTest();
		return myData.GetLast();
	}

	STACK_TEMPLATE
		int	STACK_OBJECT::Size() const
	{
		return myData.Size();
	}

	STACK_TEMPLATE
		void STACK_OBJECT::Push(const Type& aData)
	{
		myData.Add(aData);
	}

	STACK_TEMPLATE
		void STACK_OBJECT::Clear()
	{
		myData.RemoveAll();
	}

}

namespace CU = CommonUtilities;

#undef STACK_TEMPLATE
#undef STACK_OBJECT
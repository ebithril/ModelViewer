#include "Common Utilities_Precompiled.h"
#include "StateStack.h"
#include "State.h"

namespace CommonUtilities
{
	StateStack::StateStack()
	{
		myMainStates.Init(4);
	}


	StateStack::~StateStack()
	{
		for (State* state : myMainStates)
		{
			state->PopAllChildren();
		}

		myMainStates.DeleteAll();
	}

	void StateStack::Render()
	{
		if (myMainStates.Size() != 0)
		{
			myMainStates.GetLast()->Render();
		}
	}

	void StateStack::Update()
	{
		if (myMainStates.Size() != 0)
		{
			myMainStates.GetLast()->Update();
		}

	}

	void StateStack::Pop()
	{
		if (myMainStates.Size() > 0)
		{
			myMainStates.GetLast()->Pop();
		}
	}

	void StateStack::Push(State* aState)
	{
		aState->SetStateStack(this);
		myMainStates.GetLast()->Push(aState);

	}

	void StateStack::PushMain(State* aState)
	{
		aState->SetStateStack(this);
		myMainStates.Add(aState);
	}

	void StateStack::PopMain()
	{
		myMainStates.GetLast()->PopAllChildren();
		SAFE_DELETE(myMainStates.GetLast());
		myMainStates.RemoveLast();
	}

	void StateStack::PopMainAndPush(State* aState)
	{
		PopMain();
		PushMain(aState);
	}

	unsigned short StateStack::GetMainStateCount()
	{
		return myMainStates.Size();
	}

}
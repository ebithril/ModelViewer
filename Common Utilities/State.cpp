#include "Common Utilities_Precompiled.h"
#include "State.h"

namespace CommonUtilities
{
	State::State()
	{
		myHandleEvents = false;
		myUpdateParent = false;
		myRenderParent = false;
		myParentState = nullptr;
		myChildState = nullptr;
	}

	State::~State()
	{
	}

	void State::Pop()
	{
		if (myChildState->myChildState == nullptr)
		{
			SAFE_DELETE(myChildState);
		}
		else
		{
			myChildState->Pop();
		}
	}

	void State::PopAllChildren()
	{
		if (myChildState != nullptr)
		{
			myChildState->PopAllChildren();
		}
		SAFE_DELETE(myChildState);
	}

	void State::Push(State* aState)
	{
		if (myChildState == nullptr)
		{
			myChildState = aState;
			myChildState->myParentState = this;
		}
		else
		{
			myChildState->Push(aState);
		}
	}

	void State::SetStateStack(StateStack* aStateStack)
	{
		myStateStack = aStateStack;
	}

	void State::SetHandleEvents(bool aFlag)
	{
		aFlag;
	}

	bool  State::UpdateUntilTop()
	{
		if (StateUpdateFunction() == false)
		{
			return false;
		}
		if (myChildState != nullptr)
		{
			myChildState->UpdateUntilTop();
		}

		return true;
	}

	bool State::RenderUntilTop()
	{
		if (StateRenderFunction() == false)
		{
			return false;
		}

		if (myChildState != nullptr)
		{
			if (myChildState->RenderUntilTop() == false)
			{
				return false;
			}
		}

		return true;
	}

	bool State::UpdateUntilLowest()
	{
		if (myParentState != nullptr && myUpdateParent == true)
		{
			if (myParentState->UpdateUntilLowest() == false)
			{
				return false;
			}
		}
		else
		{
			if (UpdateUntilTop() == false)
			{
				return false;
			}
		}

		return true;
	}

	bool State::RenderUntilLowest()
	{
		if (myParentState != nullptr && myRenderParent == true)
		{
			if (myParentState->RenderUntilLowest() == false)
			{
				return false;
			}
		}
		else
		{
			if (RenderUntilTop() == false)
			{
				return false;
			}
		}

		return true;
	}

	bool State::Update()
	{
		if (myChildState == nullptr)
		{
			if (UpdateUntilLowest() == false)
			{
				return false;
			}
		}
		else
		{
			if(myChildState->Update() == false)
			{
				return false;
			}
		}

		return true;
	}

	bool State::Render()
	{
		if (myChildState == nullptr)
		{
			if (RenderUntilLowest() == false)
			{
				return false;
			}
		}
		else
		{
			if (myChildState->Render() == false)
			{
				return false;
			}
		}
		return true;
	}
}
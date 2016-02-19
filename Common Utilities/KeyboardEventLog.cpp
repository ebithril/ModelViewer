#include "Common Utilities_Precompiled.h"
#include "KeyboardEventLog.h"

namespace CommonUtilities
{
	void KeyboardEventLog::AddKeyboardEvent(const KeyboardEvent& aKeyboardEvent)
	{
		for (unsigned short i = 0; i < myKeyboardEvents.Size(); i++)
		{
			if (aKeyboardEvent == myKeyboardEvents[i])
			{
				return;
			}
		}
		myKeyboardEvents.Add(aKeyboardEvent);
	}
	
	void KeyboardEventLog::ClearLog()
	{
		myKeyboardEvents.RemoveAll();
	}

	const CU::GrowingArray<KeyboardEvent>& KeyboardEventLog::GetLog()const
	{
		return myKeyboardEvents;
	}


	KeyboardEventLog::KeyboardEventLog() 
	{
		myKeyboardEvents.Init(16);
	}

	KeyboardEventLog::~KeyboardEventLog()
	{

	}

}
#pragma once
#include "EnumKeys.h"

namespace CommonUtilities
{
	struct KeyboardEvent
	{
		inline KeyboardEvent();

		inline KeyboardEvent(const eKeyboardKeys& aKeyboardKey, const eKeyState& aKeystate);

		eKeyboardKeys myKey;
		eKeyState myState;

		inline bool operator==(const KeyboardEvent& aKeyboardEvent) const;
	};

	bool KeyboardEvent::operator==(const KeyboardEvent& aKeyboardEvent) const
	{
		if (myKey == aKeyboardEvent.myKey && myState == aKeyboardEvent.myState)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	KeyboardEvent::KeyboardEvent()
	{

	}

	KeyboardEvent::KeyboardEvent(const eKeyboardKeys& aKeyboardKey, const eKeyState& aKeystate)
	{
		myKey = aKeyboardKey;
		myState = aKeystate;
	}
}

namespace CU = CommonUtilities;


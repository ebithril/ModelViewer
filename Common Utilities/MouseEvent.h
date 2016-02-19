#pragma once

#include "EnumKeys.h"

namespace CommonUtilities
{
	struct MouseEvent
	{
		eMouseButtons myButton;
		eKeyState myState;

		inline MouseEvent();
		inline MouseEvent(const eMouseButtons aMouseButton, const eKeyState aKeyState);
		inline bool operator==(const MouseEvent& aMouseEvent);
	};

	MouseEvent::MouseEvent(const eMouseButtons aMouseButton, const eKeyState aKeyState)
	{
		myButton = aMouseButton;
		myState = aKeyState;
	}

	MouseEvent::MouseEvent()
	{

	}

	bool MouseEvent::operator==(const MouseEvent& aMouseEvent)
	{
		if (myButton == aMouseEvent.myButton && myState == aMouseEvent.myState)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

}

namespace CU = CommonUtilities;
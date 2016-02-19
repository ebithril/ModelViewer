#include "Common Utilities_Precompiled.h"
#include "Event.h"

namespace CommonUtilities
{
	Event::Event()
	{
	}

	Event::Event(const eEvent anEvent)
	{
		myEventType = anEvent;
	}

	Event::~Event()
	{
	}

	eEvent Event::GetMyType()const
	{
		return myEventType;
	}

	void Event::SetMyType(const eEvent anEventType)
	{
		myEventType = anEventType;
	}

}


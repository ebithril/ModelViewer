#pragma once

#include "GrowingArray.h"
#include "Observer.h"
#include "Event.h"
#include "ObserverPrio.h"

namespace CommonUtilities
{
	struct ObserverEvent
	{
	public:
		eEvent myEventType;
		CU::GrowingArray<ObserverPrio> myObservers;
	};
}

namespace CU = CommonUtilities;
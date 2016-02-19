#pragma once
#include "GrowingArray.h"
#include "EventPrio.h"

namespace CommonUtilities
{
	class Observer;
	struct DetachObserverData
	{
		Observer* myObserver;
		EventPrio myAttachEvents;
	};
}

namespace CU = CommonUtilities;

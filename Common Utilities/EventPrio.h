#pragma once
#include "EventEnums.h"	

namespace CommonUtilities
{
	struct EventPrio
	{
		CU::eEvent myEventType;
		int myPriority;
	};
}

namespace CU = CommonUtilities;
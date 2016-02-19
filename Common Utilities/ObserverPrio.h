#pragma once
#include "Observer.h"
#include "Event.h"
#include "PoolPointer.h"
#include <functional>

namespace CommonUtilities
{
	struct ObserverPrio
	{
		inline ObserverPrio();
		CU::Observer* myObserver;
		std::function<bool(const CU::PoolPointer<CU::Event>)> myHandleEventFunction;
		int myPrio;
		bool myIsActive;
	};

	ObserverPrio::ObserverPrio()
	{
		myPrio = 0;
		myIsActive = false;
	}
}
namespace CU = CommonUtilities;

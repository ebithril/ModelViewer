#pragma once
#include "PoolPointer.h"


namespace CommonUtilities
{
	class Observer;
	class Event;
	enum class eEvent;

	struct AttachObserverData
	{
		inline AttachObserverData();
		Observer* myObserver;
		std::function<bool(const CU::PoolPointer<CU::Event>)> myHandleEventFunction;
		eEvent myEventType;
		int aPriority;
	};

	AttachObserverData::AttachObserverData()
	{
		aPriority = 0;
	}
}
namespace CU = CommonUtilities;


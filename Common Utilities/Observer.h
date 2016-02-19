#pragma once

#include "Event.h"
#include "EventPrio.h"
#include <functional>
#include "PoolPointer.h"
#include "GrowingArray.h"

namespace CommonUtilities
{
#define HANDLE_EVENT_FUNCTION(Function) [&](const CU::PoolPointer<CU::Event> anEvent)->bool{return Function(anEvent);} 

	class Observer
	{
	public:
		Observer();
		~Observer();

		void ObserveEvent(const eEvent& anEvent, const std::function<bool(const CU::PoolPointer<CU::Event>)>& anHandleEventFunction, const int aPriority = MAXINT32);
		Observer& operator=(const Observer& anObserver);

		const CU::GrowingArray<EventPrio>& GetMyAttachEvents()const;

	protected:

		void DetachMe();
		
	private:
		CU::GrowingArray<EventPrio> myAttachEvents;	
	};
}

namespace CU = CommonUtilities;


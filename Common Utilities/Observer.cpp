#include "Common Utilities_Precompiled.h"
#include "Observer.h"
#include "EventManager.h"
#include "AttachObserverData.h"

namespace CommonUtilities
{
	Observer::Observer()
	{
		myAttachEvents.Init(4);
	}

	Observer::~Observer()
	{
		DetachMe();
	}

	Observer& Observer::operator=(const Observer& anObserver)
	{
		anObserver;
		DetachMe();
		return *this;
	}

	void Observer::DetachMe()
	{
		EventManager::GetInstance()->DetachObserver(*this);
		myAttachEvents.RemoveAll();
	}

	void Observer::ObserveEvent(const eEvent& anEvent, const std::function<bool(const CU::PoolPointer<CU::Event>)>& anHandleEventFunction, const int aPriority)
	{
		AttachObserverData data;

		data.myObserver = this;
		data.myEventType = anEvent;
		data.myHandleEventFunction = anHandleEventFunction;
		data.aPriority = aPriority;

		EventManager::GetInstance()->AttachObserver(data);
		EventPrio anEventPrio;
		anEventPrio.myEventType = anEvent;
		anEventPrio.myPriority = aPriority;

		myAttachEvents.Add(anEventPrio);
	}

	const CU::GrowingArray<EventPrio>& Observer::GetMyAttachEvents()const
	{
		return myAttachEvents;
	}
}
namespace CU = CommonUtilities;
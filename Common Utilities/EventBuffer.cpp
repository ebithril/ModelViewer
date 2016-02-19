#include "Common Utilities_Precompiled.h"
#include "EventBuffer.h"

namespace CommonUtilities
{

	EventBuffer::EventBuffer()
	{
		
	}

	const CU::GrowingArray<PoolPointer<Event>>& EventBuffer::GetMyEvents() const
	{
		return myEvents;
	}

	EventBuffer::~EventBuffer()
	{
		myEvents.RemoveAll();
	}

	void EventBuffer::Init()
	{
		myEvents.Init(4);
		myEventPool.Init();
		myInternalEventPool.Init();
	}

	void EventBuffer::AddEvent(const PoolPointer<Event>& anEvent)
	{
		myEvents.Add(anEvent);
	}

	void EventBuffer::AddEvent(const CU::GrowingArray<PoolPointer<Event>>& anEvent)
	{
		for (unsigned short i = 0; i < anEvent.Size(); i++)
		{
			myEvents.Add(anEvent[i]);
		}
	}

	void EventBuffer::ClearBuffer()
	{
		myEvents.RemoveAll();
	}

}

namespace CU = CommonUtilities;
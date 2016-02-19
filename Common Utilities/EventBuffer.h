#pragma once

#include "GrowingArray.h"
#include "Event.h"
#include "PoolPointer.h"
#include "ObjectPool.h"


namespace CommonUtilities
{

	class EventBuffer
	{
	public:
		EventBuffer();
		~EventBuffer();

		void Init();

		void AddEvent(const PoolPointer<Event>& anEvent);
		void AddEvent(const CU::GrowingArray<PoolPointer<Event>>& anEvent);
		void ClearBuffer();

		const CU::GrowingArray<PoolPointer<Event>>& GetMyEvents() const;

		template <typename Type>
		CU::PoolPointer<Event> CreateInternalEvent(const Type& anEvent);

		template <typename Type>
		CU::PoolPointer<Event> CreatePoolEvent(const Type& anEvent);
	private:

		CU::GrowingArray<PoolPointer<Event>> myEvents;
		CU::ObjectPool<16777216> myEventPool;
		CU::ObjectPool<16777216> myInternalEventPool;


	};

	template <typename Type>
	CU::PoolPointer<Event> EventBuffer::CreateInternalEvent(const Type& anEvent)
	{
		return myInternalEventPool.Allocate<Type, Event>(anEvent);
	}

	template <typename Type>
	CU::PoolPointer<Event> EventBuffer::CreatePoolEvent(const Type& anEvent)
	{
		return myEventPool.Allocate<Type, Event>(anEvent);
	}
}

namespace CU = CommonUtilities;
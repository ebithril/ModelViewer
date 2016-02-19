#pragma once
#include "EventEnums.h"


namespace CommonUtilities
{
#define GET_EVENT_DATA(Event,DataType, VariableName) const DataType* VariableName = reinterpret_cast<const DataType*>(&*Event);
#define GET_NON_CONST_EVENT_DATA(Event,DataType, VariableName) DataType* VariableName = reinterpret_cast<DataType*>(&*Event);
	class Event
	{
	public:
		Event();
		Event(const eEvent anEvent);
		virtual ~Event();

		eEvent GetMyType() const;
		void SetMyType(const eEvent anEventType);

	protected:

		eEvent myEventType;
	};
}

namespace CU = CommonUtilities;

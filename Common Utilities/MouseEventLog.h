#pragma once
#include <vector>
#include "MouseAxisLog.h"
#include "GrowingArray.h"
#include "MouseEvent.h"

namespace CommonUtilities
{
	class MouseEventLog
	{
	public:

		MouseEventLog();
		~MouseEventLog();

		void AddMouseEvent(const MouseEvent& aMouseEvent);
		void AddMouseEvent(const MouseAxisLog& aMouseAxisEvent);
		void ClearLog();
		const CU::GrowingArray<MouseEvent>& GetButtonLog()const;
		const CU::GrowingArray<MouseAxisLog>& GetAxisLog()const;

	private:											 

		CU::GrowingArray<MouseEvent> myMouseEvents;
		CU::GrowingArray<MouseAxisLog> myAxisLogs;
	};
}

namespace CU = CommonUtilities;
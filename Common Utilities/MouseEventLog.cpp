#include "Common Utilities_Precompiled.h"
#include "MouseEventLog.h"

const int locVectorStartSize = 10;

namespace CommonUtilities
{
	MouseEventLog::MouseEventLog() 
	{
		myAxisLogs.Init(4);
		myMouseEvents.Init(4);
	}

	void MouseEventLog::AddMouseEvent(const MouseEvent& aMouseEvent)
	{
		for (unsigned short i = 0; i < myMouseEvents.Size(); i++)
		{
			if (myMouseEvents[i] == aMouseEvent)
			{
				return;
			}
		}
		myMouseEvents.Add(aMouseEvent);
	}

	void MouseEventLog::AddMouseEvent(const MouseAxisLog& aMouseAxisEvent)
	{
		for (unsigned short i = 0; i < myAxisLogs.Size(); i++)
		{
			if (myAxisLogs[i] == aMouseAxisEvent)
			{
				return;
			}
		}
		myAxisLogs.Add(aMouseAxisEvent);
	}

	void MouseEventLog::ClearLog()
	{
		myMouseEvents.RemoveAll();
		myAxisLogs.RemoveAll();
	}

	const CU::GrowingArray<MouseEvent>& MouseEventLog::GetButtonLog()const
	{
		return myMouseEvents;
	}

	const CU::GrowingArray<MouseAxisLog>& MouseEventLog::GetAxisLog()const
	{
		return myAxisLogs;
	}

	MouseEventLog::~MouseEventLog()
	{
	}
}
#pragma once
#include "EnumKeys.h"

namespace CommonUtilities
{
	struct MouseAxisLog
	{
	public:
		inline MouseAxisLog();
		inline MouseAxisLog(eMouseAxis aAxis, long aValue);
		inline bool operator==(const MouseAxisLog& anAxisLog);
		long myValue;
		eMouseAxis myAxis;
	};

	MouseAxisLog::MouseAxisLog()
	{

	}

	MouseAxisLog::MouseAxisLog(eMouseAxis aAxis, long aValue)
	{
		myValue = aValue;
		myAxis = aAxis;
	}

	bool MouseAxisLog::operator==(const MouseAxisLog& anAxisLog)
	{
		if (myAxis == anAxisLog.myAxis && myValue == anAxisLog.myValue)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

namespace CU = CommonUtilities;
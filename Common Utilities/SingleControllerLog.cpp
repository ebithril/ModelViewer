#include "Common Utilities_Precompiled.h"
#include "SingleControllerLog.h"

namespace CommonUtilities
{
	SingleControllerLog::SingleControllerLog(unsigned short aControllerNumber) :
		myAnalogueLogs(10),
		myButtons(10),
		myControllerNumber(aControllerNumber)
	{
		myIsConnected = false;
	}

	SingleControllerLog::~SingleControllerLog()
	{
	}

	void SingleControllerLog::Clear()
	{
		myButtons.clear();
		myAnalogueLogs.clear();
	}

}
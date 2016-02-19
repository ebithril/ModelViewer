#pragma once
#include <vector>
#include "AnalogueLog.h"
#include "ControllerButton.h"

#define _GROWINGARRAY std::vector
namespace CommonUtilities
{
	struct SingleControllerLog
	{
	public:
		SingleControllerLog(unsigned short aControllerNumber = 0);
		~SingleControllerLog();
		void Clear();

		bool myIsConnected;
		unsigned short myControllerNumber;
		_GROWINGARRAY<CU::AnalogueLog> myAnalogueLogs;
		_GROWINGARRAY<ControllerButton> myButtons;
	
	};
}
namespace CU = CommonUtilities;
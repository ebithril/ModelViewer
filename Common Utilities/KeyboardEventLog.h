#pragma once
#include "GrowingArray.h"
#include "KeyboardEvent.h"

namespace CommonUtilities
{
	

	class KeyboardEventLog
	{
	public:

		void AddKeyboardEvent(const KeyboardEvent& aKeyboardEvent);
		void ClearLog();
		const CU::GrowingArray<KeyboardEvent>& GetLog()const;

		KeyboardEventLog();
		~KeyboardEventLog();

	private:

		CU::GrowingArray<KeyboardEvent> myKeyboardEvents;
	

	};
}

namespace CU = CommonUtilities;
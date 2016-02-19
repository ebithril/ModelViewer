#include "Common Utilities_Precompiled.h"
#include "CharInput.h"
#include "EnumKeys.h"
#include <Windows.h>


namespace CommonUtilities
{
	char GetCharFromKey(const eKeyboardKeys aKey)
	{
		static HKL keyboardLayout = GetKeyboardLayout(0);
		static BYTE currentKeyboardState[256];
		WORD character[4];

		GetKeyboardState(currentKeyboardState);

		UINT virtualKey = MapVirtualKeyExA(aKey, MAPVK_VSC_TO_VK, keyboardLayout);
		int result = ToAsciiEx(virtualKey, aKey, currentKeyboardState, &character[0], 0, keyboardLayout);

		if (result > 0)
		{
			return static_cast<char>(character[0]);
		}

		switch (aKey)
		{
		case eKeyboardKeys::KEY_NUMPAD0:
			return '0';
			break;

		case eKeyboardKeys::KEY_NUMPAD1:
			return '1';
			break;

		case eKeyboardKeys::KEY_NUMPAD2:
			return '2';
			break;

		case eKeyboardKeys::KEY_NUMPAD3:
			return '3';
			break;

		case eKeyboardKeys::KEY_NUMPAD4:
			return '4';
			break;

		case eKeyboardKeys::KEY_NUMPAD5:
			return '5';
			break;

		case eKeyboardKeys::KEY_NUMPAD6:
			return '6';
			break;

		case eKeyboardKeys::KEY_NUMPAD7:
			return '7';
			break;

		case eKeyboardKeys::KEY_NUMPAD8:
			return '8';
			break;

		case eKeyboardKeys::KEY_NUMPAD9:
			return '9';
			break;

		case eKeyboardKeys::KEY_DIVIDE:
			return '/';
			break;

		case eKeyboardKeys::KEY_DECIMAL:
			return ',';
			break;

		default:
			break;
		}


		return -1;
	}
}

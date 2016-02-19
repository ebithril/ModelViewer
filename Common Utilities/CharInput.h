#pragma once

enum eKeyboardKeys : unsigned char;

namespace CommonUtilities
{
	//Returns -1 if there is no char for the key.
	char GetCharFromKey(const eKeyboardKeys aKey);
}

namespace CU = CommonUtilities;
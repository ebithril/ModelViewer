#pragma once
#include "EnumKeys.h"
#include "Vector.h"

namespace CommonUtilities
{
	struct AnalogueLog
	{
	public:

		AnalogueLog();
		~AnalogueLog();

		Vector2f myNormalizedValue;
		eXboxAnalogue myAnalogueType;

	};
}
namespace CU = CommonUtilities;
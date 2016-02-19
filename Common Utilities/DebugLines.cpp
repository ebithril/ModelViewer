#include "Common Utilities_Precompiled.h"
#include "DebugLines.h"

namespace CommonUtilities
{
	DebugLines::DebugLines()
	{
		myLines.Init(4);
	}

	DebugLines::DebugLines(const CU::GrowingArray<Line<float>> someLines,
		const Vector3<float> aColor)
	{
		myLines = someLines;
		myColor = aColor;
	}
}
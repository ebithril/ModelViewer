#pragma once
#include "Line.h"
#include "GrowingArray.h"

namespace CommonUtilities
{

	struct DebugLines
	{
	public:
		DebugLines();
		DebugLines(const CU::GrowingArray<Line<float>> someLines, 
			const Vector3<float> aColor = Vector3<float>(256,256,128));

		CU::GrowingArray<Line<float>> myLines;
		Vector3<float> myColor;
	};

}

namespace CU = CommonUtilities;
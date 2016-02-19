#pragma once
#include "Vector3.h"

namespace CommonUtilities
{
	struct CollisionStruct
	{
		Matrix44<float> myObjectSpace;
		float myObjectScale;
		Matrix44<float> myArgumentObjectSpace;
		float myArgumentObjectScale;
	};

}
namespace CU = CommonUtilities;
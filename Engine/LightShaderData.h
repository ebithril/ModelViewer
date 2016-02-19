#pragma once
#include "../Common Utilities/Vector.h"


namespace GraphicsEngine
{
	struct LightShaderData
	{
		Vector3f myColor;
		float myRange;
		Vector3f myDirection;
		int myLightType;
		Vector3f myPosition;
		float myAngle;
		float myPadding1;
		float myPadding2;
		float myPadding3;
		float myPadding4;

	};
}

namespace GE = GraphicsEngine;

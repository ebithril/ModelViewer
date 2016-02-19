#pragma once
#include "Vector.h"

namespace CommonUtilities
{
	struct RenderCommandLine
	{
	public:
		RenderCommandLine();
		~RenderCommandLine();

		RenderCommandLine(const Vector4f& afirstPosition, const Vector4f& aSecondPosition);
		RenderCommandLine(const Vector3f& afirstPosition, const Vector3f& aSecondPosition);
		Vector4f myFirstPoint;
		Vector4f mySecondPoint;
	};
}

namespace CU = CommonUtilities;
#include "Common Utilities_Precompiled.h"
#include "RenderCommandLine.h"

namespace CommonUtilities
{
	RenderCommandLine::RenderCommandLine()
	{
	}

	RenderCommandLine::RenderCommandLine(const Vector4f& afirstPosition, const Vector4f& aSecondPosition)
	{
		myFirstPoint = afirstPosition;
		mySecondPoint = aSecondPosition;
	}

	RenderCommandLine::RenderCommandLine(const Vector3f& afirstPosition, const Vector3f& aSecondPosition)
	{
		myFirstPoint = Vector4f(afirstPosition.x, afirstPosition.y, afirstPosition.z, 1.f);
		mySecondPoint = Vector4f(aSecondPosition.x, aSecondPosition.y, aSecondPosition.z, 1.f);
	}

	RenderCommandLine::~RenderCommandLine()
	{
	}
}
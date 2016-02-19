#pragma once
#include "Instance.h"

namespace GraphicsEngine
{
	enum class eEffectType
	{
		NORMAL,
		TERRAIN,
		ONE_BIT_ALPHA,
	};

	struct ShadowRenderData
	{
		GraphicsEngine::Instance* myInstance;
		Matrix44f myOrientation;
		eEffectType myEffectType;
	};
}

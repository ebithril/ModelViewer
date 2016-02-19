#pragma once

#include "COMObjectPointer.h"
#include "Texture.h"

namespace GraphicsEngine
{
	struct RenderTargetData
	{
		Texture myRenderTarget;
		Texture myDepthStencil;
	};
}

namespace GE = GraphicsEngine;
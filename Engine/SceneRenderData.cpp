#include "Engine_Precompiled.h"
#include "SceneRenderData.h"
#include "StreakEmitter.h"
#include "ParticleSystem.h"

namespace GraphicsEngine
{
	SceneRenderData::SceneRenderData()
	{
		myPostProcessOperations = ePostProcess::HDR;
	}

	SceneRenderData::~SceneRenderData()
	{

	}
}


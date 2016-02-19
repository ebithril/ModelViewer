#pragma once
#include "Camera.h"

#include "ParticleSystem.h"

namespace GraphicsEngine
{
	enum ePostProcess : unsigned short
	{
		NO_EFFECT = 1,
		HDR = 2,
		BLUR = 4,
		MOTION_BLUR = 8,
		BLOOM = 16,
	};

	inline ePostProcess operator|(ePostProcess a, ePostProcess b)
	{
		return static_cast<ePostProcess>(static_cast<unsigned short>(a) | static_cast<unsigned short>(b));
	}

	class ParticleSystem;
	class StreakEmitter;
	class LightBuffer;

	struct SceneRenderData
	{
		SceneRenderData();
		~SceneRenderData();

		Camera myCamera;
		
		LightBuffer* myLightBuffer;
		CU::GrowingArray<CU::PoolPointer<ParticleSystem>> myParticleSystems;
		CU::GrowingArray<CU::PoolPointer<GE::StreakEmitter>> myStreakEmitters;
		CU::ID myID;
		ePostProcess myPostProcessOperations;
	};
}
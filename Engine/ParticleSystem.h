#pragma once

#include "ParticleEmitter.h"
#include "../Common Utilities/Matrix44.h"
#include "../Common Utilities/GrowingArray.h"
#include "../Common Utilities/PoolPointer.h"

template <typename T>
class SharedPointer;

namespace GraphicsEngine
{
	class Camera;
	class ParticleSystemData;

	struct EmitterSpawnData
	{
		CU::PoolPointer<ParticleEmitter> myEmitter;
		Matrix44<float> myOrientation;
		float myTime;
	};

	class ParticleSystem
	{
	public:
		ParticleSystem();
		ParticleSystem(SharedPointer<ParticleSystemData> someParticleSystemData);
		~ParticleSystem();

		void Update();
		void Render(const Camera& aCamera);

		bool IsDone();

		void SetOrientation(const Matrix44<float>& aOrientation, bool aShouldUpdateEmitterOrientation = true);

		void Init(SharedPointer<ParticleSystemData> someParticleSystemData);
	private:
		CU::GrowingArray<EmitterSpawnData> myEmitters;
		Matrix44<float> myOrientation;
		float myTime;
	};
}

namespace GE = GraphicsEngine;
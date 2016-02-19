#pragma once
#include <string>
#include "../Common Utilities/Vector.h"
#include "COMObjectPointer.h"

struct ID3D11InputLayout;

namespace GraphicsEngine
{
	class Effect;
	struct ParticleData;
	class Texture;
	class EmitterFactory;

	class EmitterData
	{
		friend class EmitterFactory;
		friend class ParticleEmitter;
	public:
		EmitterData();
		~EmitterData();

		void LoadFile(const std::string& aFileName);

	private:
		void Load(const std::string& aFileName);

		Texture* myTexture;
		Effect* myEffect;
		ParticleData* myParticleData;
		COMObjectPointer<ID3D11InputLayout> myParticleInputLayout;
		Vector3f myEmissionExtents;
		Vector3f myEmissionVelocityDelta;
		float myEmissionLifeTime;
		float myEmissionRatePerSecond;
		unsigned short myMaxNumberOfParticles;
	};
}



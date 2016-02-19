#pragma once
#include "../Common Utilities/Vector3.h"

struct ID3D11InputLayout;

namespace GraphicsEngine
{
	class Effect;
	struct StreakData;
	class Texture;
	class EmitterFactory;

	class StreakEmitterData
	{
		friend class StreakEmitter;
		friend class StreakFactory;

	public:
		StreakEmitterData();
		~StreakEmitterData();

	private:
		Texture* myTexture;
		Effect* myEffect;
		StreakData* myStreakData;
		COMObjectPointer<ID3D11InputLayout> myStreakInputLayout;
		Vector3<float> myEmissionExtents;
		Vector3<float> myEmissionVelocityDelta;
		float myEmissionLifeTime;
		float myEmissionRatePerSecond;
		unsigned short myMaxNumberOfStreaks;

	};

}

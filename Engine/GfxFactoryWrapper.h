#pragma once

#include "COMObjectPointer.h"

struct ID3D11Device;

namespace GraphicsEngine
{
	class EffectFactory;
	class FBXFactory;
	class ModelFactory;
	class TextureFactory;
	class EmitterFactory;
	class StreakFactory;
	class ParticleSystemFactory;
	class DecalFactory;
	class SpriteFactory;

	class GfxFactoryWrapper
	{
	public:
		static void Create();
		static void Destroy();
		static GfxFactoryWrapper* GetInstance();

		void Init(COMObjectPointer<ID3D11Device>& aDevice);

		ModelFactory* GetModelFactory();
		TextureFactory* GetTextureFactory();
		EffectFactory* GetEffectFactory();
		EmitterFactory* GetEmitterFactory();
		StreakFactory* GetStreakFactory();
		ParticleSystemFactory* GetParticleSystemFactory();
		DecalFactory* GetDecalFactory();
		SpriteFactory* GetSpriteFactory();

	private:
		GfxFactoryWrapper();
		~GfxFactoryWrapper();
		static GfxFactoryWrapper* ourInstance;
		ModelFactory* myModelFactory;
		TextureFactory* myTextureFactory;
		EffectFactory* myEffectFactory;
		EmitterFactory* myEmitterFactory;
		StreakFactory* myStreakFactory;
		ParticleSystemFactory* myParticleSystemFactory;
		DecalFactory* myDecalFactory;
		SpriteFactory* mySpriteFactory;
	};
}


#include "Engine_Precompiled.h"
#include "GfxFactoryWrapper.h"
#include "ModelFactory.h"
#include "TextureFactory.h"
#include "EffectFactory.h"
#include "EmitterFactory.h"
#include "StreakFactory.h"
#include "ParticleSystemFactory.h"
#include "SpriteFactory.h"

namespace GraphicsEngine
{
	GfxFactoryWrapper* GfxFactoryWrapper::ourInstance = nullptr;
	

	void GfxFactoryWrapper::Create()
	{
		if (ourInstance == nullptr)
		{
			ourInstance = new GfxFactoryWrapper;
			ourInstance->myModelFactory = new ModelFactory;
			ourInstance->myTextureFactory = new TextureFactory;
			ourInstance->myEffectFactory = new EffectFactory;
			ourInstance->myEmitterFactory = new EmitterFactory;
			ourInstance->myStreakFactory = new StreakFactory;
			ourInstance->myParticleSystemFactory = new ParticleSystemFactory();
			ourInstance->mySpriteFactory = new SpriteFactory();
		}
	}

	void GfxFactoryWrapper::Destroy()
	{
		if (ourInstance != nullptr)
		{
			SAFE_DELETE(ourInstance);
		}
	}

	GfxFactoryWrapper* GfxFactoryWrapper::GetInstance()
	{
		return ourInstance;
	}
	
	void GfxFactoryWrapper::Init(COMObjectPointer<ID3D11Device>& aDevice)
	{
		myEffectFactory->Init(aDevice);
		myTextureFactory->Init(aDevice);
		myModelFactory->Init(aDevice);
		myEmitterFactory->Init();
		myStreakFactory->Init();
		myParticleSystemFactory->LoadAll();
		mySpriteFactory->Init();
	}

	ModelFactory* GfxFactoryWrapper::GetModelFactory()
	{
		return myModelFactory;
	}

	TextureFactory* GfxFactoryWrapper::GetTextureFactory()
	{
		return myTextureFactory;
	}

	EffectFactory* GfxFactoryWrapper::GetEffectFactory()
	{
		return myEffectFactory;
	}

	EmitterFactory* GfxFactoryWrapper::GetEmitterFactory()
	{
		return myEmitterFactory;
	}

	GfxFactoryWrapper::GfxFactoryWrapper()
	{
	}
	
	GfxFactoryWrapper::~GfxFactoryWrapper()
	{
		SAFE_DELETE(ourInstance->myModelFactory);
		SAFE_DELETE(ourInstance->myEmitterFactory);
		SAFE_DELETE(ourInstance->myTextureFactory);
		SAFE_DELETE(ourInstance->myEffectFactory);
		SAFE_DELETE(ourInstance->myStreakFactory);
		SAFE_DELETE(ourInstance->mySpriteFactory);
	}

	StreakFactory* GfxFactoryWrapper::GetStreakFactory()
	{
		return myStreakFactory;
	}

	DecalFactory* GfxFactoryWrapper::GetDecalFactory()
	{
		return myDecalFactory;
	}

	ParticleSystemFactory* GfxFactoryWrapper::GetParticleSystemFactory()
	{
		return myParticleSystemFactory;
	}

	SpriteFactory* GfxFactoryWrapper::GetSpriteFactory()
	{
		return mySpriteFactory;
	}
}

#include "Engine_Precompiled.h"
#include "EmitterFactory.h"
#include "EmitterData.h"
#include "GfxFactoryWrapper.h"
#include "ParticleData.h"
#include "TextureFactory.h"
#include "EffectFactory.h"
#include "d3dx11effect.h"
#include "Effect.h"
#include "ParticleEmitter.h"
#include "../Common Utilities/VTuneAPI.h"

#include "WindowsFunctions.h"

#include <memory>

VTUNE_API_CREATE_DOMAIN(locCreateParticleEmitterDomain, "CreateParticleEmitterDomain");
VTUNE_API_CREATE_HANDLE("CreateParticleEmitter", locCreateParticleEmitterObject);


namespace GraphicsEngine
{
	EmitterFactory::EmitterFactory()
	{
		myEmitterDataNames.Init(16);
		myEmitterPointers.Init(1025);
	}

	EmitterFactory::~EmitterFactory()
	{
		myEmitterPointers.~GrowingArray();
	}

	void EmitterFactory::Init()
	{
		myEmitters.Init();
		for (unsigned short i = 0; i < 1024; i++)
		{
			myEmitterPointers.Add(myEmitters.Allocate<ParticleEmitter, ParticleEmitter>());
			myEmitterPointers.GetLast()->CreateVertexBuffer();
		}
	}

	CU::PoolPointer<ParticleEmitter> EmitterFactory::CreateEmitter(const std::string& anEmitterName)
	{
		VTUNE_API_TASK_BEGIN(locCreateParticleEmitterDomain, locCreateParticleEmitterObject);
		for (CU::PoolPointer<ParticleEmitter>& emitter : myEmitterPointers)
		{
			if (emitter->GetIsDone() == true)
			{
				emitter->Init(myEmitterData[anEmitterName]);
				VTUNE_API_TASK_END(locCreateParticleEmitterDomain);
				return emitter;
			}
		}

		VTUNE_API_TASK_END(locCreateParticleEmitterDomain);
		return nullptr;
	}

	void EmitterFactory::CreateEmitterViaXML(const std::string& aName)
	{
		std::string name = WF::GetFileNameWithoutExtension(aName);
		SharedPointer<EmitterData> newEmitter = new EmitterData[1];
		newEmitter->LoadFile(aName);
		myEmitterData.Insert(name, newEmitter);
		myEmitterDataNames.Add(name);
	}

	bool EmitterFactory::CheckIfExists(const std::string& aEmitterName)
	{
		return myEmitterData.KeyExists(aEmitterName);
	}

	void EmitterFactory::LoadEmitters()
	{	
		CU::GrowingArray<std::string> particleEmitters = WF::GetFilesWithExtension("Data/XML/Particles/Emitters/", ".xml");

		for (const std::string& aFileName : particleEmitters)
		{
			if (aFileName != "")
			{
				CreateEmitterViaXML(aFileName);
			}
		}
	}

	SharedPointer<EmitterData> EmitterFactory::GetEmitterData(const std::string& aEmitterName)
	{
		if (CheckIfExists(aEmitterName) == true)
		{
			return myEmitterData[aEmitterName];
		}
		else
		{
			return nullptr;
		}
	}

	void EmitterFactory::CreateDefaultEmitter(const std::string& aName, const std::string& aTexture)
	{
		SharedPointer<EmitterData> newEmitter = new EmitterData[1];

		newEmitter->myEffect = GfxFactoryWrapper::GetInstance()->GetEffectFactory()->GetEffect("ParticleShader");
		newEmitter->myEmissionExtents = Vector3f(1.f, 1.f, 1.f);
		newEmitter->myEmissionLifeTime = 0.5f;
		newEmitter->myEmissionRatePerSecond = 2000.f;
		newEmitter->myEmissionVelocityDelta = Vector3f(0.5f, 0.5f, 0.5f);

		ParticleData* particleData = new ParticleData();
		particleData->myDeltaAlpha = -1.f;
		particleData->myIsAffectedByGravity = false;
		particleData->myLifeTime = 1.f;
		particleData->myStartScale = { 10.f, 10.f };
		particleData->myMaxVelocity = Vector3f(200.0f, 200.0f, 200.0f);
		particleData->myMinVelocity = Vector3f(-200.0f, -200.0f, -200.0f);
		particleData->myScaleDelta = { -2.f, -2.f };
		particleData->myStartAlpha = 1.0f;
		particleData->myDeAcceleration = 0;
		newEmitter->myParticleData = particleData;
		newEmitter->myMaxNumberOfParticles = unsigned short(newEmitter->myEmissionLifeTime * newEmitter->myEmissionRatePerSecond * particleData->myLifeTime);

		newEmitter->myParticleInputLayout = newEmitter->myEffect->GetInputLayout();

		newEmitter->myTexture = GfxFactoryWrapper::GetInstance()->GetTextureFactory()->GetTexture(aTexture);

		myEmitterData.Insert(aName, newEmitter);
		myEmitterDataNames.Add(aName);
	}
}
#include "Engine_Precompiled.h"
#include "ParticleSystem.h"

#include "ParticleSystemData.h"
#include "EmitterData.h"
#include "GfxFactoryWrapper.h"
#include "EmitterFactory.h"

#include "../Common Utilities/EventManager.h"
#include "../Common Utilities/SharedPointer.h"

namespace GraphicsEngine
{
	ParticleSystem::ParticleSystem()
	{
		myTime = 0;
		myEmitters.Init(4);
	}

	ParticleSystem::ParticleSystem(SharedPointer<ParticleSystemData> someParticleSystemData)
	{
		myTime = 0;
		myEmitters.Init(4);

		Init(someParticleSystemData);
	}

	ParticleSystem::~ParticleSystem()
	{
	}

	void ParticleSystem::Update()
	{
		myTime += CU::EventManager::GetInstance()->GetDeltaTime();

		for (EmitterSpawnData& emitter : myEmitters)
		{
			if (myTime >= emitter.myTime)
			{
				emitter.myEmitter->Update();
			}
		}
	}

	void ParticleSystem::Render(const Camera& aCamera)
	{
		for (EmitterSpawnData& emitter : myEmitters)
		{
			if (myTime >= emitter.myTime)
			{
				emitter.myEmitter->Render(aCamera);
			}
		}
	}

	bool ParticleSystem::IsDone()
	{
		for (EmitterSpawnData& emitter : myEmitters)
		{
			if (myTime >= emitter.myTime)
			{
				if (emitter.myEmitter->GetIsDone() == false)
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}

		return true;
	}

	void ParticleSystem::SetOrientation(const Matrix44<float>& aOrientation, bool aShouldUpdateEmitterOrientation)
	{
		myOrientation = aOrientation;

		if (aShouldUpdateEmitterOrientation == true)
		{
			for (EmitterSpawnData& emitter : myEmitters)
			{
				emitter.myEmitter->SetOrientation(emitter.myOrientation * myOrientation);
			}
		}
	}

	void ParticleSystem::Init(SharedPointer<ParticleSystemData> someParticleSystemData)
	{
		for (unsigned short i = 0; i < someParticleSystemData->myEmitterData.Size(); i++)
		{
			myEmitters.Add(EmitterSpawnData());
			myEmitters.GetLast().myTime = someParticleSystemData->myTimeStamps[i];
			myEmitters.GetLast().myEmitter = GfxFactoryWrapper::GetInstance()->GetEmitterFactory()->CreateEmitter(someParticleSystemData->myEmitterData[i]);
			myEmitters.GetLast().myOrientation = someParticleSystemData->myOrientations[i];

			myEmitters.GetLast().myEmitter->SetOrientation(myEmitters.GetLast().myOrientation * myOrientation);
		}
	}
}
#include "Engine_Precompiled.h"
#include "StreakFactory.h"
#include "StreakData.h"
#include "StreakEmitterData.h"
#include "GfxFactoryWrapper.h"
#include "EffectFactory.h"
#include "Effect.h"
#include "TextureFactory.h"
#include "../Common Utilities/VTuneAPI.h"
#include <d3d11.h>

VTUNE_API_CREATE_DOMAIN(locCreateStreakEmitterDomain, "CreateStreakEmitterDomain");
VTUNE_API_CREATE_HANDLE("CreateStreakEmitter", locCreateStreakEmitterObject);

namespace GraphicsEngine
{

	StreakFactory::StreakFactory()
	{
		myStreakDataNames.Init(10);
		myEmitterPointers.Init(NUMBER_OF_STREAK_EMITTERS + 1);
	}

	StreakFactory::~StreakFactory()
	{
		for (unsigned short i = 0; i < myEmitterPointers.Size(); i++)
		{
			myEmitterPointers[i] = nullptr;
		}
	}

	void StreakFactory::Init()
	{
		myEmitters.Init();
		for (unsigned short i = 0; i < NUMBER_OF_STREAK_EMITTERS; i++)
		{
			myEmitterPointers.Add(myEmitters.Allocate<StreakEmitter, StreakEmitter>());
			myEmitterPointers.GetLast()->CreateVertexBuffer();
		}
	}

	CU::PoolPointer<StreakEmitter> StreakFactory::CreateEmitter(const std::string& anEmitterName)
	{
		VTUNE_API_TASK_BEGIN(locCreateStreakEmitterDomain, locCreateStreakEmitterObject);
		for (CU::PoolPointer<StreakEmitter>& emitter : myEmitterPointers)
		{
			if (emitter->GetIsDone() == true)
			{
				emitter->Init(myStreakData[anEmitterName]);
				VTUNE_API_TASK_END(locCreateStreakEmitterDomain);
				return emitter;
			}
		}

		VTUNE_API_TASK_END(locCreateStreakEmitterDomain);
		return nullptr;
	}

	const CU::GrowingArray<CU::PoolPointer<StreakEmitter>>& StreakFactory::GetStreakEmitters() const
	{
		return myEmitterPointers;
	}

	void StreakFactory::LoadEmitters()
	{
		//StreakEmitterData* newEmitter = new StreakEmitterData[1];

		//newEmitter->myEffect = GfxFactoryWrapper::GetInstance()->GetEffectFactory()->GetEffect("StreakShader");
		//newEmitter->myEmissionExtents = Vector3f(0.f, 0.f, 0.f);
		//newEmitter->myEmissionLifeTime = 1000.f;
		//newEmitter->myEmissionRatePerSecond = 100.f;
		//newEmitter->myEmissionVelocityDelta = Vector3f(-0.01f, -0.01f, -0.01f);
		//newEmitter->myMaxNumberOfStreaks = 500;

		//StreakData* streakData = new StreakData();
		//streakData->myDeltaAlpha = -0.3f;
		//streakData->myIsAffectedByGravity = false;
		//streakData->myLifeTime = 5.f;
		//streakData->myStartSize = 5.f;
		//streakData->myVelocity = Vector3f(0.0f, 0.0f, 0.0f);
		//streakData->myDeltaSize = -0.3f;
		//streakData->myStartAlpha = 1.5f;
		//streakData->myDeAcceleration = 0;
		//newEmitter->myStreakData = streakData;

		//newEmitter->myStreakInputLayout = newEmitter->myEffect->GetInputLayout();

		//newEmitter->myTexture = GfxFactoryWrapper::GetInstance()->GetTextureFactory()->GetTexture("streakTexture");

		//myStreakData.Insert("Wardh", newEmitter);
		//myStreakDataNames.Add("Wardh");
		////Placeholder bulletstreak
		//StreakEmitterData* newEmitter2 = new StreakEmitterData[1];

		//newEmitter2->myEffect = GfxFactoryWrapper::GetInstance()->GetEffectFactory()->GetEffect("StreakShader");
		//newEmitter2->myEmissionExtents = Vector3f(0.f, 0.f, 0.f);
		//newEmitter2->myEmissionLifeTime = 1000.f;
		//newEmitter2->myEmissionRatePerSecond = 100.f;
		//newEmitter2->myEmissionVelocityDelta = Vector3f(-0.01f, -0.01f, -0.01f);
		//newEmitter2->myMaxNumberOfStreaks = 440;

		//StreakData* streakData2 = new StreakData();
		//streakData2->myDeltaAlpha = -0.8f;
		//streakData2->myIsAffectedByGravity = false;
		//streakData2->myLifeTime = 0.2f;
		//streakData2->myStartSize = 0.2f;
		//streakData2->myVelocity = Vector3f(0.0f, 0.0f, 0.0f);
		//streakData2->myDeltaSize = 0.2f;
		//streakData2->myStartAlpha = 0.8f;
		//streakData2->myDeAcceleration = 0;
		//newEmitter2->myStreakData = streakData2;

		//newEmitter2->myStreakInputLayout = newEmitter2->myEffect->GetInputLayout();

		//newEmitter2->myTexture = GfxFactoryWrapper::GetInstance()->GetTextureFactory()->GetTexture("bulletStreakTexture");

		//myStreakData.Insert("BulletStreak", newEmitter2);
		//myStreakDataNames.Add("BulletStreak");

		////Placeholder missilestreak

		//StreakEmitterData* newEmitter3 = new StreakEmitterData[1];

		//newEmitter3->myEffect = GfxFactoryWrapper::GetInstance()->GetEffectFactory()->GetEffect("StreakShader");
		//newEmitter3->myEmissionExtents = Vector3f(0.f, 0.f, 0.f);
		//newEmitter3->myEmissionLifeTime = 100.f;
		//newEmitter3->myEmissionRatePerSecond = 25.f;
		//newEmitter3->myEmissionVelocityDelta = Vector3f(-0.01f, -0.01f, -0.01f);
		//newEmitter3->myMaxNumberOfStreaks = 250;

		//StreakData* streakData3 = new StreakData();
		//streakData3->myDeltaAlpha = -0.4f;
		//streakData3->myIsAffectedByGravity = false;
		//streakData3->myLifeTime = 10.f;
		//streakData3->myStartSize = 0.3f;
		//streakData3->myVelocity = Vector3f(0.0f, 0.0f, 0.0f);
		//streakData3->myDeltaSize = -0.1f;
		//streakData3->myStartAlpha = 4.0f;
		//streakData3->myDeAcceleration = 0;
		//newEmitter3->myStreakData = streakData3;

		//newEmitter3->myStreakInputLayout = newEmitter3->myEffect->GetInputLayout();

		//newEmitter3->myTexture = GfxFactoryWrapper::GetInstance()->GetTextureFactory()->GetTexture("missileStreakTexture");

		//myStreakData.Insert("MissileStreak", newEmitter3);
		//myStreakDataNames.Add("MissileStreak");

	}
}


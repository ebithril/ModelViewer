#include "Engine_Precompiled.h"
#include "Renderer.h"
#include "Texture.h"
#include "DirectXEngine.h"
#include "DirectX2D.h"
#include "SceneRenderData.h"
#include "Model.h"
#include "d3dx11effect.h"
#include "ParticleSystem.h"
#include "StreakEmitter.h"
#include "LineRenderer.h"
#include "Instance.h"

#include "../Common Utilities/SceneRenderCommands.h"
#include "../Common Utilities/VTuneAPI.h"
#include "../Common Utilities/StaticArray.h"
#include "../Common Utilities/EventManager.h"

VTUNE_API_CREATE_DOMAIN(locBloomDomain, "BloomDomain")
VTUNE_API_CREATE_HANDLE("Bloom", locBloomTimeStamp)

VTUNE_API_CREATE_DOMAIN(locShadowMapDomain, "ShadowMapDomain")
VTUNE_API_CREATE_HANDLE("CreateShadowMaps", locShadowMapTimeStamp)

VTUNE_API_CREATE_DOMAIN(locInstancesRenderDomain, "InstancesRenderDomain")
VTUNE_API_CREATE_HANDLE("InstancesRender", locInstancesRenderTimeStamp)

VTUNE_API_CREATE_DOMAIN(locLineRenderDomain, "LineRenderDomain")
VTUNE_API_CREATE_HANDLE("LineRender", locLineRenderTimeStamp)

VTUNE_API_CREATE_DOMAIN(locParticleRenderDomain, "ParticleRenderDomain")
VTUNE_API_CREATE_HANDLE("ParticleRender", locParticleRenderTimeStamp)

VTUNE_API_CREATE_DOMAIN(locFrameRenderSetupDomain, "FrameRenderSetupDomain")
VTUNE_API_CREATE_HANDLE("FrameRenderSetup", locFrameRenderSetupTimeStamp)

VTUNE_API_CREATE_DOMAIN(locHdrDomain, "HdrDomain")
VTUNE_API_CREATE_HANDLE("HDR", locHdrTimeStamp)

VTUNE_API_CREATE_DOMAIN(locMotionBlurDomain, "MotionBlurDomain")
VTUNE_API_CREATE_HANDLE("MotionBlur", locMotionBlurTimeStamp)

VTUNE_API_CREATE_DOMAIN(locAlphaInstancesDomain, "AlphaInstancesDomain")
VTUNE_API_CREATE_HANDLE("AlphaInstances", locAlphaInstancesTimeStamp)

namespace GraphicsEngine
{
	Renderer::Renderer()
	{
		myEngine = nullptr;
		myHDRScreenTarget = nullptr;
		myPostProcessScreenTarget = nullptr;
		mySecondPostProcessScreenTarget = nullptr;
		myLineRenderer = nullptr;
		myObjectVelocityData = nullptr;
		myAlphaIndexes.Init(128);
	}

	Renderer::~Renderer()
	{
		SAFE_DELETE(myLineRenderer);
		myShadowManager.Destroy();
	}

	//TEST
	DirectXEngine* Renderer::GetEngine()
	{
		return myEngine;
	}

	void Renderer::Init(DirectXEngine* anEngine)
	{
		myEngine = anEngine;

		myLineRenderer = new LineRenderer();
		myLineRenderer->Init(myEngine);

		myFullScreenHelper.Init(myEngine);
		myDispatcher.Init(myEngine);
		myBloomManager.Init(myEngine->GetSetupInfo().myResolution.x, myEngine->GetSetupInfo().myResolution.myY, &myFullScreenHelper, &myDispatcher, myEngine);
		myHDRManager.Init(myEngine->GetSetupInfo().myResolution.x, myEngine->GetSetupInfo().myResolution.myY, &myFullScreenHelper, &myDispatcher, myEngine);
		myMotionBlurManager.Init(&myFullScreenHelper, myEngine);
		myFaderHelper.Init(myEngine->GetSetupInfo().myResolution.x, myEngine->GetSetupInfo().myResolution.myY, &myFullScreenHelper, &myDispatcher, myEngine);

		OnResize(myEngine->GetSetupInfo().myResolution);
		ObserveEvent(CU::eEvent::TOGGLE_SHADOWS, HANDLE_EVENT_FUNCTION(HandleToggleShadowEvent));
		myShadowsIsEnabled = true;

		myShadowManager.Init();
	}

	void Renderer::OnResize(Vector2<int> aResolution)
	{
		SAFE_DELETE(myHDRScreenTarget);
		SAFE_DELETE(myPostProcessScreenTarget);
		SAFE_DELETE(mySecondPostProcessScreenTarget);
		SAFE_DELETE(myObjectVelocityData);

		myHDRScreenTarget = new Texture(myEngine, aResolution, DXGI_FORMAT_R16G16B16A16_FLOAT, true, true, true);
		myPostProcessScreenTarget = new Texture(myEngine, aResolution, DXGI_FORMAT_R8G8B8A8_UNORM, true, true, false);
		mySecondPostProcessScreenTarget = new Texture(myEngine, aResolution, DXGI_FORMAT_R8G8B8A8_UNORM, true, true, false);
		myObjectVelocityData = new Texture(myEngine, aResolution, DXGI_FORMAT_R16G16_FLOAT, true, true, true);

		myBloomManager.OnResize(aResolution);
	}

	void Renderer::PreSceneSetup(RenderTargetData& someData)
	{
		someData;
		float ClearColor[4] = { 0.5f, 0.02f, 0.02f, 1.0f }; //red,green,blue,alpha
		myEngine->GetContext()->ClearRenderTargetView(myHDRScreenTarget->GetRenderTargetView().Get(), ClearColor);
		myEngine->GetContext()->ClearRenderTargetView(myPostProcessScreenTarget->GetRenderTargetView().Get(), ClearColor);
		myEngine->GetContext()->ClearRenderTargetView(mySecondPostProcessScreenTarget->GetRenderTargetView().Get(), ClearColor);

		float ClearColorVelocity[2] = { 0.00f, 0.00f }; //red,green,blue,alpha
		myEngine->GetContext()->ClearRenderTargetView(myObjectVelocityData->GetRenderTargetView().Get(), ClearColorVelocity);
	}

	void Renderer::RenderScene(RenderTargetData& someRenderData, CU::SceneRenderCommands& someSceneData)
	{
		VTUNE_API_TASK_BEGIN(locFrameRenderSetupDomain, locFrameRenderSetupTimeStamp);
		PreSceneSetup(someRenderData);
		VTUNE_API_TASK_END(locFrameRenderSetupDomain);

		if (myShadowsIsEnabled == true)
		{
			VTUNE_API_TASK_BEGIN(locShadowMapDomain, locShadowMapTimeStamp);
			DrawShadowMaps(someSceneData);
			VTUNE_API_TASK_END(locShadowMapDomain);
		}

		COMObjectPointer<ID3D11RenderTargetView> RenderTargets[2];
		if (someSceneData.mySceneData->myPostProcessOperations & ePostProcess::HDR)
		{
			RenderTargets[0] = myHDRScreenTarget->GetRenderTargetView();
		}
		else
		{
			RenderTargets[0] = myPostProcessScreenTarget->GetRenderTargetView();
		}

		RenderTargets[1] = myObjectVelocityData->GetRenderTargetView();
		myEngine->GetContext()->OMSetRenderTargets(2, &RenderTargets[0], someRenderData.myDepthStencil.GetDepthStencilView().Get());



		VTUNE_API_TASK_BEGIN(locInstancesRenderDomain, locInstancesRenderTimeStamp);
		DrawInstances(someSceneData);
		VTUNE_API_TASK_END(locInstancesRenderDomain);

		VTUNE_API_TASK_BEGIN(locLineRenderDomain, locLineRenderTimeStamp);
		myLineRenderer->UpdateLineShader(someSceneData.mySceneData->myCamera);
		myLineRenderer->DrawLines(someSceneData.myLineRenderCommands);
		VTUNE_API_TASK_END(locLineRenderDomain);

		VTUNE_API_TASK_BEGIN(locParticleRenderDomain, locParticleRenderTimeStamp);
		DrawStreaks(someSceneData);
		DrawEmitters(someSceneData);
		VTUNE_API_TASK_END(locParticleRenderDomain);

		VTUNE_API_TASK_BEGIN(locAlphaInstancesDomain, locAlphaInstancesTimeStamp);
		DrawAlphaInstances(someSceneData);
		VTUNE_API_TASK_END(locAlphaInstancesDomain);

		if (someSceneData.mySceneData->myPostProcessOperations & ePostProcess::HDR)
		{
			VTUNE_API_TASK_BEGIN(locHdrDomain, locHdrTimeStamp);
			myHDRManager.Process(myPostProcessScreenTarget->GetRenderTargetView(), myHDRScreenTarget);
			VTUNE_API_TASK_END(locHdrDomain);
		}

		if (someSceneData.mySceneData->myPostProcessOperations & ePostProcess::BLOOM)
		{
			VTUNE_API_TASK_BEGIN(locBloomDomain, locBloomTimeStamp);
			myBloomManager.Process(mySecondPostProcessScreenTarget->GetRenderTargetView(), myPostProcessScreenTarget);
			VTUNE_API_TASK_END(locBloomDomain);
		}

		/*VTUNE_API_TASK_BEGIN(locMotionBlurDomain, locMotionBlurTimeStamp);
		myMotionBlurManager.Process(mySecondPostProcessScreenTarget->GetRenderTargetView(), myPostProcessScreenTarget, myObjectVelocityData);
		VTUNE_API_TASK_END(locMotionBlurDomain);*/

		//myEngine->GetContext()->OMSetRenderTargets(1, &someRenderData.myRenderTarget, someRenderData.myDepthStencil.Get());

		if (someSceneData.mySceneData->myPostProcessOperations & ePostProcess::BLOOM)
		{
			myFaderHelper.Process(someRenderData.myRenderTarget.GetRenderTargetView(), mySecondPostProcessScreenTarget);
		}
		else
		{
			myFaderHelper.Process(someRenderData.myRenderTarget.GetRenderTargetView(), myPostProcessScreenTarget);
		}

		ID3D11RenderTargetView* nullRenderTargets[2];
		nullRenderTargets[0] = {};
		nullRenderTargets[1] = {};

		ID3D11DepthStencilView* nullDepthStencils[2];
		nullDepthStencils[0] = {};
		nullDepthStencils[1] = {};
		myEngine->GetContext()->OMSetRenderTargets(2, &nullRenderTargets[0], nullDepthStencils[0]);
	}

	void Renderer::DrawEmitters(CU::SceneRenderCommands& someSceneData)
	{
		for (unsigned short i = 0; i < someSceneData.mySceneData->myParticleSystems.Size(); ++i)
		{
			someSceneData.mySceneData->myParticleSystems[i]->Render(someSceneData.mySceneData->myCamera);
		}
	}

	void Renderer::DrawStreaks(CU::SceneRenderCommands& someSceneData)
	{
		for (unsigned short i = 0; i < someSceneData.mySceneData->myStreakEmitters.Size(); ++i)
		{
			someSceneData.mySceneData->myStreakEmitters[i]->Render(someSceneData.mySceneData->myCamera);
		}
	}

	void Renderer::DrawShadowMaps(CU::SceneRenderCommands& someData)
	{
		myShadowManager.DrawShadowMaps(someData);
	}

	void Renderer::DrawInstances(CU::SceneRenderCommands& someSceneData)
	{
		CU::GrowingArray<CU::RenderCommandInstance>& someRenderCommands = someSceneData.myRenderCommandInstances;

		for (unsigned short i = 0; i < someRenderCommands.Size(); i++)
		{
			CU::RenderCommandInstance cmd = someRenderCommands[i];
			Instance* currentInstance = cmd.myModel;

			if (currentInstance != nullptr && currentInstance->ModelIsNullPointer() == false && currentInstance->IsLoaded() == true)
			{

				if (currentInstance->GetEffect() != nullptr && currentInstance->GetEffect()->GetIsUsingAlpha() == true)
				{
					myAlphaIndexes.Add(i);
					continue;
				}

				if (someRenderCommands[i].myAffectedByFow)
				{
					currentInstance->GetEffect()->SetTechnique("RenderWithFow");
				}
				else
				{
					currentInstance->GetEffect()->SetTechnique("Render");
				}

				currentInstance->Render(*someSceneData.mySceneData, cmd);
			}
		}
	}

	void Renderer::DrawAlphaInstances(CU::SceneRenderCommands& someSceneData)
	{
		CU::GrowingArray<CU::RenderCommandInstance>& someRenderCommands = someSceneData.myRenderCommandInstances;

		for (unsigned short i = 0; i < myAlphaIndexes.Size(); i++)
		{
			CU::RenderCommandInstance cmd = someRenderCommands[myAlphaIndexes[i]];
			Instance* currentInstance = cmd.myModel;

			if (currentInstance != nullptr)
			{
				currentInstance->Render(*someSceneData.mySceneData, cmd);
			}
		}

		myAlphaIndexes.RemoveAll();
	}

	bool Renderer::HandleToggleShadowEvent(const CU::PoolPointer<CU::Event>& anEvent)
	{
		anEvent;
		myShadowsIsEnabled = !myShadowsIsEnabled;
		return true;
	}

	ShadowManager& Renderer::GetShadowManager()
	{
		return myShadowManager;
	}

}
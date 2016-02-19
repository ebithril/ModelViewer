#include "Engine_Precompiled.h"
#include "HDRManager.h"

#include "Texture.h"
#include "DispatchHelper.h"
#include "FullScreenHelper.h"
#include "d3dx11effect.h"
#include "DirectXEngine.h"
#include "../Common Utilities/EventManager.h"

namespace GraphicsEngine
{
	HDRManager::HDRManager()
	{
		myDownSampleTargets = nullptr;
		myPostProcess = nullptr;
		myDispatcher = nullptr;
		myDirectXEngine = nullptr;
		for (unsigned short i = 0; i < 3; i++)
		{
			myOldLuminanceTargets[i] = nullptr;
			myAdaptedLuminanceTargets[i] = nullptr;
		}
	}


	HDRManager::~HDRManager()
	{
	}

	void HDRManager::Init(int aWidth, int aHeight, FullScreenHelper* aPostProcesser, DispatchHelper* aDispatcher, DirectXEngine* aDirectXEngine)
	{
		myDirectXEngine = aDirectXEngine;
		myPostProcess = aPostProcesser;
		myDispatcher = aDispatcher;

		OnResize({ aWidth, aHeight });

	}

	void HDRManager::Process(COMObjectPointer<ID3D11RenderTargetView>& aDestination, Texture* aSource)
	{
		myPostProcess->Process("RenderLum", myDownSampleTargets[myTargetCount - 1], aSource);
		myDirectXEngine->GetContext()->OMSetRenderTargets(0, 0, 0);

		for (int i = myTargetCount - 2; i >= 0; i--)
		{
			myDispatcher->Dispatch(myDownSampleTargets[i],
				myDownSampleTargets[i + 1], 
				"DownSample2x2Log", 
				static_cast<int>(ceilf(myDownSampleTargets[i]->GetImageSize().x / 32.f) + 1),
				static_cast<int>(ceilf(myDownSampleTargets[i]->GetImageSize().y / 32.f) + 1));
		}

		float deltaTime = CU::EventManager::GetInstance()->GetDeltaTime();

		for (int i = 0; i < 1; i++)
		{
			Texture* temp = myAdaptedLuminanceTargets[i];
			myAdaptedLuminanceTargets[i] = myOldLuminanceTargets[i];
			myOldLuminanceTargets[i] = temp;
		
			myPostProcess->GetEffect()->GetEffect()->GetVariableByName("SecondaryDiffuseTexture")->AsShaderResource()->SetResource(myOldLuminanceTargets[i]->GetShaderView().Get());
			myPostProcess->GetEffect()->GetEffect()->GetVariableByName("DeltaTime")->AsScalar()->SetFloat(deltaTime);

			myPostProcess->Process("AdaptLuminance", myAdaptedLuminanceTargets[i], myDownSampleTargets[i]);
		}
		myDirectXEngine->GetContext()->OMSetRenderTargets(0, 0, 0);
		myPostProcess->GetEffect()->GetEffect()->GetVariableByName("SecondaryDiffuseTexture")->AsShaderResource()->SetResource(myAdaptedLuminanceTargets[0]->GetShaderView().Get());
		myPostProcess->Process("ToneMap", aDestination, aSource);
	}

	void HDRManager::OnResize(const Vector2<int>& aResolution)
	{
		for (int i = 0; i < myTargetCount; i++)
		{
			SAFE_DELETE(myDownSampleTargets[i]);
		}

		if (myDownSampleTargets != NULL)
		{
			SAFE_DELETE_ARRAY(myDownSampleTargets);
		}

		myWidth = aResolution.x;
		myHeight = aResolution.y;

		myTargetCount = static_cast<int>(std::log2(__min(myWidth, myHeight)) + 1);
		myDownSampleTargets = new Texture*[myTargetCount];
		int size = 1;
		for (int i = 0; i < myTargetCount; i++)
		{
			myDownSampleTargets[i] = new Texture(myDirectXEngine, Vector2<int>(size, size), DXGI_FORMAT_R16G16B16A16_FLOAT, true, true, true);
			size *= 2;
		}

		size = 4;

		for (int i = 2; i >= 0; i--)
		{
			myOldLuminanceTargets[i] = new Texture(myDirectXEngine, { size, size }, DXGI_FORMAT_R16G16B16A16_FLOAT, true, true, true);
			myAdaptedLuminanceTargets[i] = new Texture(myDirectXEngine, { size, size }, DXGI_FORMAT_R16G16B16A16_FLOAT, true, true, true);
			float ClearColor[4] = { 0.0f, 0.0, 0.0f, 0.0f }; //red,green,blue,alpha
			myDirectXEngine->GetContext()->ClearRenderTargetView(myAdaptedLuminanceTargets[i]->GetRenderTargetView().Get(), ClearColor);
			myDirectXEngine->GetContext()->ClearRenderTargetView(myOldLuminanceTargets[i]->GetRenderTargetView().Get(), ClearColor);
			size /= 2;
		}
	}

}
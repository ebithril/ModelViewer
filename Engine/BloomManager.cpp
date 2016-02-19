#include "Engine_Precompiled.h"
#include "BloomManager.h"
#include "FullScreenHelper.h"
#include "DispatchHelper.h"
#include <d3d11.h>
#include "Texture.h"
#include "d3dx11effect.h"
#include "DirectXEngine.h"

namespace GraphicsEngine
{
	BloomManager::BloomManager()
	{
		myDirectXEngine = nullptr;
		myBlurTarget = nullptr;
		myDownSampleBlurTarget2X2 = nullptr;
		myDownSampleTarget2X2 = nullptr;
		myDownSampleBlurTarget4X4 = nullptr;
		myDownSampleTarget4X4 = nullptr;
		myDownSampleBlurTarget16X16 = nullptr;
		myDownSampleTarget16X16 = nullptr;
	}

	BloomManager::~BloomManager()
	{
		delete myBlurTarget;
		delete myDownSampleBlurTarget2X2;
		delete myDownSampleTarget2X2;
		delete myDownSampleBlurTarget4X4;
		delete myDownSampleTarget4X4;
		delete myDownSampleBlurTarget16X16;
		delete myDownSampleTarget16X16;
	}

	void BloomManager::OnResize(const Vector2<int>& aResolution)
	{
		if (myDirectXEngine != nullptr)
		{
			SAFE_DELETE(myBlurTarget);
			SAFE_DELETE(myDownSampleBlurTarget2X2);
			SAFE_DELETE(myDownSampleTarget2X2);
			SAFE_DELETE(myDownSampleBlurTarget4X4);
			SAFE_DELETE(myDownSampleTarget4X4);
			SAFE_DELETE(myDownSampleBlurTarget16X16);
			SAFE_DELETE(myDownSampleTarget16X16);

			myWidth = aResolution.myX;
			myHeight = aResolution.myY;

			myBlurTarget = new Texture(myDirectXEngine, { myWidth, myHeight }, DXGI_FORMAT_R8G8B8A8_UNORM, true, false, true);

			myDownSampleTarget2X2 = new Texture(myDirectXEngine, { myWidth / 2, myHeight / 2 }, DXGI_FORMAT_R8G8B8A8_UNORM, true, false, true);
			myDownSampleBlurTarget2X2 = new Texture(myDirectXEngine, { myWidth / 2, myHeight / 2 }, DXGI_FORMAT_R8G8B8A8_UNORM, true, false, true);

			myDownSampleTarget4X4 = new Texture(myDirectXEngine, { myWidth / 4, myHeight / 4 }, DXGI_FORMAT_R8G8B8A8_UNORM, true, false, true);
			myDownSampleBlurTarget4X4 = new Texture(myDirectXEngine, { myWidth / 4, myHeight / 4 }, DXGI_FORMAT_R8G8B8A8_UNORM, true, false, true);

			myDownSampleTarget16X16 = new Texture(myDirectXEngine, { myWidth / 8, myHeight / 8 }, DXGI_FORMAT_R8G8B8A8_UNORM, true, false, true);
			myDownSampleBlurTarget16X16 = new Texture(myDirectXEngine, { myWidth / 8, myHeight / 8 }, DXGI_FORMAT_R8G8B8A8_UNORM, true, false, true);
		}
	}

	void BloomManager::Init(int aWidth, int aHeight, FullScreenHelper* aPostProcess, DispatchHelper* aDispatcher, DirectXEngine* anEngine)
	{
		myDirectXEngine = anEngine;
		myWidth = aWidth;
		myHeight = aHeight;

		myPostProcess = aPostProcess;
		myDispatcher = aDispatcher;

		OnResize(Vector2<int>(myWidth, myHeight));
	}

	void BloomManager::Process(COMObjectPointer<ID3D11RenderTargetView>& aRenderTargetView, Texture* aSource)
	{
		DownSample(aSource);
		myPostProcess->GetEffect()->GetEffect()->GetVariableByName("SecondaryDiffuseTexture")->AsShaderResource()->SetResource(myDownSampleTarget2X2->GetShaderView().Get());
		myPostProcess->GetEffect()->GetEffect()->GetVariableByName("TertiaryDiffuseTexture")->AsShaderResource()->SetResource(myDownSampleTarget4X4->GetShaderView().Get());
		myPostProcess->GetEffect()->GetEffect()->GetVariableByName("QuartaryDiffuseTexture")->AsShaderResource()->SetResource(myDownSampleTarget16X16->GetShaderView().Get());
		myPostProcess->Process("Bloom", aRenderTargetView, myWidth, myHeight, aSource);
	}

	void BloomManager::DownSample(Texture* aSource)
	{
		myDirectXEngine->GetContext()->OMSetRenderTargets(0, 0, 0);

		UINT numGroupsX = (UINT)ceilf(myDownSampleTarget2X2->GetImageSize().x / 32.f) + 1;
		UINT numGroupsY = (UINT)ceilf(myDownSampleTarget2X2->GetImageSize().y / 32.f) + 1;
		myDispatcher->Dispatch(myDownSampleTarget2X2, aSource, "DownSample2x2AndHiPass", numGroupsX, numGroupsY);
		numGroupsX = (UINT)ceilf(myDownSampleTarget4X4->GetImageSize().x / 32.f) + 1;
		numGroupsY = (UINT)ceilf(myDownSampleTarget4X4->GetImageSize().y / 32.f) + 1;
		myDispatcher->Dispatch(myDownSampleTarget4X4, myDownSampleTarget2X2, "DownSample2x2", numGroupsX, numGroupsY);
		numGroupsX = (UINT)ceilf(myDownSampleTarget16X16->GetImageSize().x / 32.f) + 1;
		numGroupsY = (UINT)ceilf(myDownSampleTarget16X16->GetImageSize().y / 32.f) + 1;
		myDispatcher->Dispatch(myDownSampleTarget16X16, myDownSampleTarget4X4, "DownSample2x2", numGroupsX, numGroupsY);

		Blur(myDownSampleTarget2X2, myDownSampleBlurTarget2X2);
		Blur(myDownSampleTarget4X4, myDownSampleBlurTarget4X4);
		Blur(myDownSampleTarget16X16, myDownSampleBlurTarget16X16);
	}

	void BloomManager::Blur(Texture* aSource, Texture* aBounce)
	{
		UINT numGroupsX = (UINT)ceilf(aSource->GetImageSize().x / 256.0f) + 1;
		UINT numGroupsY = (UINT)ceilf(aSource->GetImageSize().y / 256.0f) + 1;
		myDispatcher->Dispatch(aBounce, aSource, "HorzBlur", numGroupsX, aSource->GetImageSize().y);
		myDispatcher->Dispatch(aSource, aBounce, "VertBlur", aSource->GetImageSize().x, numGroupsY);
	}
}
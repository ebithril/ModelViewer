#include "Engine_Precompiled.h"
#include "FaderHelper.h"
#include "FullScreenHelper.h"
#include "DispatchHelper.h"
#include <d3d11.h>
#include "Texture.h"
#include "d3dx11effect.h"
#include "DirectXEngine.h"
#include "../Common Utilities/EventManager.h"
#include "EngineEvents.h"

namespace GraphicsEngine
{
	FaderHelper::FaderHelper()
	{
		myDirectXEngine = nullptr;
		myFadedTarget = nullptr;
		myFadeAmount = 0.f;
		ObserveEvent(CU::eEvent::FADE_IN, HANDLE_EVENT_FUNCTION(HandleFadeInEvent));
		ObserveEvent(CU::eEvent::FADE_OUT, HANDLE_EVENT_FUNCTION(HandleFadeOutEvent));
		myFadeColor = { 0, 0, 0 };
		myFadeTime = 0;
		myFadeDeltaTime = 0;
	}


	FaderHelper::~FaderHelper()
	{
		delete myFadedTarget;
	}


	void FaderHelper::Init(int aWidth, int aHeight, FullScreenHelper* aPostProcess, DispatchHelper* aDispatcher, DirectXEngine* anEngine)
	{
		myDirectXEngine = anEngine;
		myWidth = aWidth;
		myHeight = aHeight;

		myPostProcess = aPostProcess;
		myDispatcher = aDispatcher;
		OnResize(Vector2<int>(myWidth, myHeight));
	}

	void FaderHelper::Process(COMObjectPointer<ID3D11RenderTargetView>& aRenderTargetView, Texture* aSource)
	{
		if (myIsFading == true)
		{
			myPostProcess->SetFadeColor(myFadeColor);
			myFadeDeltaTime += CU::EventManager::GetInstance()->GetDeltaTime();
			if (myFadeType == eFadeTypes::FADE_IN)
			{
				myFadeAmount = 1 - (myFadeDeltaTime / myFadeTime);
			}
			else if (myFadeType == eFadeTypes::FADE_OUT)
			{
				myFadeAmount = myFadeDeltaTime / myFadeTime;
			}
			if (myFadeAmount > 1.f)
			{
				myFadeAmount = 1.f;
				myFadeDeltaTime = 0;
				myIsFading = false;
			}
			if (myFadeAmount < 0.f)
			{
				myFadeAmount = 0.f;
				myFadeDeltaTime = 0;
				myIsFading = false;
			}
		}



		myPostProcess->Process("Fade", aRenderTargetView, myWidth, myHeight, aSource, myFadeAmount);
	}

	void FaderHelper::OnResize(const Vector2<int>& aResolution)
	{
		if (myDirectXEngine != nullptr)
		{
			SAFE_DELETE(myFadedTarget);

			myWidth = aResolution.myX;
			myHeight = aResolution.myY;

			myFadedTarget = new Texture(myDirectXEngine, { myWidth, myHeight }, DXGI_FORMAT_R8G8B8A8_UNORM, true, false, true);
		}
	}

	bool FaderHelper::HandleFadeInEvent(const CU::PoolPointer<CU::Event>& anEvent)
	{
		GET_EVENT_DATA(anEvent, GE::FadeInEvent, fadeEvent);

		myFadeColor = fadeEvent->myColor;
		myFadeTime = fadeEvent->myFadeTime;
		myFadeType = eFadeTypes::FADE_IN;
		myIsFading = true;

		return true;
	}

	bool FaderHelper::HandleFadeOutEvent(const CU::PoolPointer<CU::Event>& anEvent)
	{
		GET_EVENT_DATA(anEvent, GE::FadeOutEvent, fadeEvent);

		myFadeColor = fadeEvent->myColor;
		myFadeTime = fadeEvent->myFadeTime;
		myFadeType = eFadeTypes::FADE_OUT;
		myIsFading = true;

		return true;
	}



}
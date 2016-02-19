#include "Engine_Precompiled.h"
#include "MotionBlurManager.h"
#include "d3dx11effect.h"
#include "DirectXEngine.h"
#include "FullScreenHelper.h"
#include "Texture.h"
#include "Effect.h"

namespace GraphicsEngine
{
	void MotionBlurManager::Init(FullScreenHelper* aPostProcess, DirectXEngine* anEngine)
	{
		myPostProcess = aPostProcess;
		myDirectXEngine = anEngine;
	}

	void MotionBlurManager::Process(COMObjectPointer<ID3D11RenderTargetView>& aRenderTargetView, Texture* aSource, Texture* aVelocityTexture)
	{
		myDirectXEngine->GetContext()->OMSetRenderTargets(0, 0, 0);
		myPostProcess->GetEffect()->GetEffect()->GetVariableByName("SecondaryDiffuseTexture")->AsShaderResource()->SetResource(aVelocityTexture->GetShaderView().Get());
		myPostProcess->Process("MotionBlur", aRenderTargetView, aSource);
	}

	MotionBlurManager::MotionBlurManager()
	{
		myDirectXEngine = nullptr;
		myPostProcess = nullptr;
	}

	MotionBlurManager::~MotionBlurManager()
	{
	}
}
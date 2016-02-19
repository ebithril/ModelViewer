#include "Engine_Precompiled.h"
#include "DispatchHelper.h"
#include "Texture.h"
#include "d3dx11effect.h"
#include "DirectXEngine.h"
#include "VertexDataWrapper.h"
#include "EffectFactory.h"
#include "GfxFactoryWrapper.h"
#include "WindowsFunctions.h"
#include "COMObjectPointer.h"

namespace GraphicsEngine
{
	DispatchHelper::DispatchHelper()
	{
	}


	DispatchHelper::~DispatchHelper()
	{
	}

	bool DispatchHelper::Init(DirectXEngine* aDirectXEngine)
	{
		myComputeEffect = GfxFactoryWrapper::GetInstance()->GetEffectFactory()->GetEffect("ComputeShader");
		myDirectXEngine = aDirectXEngine;
		return true;
	}

	void DispatchHelper::Dispatch(Texture* aTarget, Texture* aSource, std::string aTechnique, int xDispatch, int yDispatch)
	{
		D3DX11_TECHNIQUE_DESC techDesc;
		HRESULT result = myComputeEffect->GetNamedTechnique(aTechnique)->GetDesc(&techDesc);

		if (FAILED(result))
		{
			std::string errorText = "Failed to get compute technique named: ";
			errorText += aTechnique;
			errorText += ".";

			WF::AssertComAndWindowsError(result, errorText.c_str());
		}

		for (UINT p = 0; p < techDesc.Passes; ++p)
		{
			COMObjectPointer<ID3DX11EffectShaderResourceVariable> input = myComputeEffect->GetEffect()->GetVariableByName("inputData")->AsShaderResource();
			assert(input->IsValid());
			input->SetResource(aSource->GetShaderView().Get());
			COMObjectPointer<ID3DX11EffectUnorderedAccessViewVariable> output = myComputeEffect->GetEffect()->GetVariableByName("outputData")->AsUnorderedAccessView();
			assert(output->IsValid());
			output->SetUnorderedAccessView(aTarget->GetUnorderedAccessView().Get());

			COMObjectPointer<ID3DX11EffectPass> pass = myComputeEffect->GetNamedTechnique(aTechnique)->GetPassByIndex(p);
			pass->Apply(0, myDirectXEngine->GetContext().Get());
			// How many groups do we need to dispatch to cover a row of pixels, where each
			// group covers 256 pixels (the 256 is defined in the ComputeShader).

			myDirectXEngine->GetContext()->Dispatch(xDispatch, yDispatch, 1);
		}

		ID3D11ShaderResourceView* nullSRV[1] = { 0 };
		myDirectXEngine->GetContext()->CSSetShaderResources(0, 1, nullSRV);

		ID3D11UnorderedAccessView* nullUAV[1] = { 0 };
		myDirectXEngine->GetContext()->CSSetUnorderedAccessViews(0, 1, nullUAV, 0);
		myDirectXEngine->GetContext()->CSSetShader(0, 0, 0);
	}
}
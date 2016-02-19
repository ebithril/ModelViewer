#include "Engine_Precompiled.h"
#include "FullScreenHelper.h"
#include "d3dx11effect.h"
#include "Effect.h"
#include "Texture.h"
#include "EffectFactory.h"
#include "GfxFactoryWrapper.h"
#include "DirectXEngine.h"
#include "WindowsFunctions.h"
#include "../Common Utilities/EventManager.h"

namespace GraphicsEngine
{

	FullScreenHelper::FullScreenHelper()
	{
	}

	FullScreenHelper::~FullScreenHelper()
	{
	}

	bool FullScreenHelper::Init(DirectXEngine* aDirectXEngine)
	{
		myDirectXEngine = aDirectXEngine;
		myEffect = GfxFactoryWrapper::GetInstance()->GetEffectFactory()->GetEffect("PostProcessShader");
		InitVertexBuffer();
		myFadeColor = { 0, 0, 0 };
		return true;
	}

	bool FullScreenHelper::InitVertexBuffer()
	{
		myVertexDataWrapper.myType = eVertexType::VERTEX_POS_UV;
		myVertexDataWrapper.myNumberOfVertexes = 4;
		myVertexDataWrapper.myStride = sizeof(VertexPosUV);
		myVertexDataWrapper.mySize = myVertexDataWrapper.myStride * myVertexDataWrapper.myNumberOfVertexes;
		myVertexDataWrapper.myVertexData = new char[myVertexDataWrapper.mySize];

		VertexPosUV quad[4];

		quad[0].myPosition = Vector3<float>(-1.0f, 1.0f, 0.5f);
		quad[0].myUV = Vector3<float>(0.0f, 0.0f, 1.f);
		quad[1].myPosition = Vector3<float>(1.0f, 1.0f, 0.5f);
		quad[1].myUV = Vector3<float>(1.0f, 0.0f, 1.f);
		quad[2].myPosition = Vector3<float>(-1.0f, -1.0f, 0.5f);
		quad[2].myUV = Vector3<float>(0.0f, 1.0f, 1.f);
		quad[3].myPosition = Vector3<float>(1.0f, -1.0f, 0.5f);
		quad[3].myUV = Vector3<float>(1.0f, 1.0f, 1.f);

		memcpy(myVertexDataWrapper.myVertexData.get(), quad, myVertexDataWrapper.mySize);

		D3DX11_PASS_DESC passDescription;

		COMObjectPointer<ID3DX11EffectPass> pass = myEffect->GetTechnique()->GetPassByIndex(0);
		pass->GetDesc(&passDescription);

		HRESULT hr;

		D3D11_BUFFER_DESC vertexBufferDescription;
		vertexBufferDescription.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDescription.ByteWidth = myVertexDataWrapper.mySize;
		vertexBufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDescription.CPUAccessFlags = 0;
		vertexBufferDescription.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA vertexBufferInitData;
		vertexBufferInitData.pSysMem = static_cast<void*>(myVertexDataWrapper.myVertexData.get());

		hr = myDirectXEngine->GetDevice()->CreateBuffer(&vertexBufferDescription, &vertexBufferInitData, &(myVertexBufferWrapper.myVertexBuffer));

		if (FAILED(hr))
		{
			return false;
		}
		myVertexBufferWrapper.myByteOffset = 0;
		myVertexBufferWrapper.myNumberOfBuffers = 1;
		myVertexBufferWrapper.myStartSlot = 0;
		myVertexBufferWrapper.myStride = myVertexDataWrapper.myStride;

		return true;
		
	}

	void FullScreenHelper::Process(std::string aTechniqueName, Texture* aTargetTexture, Texture* aSourceTexture)
	{
		RenderQuad(aTechniqueName, aTargetTexture->GetRenderTargetView(), aTargetTexture->GetImageSize(), aSourceTexture->GetShaderView(), aSourceTexture->GetImageSize(),0);
	}

	void FullScreenHelper::Process(std::string aTechniqueName, COMObjectPointer<ID3D11RenderTargetView>& aTarget, Texture* aSourceTexture)
	{
		RenderQuad(aTechniqueName, aTarget, aSourceTexture->GetImageSize(), aSourceTexture->GetShaderView(), aSourceTexture->GetImageSize(),0);
	}

	void FullScreenHelper::Process(std::string aTechniqueName, COMObjectPointer<ID3D11RenderTargetView>& aTarget, int aWidth, int aHeight, Texture* aSourceTexture)
	{
		RenderQuad(aTechniqueName, aTarget, Vector2<int>(aWidth, aHeight), aSourceTexture->GetShaderView(), aSourceTexture->GetImageSize(),0);
	}
	void FullScreenHelper::Process(std::string aTechniqueName, COMObjectPointer<ID3D11RenderTargetView>& aTarget, int aWidth, int aHeight, Texture* aSourceTexture, float aFadeAmount)
	{
		RenderQuad(aTechniqueName, aTarget, Vector2<int>(aWidth, aHeight), aSourceTexture->GetShaderView(), aSourceTexture->GetImageSize(), aFadeAmount);

	}


	void FullScreenHelper::RenderQuad(std::string aTechniqueName, COMObjectPointer<ID3D11RenderTargetView>& aTarget, const Vector2<int>& aTargetSize, COMObjectPointer<ID3D11ShaderResourceView>& aSource, const Vector2<int>& aSourceSize, float aFadeAmount)
	{
		D3D11_VIEWPORT oldViewPort[D3D11_VIEWPORT_AND_SCISSORRECT_MAX_INDEX];
		unsigned int numberOfViewPorts = 1;
		myDirectXEngine->GetContext()->RSGetViewports(&numberOfViewPorts, oldViewPort);

		D3D11_VIEWPORT viewport;
		viewport.Width = (float)aTargetSize.x;
		viewport.Height = (float)aTargetSize.y;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;

		myDirectXEngine->GetContext()->RSSetViewports(1, &viewport);

		myDirectXEngine->GetContext()->IASetInputLayout(myEffect->GetInputLayout().Get());
		myDirectXEngine->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		myDirectXEngine->GetContext()->IASetVertexBuffers(myVertexBufferWrapper.myStartSlot, myVertexBufferWrapper.myNumberOfBuffers, &(myVertexBufferWrapper.myVertexBuffer), &myVertexBufferWrapper.myStride, &myVertexBufferWrapper.myByteOffset);

		Vector2<int> invTargetSize(1 / aTargetSize.x, 1 / aTargetSize.y);
		Vector2<int> invSourceSize(1 / aSourceSize.x, 1 / aSourceSize.y);
		float deltaTime = CU::EventManager::GetInstance()->GetDeltaTime();
		float exposure = 0.5f;

		myDirectXEngine->GetContext()->OMSetRenderTargets(1, &aTarget, NULL);
		myEffect->GetEffect()->GetVariableByName("DiffuseTexture")->AsShaderResource()->SetResource(aSource.Get());
		myEffect->GetEffect()->GetVariableByName("InvTargetSize")->AsVector()->SetRawValue(&invTargetSize, 0, sizeof(Vector2<int>));
		myEffect->GetEffect()->GetVariableByName("InvSourceSize")->AsVector()->SetRawValue(&invSourceSize, 0, sizeof(Vector2<int>));
		myEffect->GetEffect()->GetVariableByName("DeltaTime")->AsScalar()->SetRawValue(&deltaTime, 0, sizeof(float));
		myEffect->GetEffect()->GetVariableByName("Exposure")->AsScalar()->SetRawValue(&exposure, 0, sizeof(float));
		myEffect->GetEffect()->GetVariableByName("FadeAmount")->AsScalar()->SetRawValue(&aFadeAmount, 0, sizeof(float));
		myEffect->GetEffect()->GetVariableByName("FadeColor")->AsVector()->SetRawValue(&myFadeColor, 0, sizeof(Vector3<float>));



		D3DX11_TECHNIQUE_DESC techDesc;
		COMObjectPointer<ID3DX11EffectTechnique> tech = myEffect->GetNamedTechnique(aTechniqueName);
		HRESULT result = tech->GetDesc(&techDesc);

		if (FAILED(result))
		{
			std::string errorText = "Failed to get post process technique named: ";
			errorText += aTechniqueName;
			errorText += ".";

			WF::AssertComAndWindowsError(result, errorText.c_str());
		}

		for (UINT p = 0; p < techDesc.Passes; ++p)
		{
			COMObjectPointer<ID3DX11EffectPass> pass = tech->GetPassByIndex(p);
			pass->Apply(0, myDirectXEngine->GetContext().Get());
			myDirectXEngine->GetContext()->Draw(4, 0);
		}

		myEffect->GetEffect()->GetVariableByName("DiffuseTexture")->AsShaderResource()->SetResource(NULL);
		myEffect->GetEffect()->GetVariableByName("SecondaryDiffuseTexture")->AsShaderResource()->SetResource(NULL);
		myEffect->GetEffect()->GetVariableByName("TertiaryDiffuseTexture")->AsShaderResource()->SetResource(NULL);
		myEffect->GetEffect()->GetVariableByName("QuartaryDiffuseTexture")->AsShaderResource()->SetResource(NULL);
		COMObjectPointer<ID3DX11EffectPass> pass = tech->GetPassByIndex(0);
		pass->Apply(0, myDirectXEngine->GetContext().Get()),

		myDirectXEngine->GetContext()->RSSetViewports(numberOfViewPorts, oldViewPort);
	}

	Effect* FullScreenHelper::GetEffect()
	{
		return myEffect;
	}

	void FullScreenHelper::SetFadeColor(const Vector3<float> aFadeColor)
	{
		myFadeColor = aFadeColor;
	}

}
#include "Engine_Precompiled.h"
#include "Effect.h"
#include "d3dx11effect.h"
#include "WindowsFunctions.h"
#include "../Common Utilities/DL_Debug.h"
#include "../Common Utilities/GrowingArray.h"
#include "../Common Utilities/Event.h"
#include "VertexDataWrapper.h"
#include "Engine.h"
#include "SetupInfo.h"
#include "Texture.h"
#include "ConstantBuffers.h"
#include <d3d11.h>
#include "DirectXEngine.h"
#include "GfxFactoryWrapper.h"
#include "TextureFactory.h"

namespace GraphicsEngine
{
	void Effect::SetTechnique(const std::string& aName)
	{
		myTechnique = GetNamedTechnique(aName);
	}

	COMObjectPointer<ID3DX11Effect>& Effect::GetEffect()
	{
		return myEffect;
	}

	COMObjectPointer<ID3DX11EffectTechnique>& Effect::GetTechnique()
	{
		return myTechnique;
	}

	ID3DX11EffectTechnique* Effect::GetNamedTechnique(const std::string& aName)
	{
		return myEffect->GetTechniqueByName(aName.c_str());
	}

	COMObjectPointer<ID3D11InputLayout>& Effect::GetInputLayout()
	{
		return myInputLayout;
	}

	bool Effect::Init(const std::string& anEffectFile, eVertexType aVertexType, COMObjectPointer<ID3D11Device>& aDevice)
	{
		InitOnFileChange(anEffectFile);
		ObserveEvent(CU::eEvent::RESIZE, HANDLE_EVENT_FUNCTION(HandleOnResizeEvent));
		ObserveEvent(CU::eEvent::NEW_FRAME, HANDLE_EVENT_FUNCTION(HandleNewFrameEvent));
		myDevice = aDevice;
		myVertexType = aVertexType;
		
		InitBuffers();
		ReInit();
		return true;
	}

	void Effect::PerObjectUpdate(const PerObjectDataBuffer& aObjectDataBuffer)
	{
		UpdateShaderResource(myObjectDataBuffer, &aObjectDataBuffer, "PerObjectDataBuffer");	
	}

	void Effect::PerFrameUpdate(const PerFrameCameraBuffer& aCameraBuffer, const PerFrameLightBuffer& aLightBuffer)
	{
		myNeedNewFrameUpdate = false;

		UpdateShaderResource(myCameraBuffer, &aCameraBuffer, "PerFrameCameraBuffer");

		UpdateShaderResource(myDirectionalLightBuffer, &aLightBuffer, "PerFrameLightBuffer");

		COMObjectPointer<ID3DX11EffectShaderResourceVariable> shadowTexture = myEffect->GetVariableByName("DirectionalShadowMap")->AsShaderResource();
		
		if (shadowTexture->IsValid() == false)
		{
			DL_ASSERT("Failed to set shader texture resource.");
		}

		HRESULT result = shadowTexture->SetResource(Engine::GetInstance()->GetEngine()->GetShadowManager().GetDirectionalShadowMap().GetShaderView().Get());
		if (FAILED(result) == true)
		{
			WF::AssertComAndWindowsError(result, "Failed to set shader texture resource.");
		}

	}

	void Effect::CreateBuffer(COMObjectPointer<ID3D11Buffer>& aBuffer, unsigned int aSizeInBytes)
	{
		D3D11_BUFFER_DESC bufferDescription{ 0 };
		bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDescription.ByteWidth = aSizeInBytes;
		bufferDescription.CPUAccessFlags = 0;
		bufferDescription.Usage = D3D11_USAGE_DEFAULT;

		HRESULT result = myDevice->CreateBuffer(&bufferDescription, nullptr, &aBuffer);
		if (FAILED(result))
		{
			WF::AssertComAndWindowsError(result, "Failed to create Buffer");
		}
	}

	Effect::Effect()
	{
		myNeedNewFrameUpdate = true;
		myNeedOnResizeUpdate = true;
		myIsUsingAlpha = false;
		myEffect = nullptr;
		myDevice = nullptr;
		myTechnique = nullptr;
		myCameraBuffer = nullptr;
		myInputLayout = nullptr;
	}

	Effect::~Effect()
	{
		int a = 0;
		a;
	}

	bool Effect::HandleNewFrameEvent(const CU::PoolPointer<CU::Event>& anEvent)
	{
		anEvent;
		myNeedNewFrameUpdate = true;
		return true;
	}

	bool Effect::HandleOnResizeEvent(const CU::PoolPointer<CU::Event>& anEvent)
	{
		anEvent;
		myNeedOnResizeUpdate = true;
		return true;
	}

	bool Effect::NeedNewFrameUpdate()
	{
		return myNeedNewFrameUpdate;
	}

	bool Effect::NeedNewOnResizeUpdate()
	{
		return myNeedOnResizeUpdate;
	}

	void Effect::OnResizeUpdate(const OnResizeBuffer& aResizeBuffer)
	{
		myNeedOnResizeUpdate = false;

		COMObjectPointer<ID3D11DeviceContext> context;
		myDevice->GetImmediateContext(&context);

		context->UpdateSubresource(myOnResizeBuffer.Get(), 0, nullptr, &aResizeBuffer, 0, 0);

		COMObjectPointer<ID3DX11EffectConstantBuffer> onResizeConstantBuffer = myEffect->GetConstantBufferByName("OnResizeBuffer");
		if (onResizeConstantBuffer->IsValid() == false)
		{
			DL_ASSERT("Failed to get constant buffer");
		}
		HRESULT result = onResizeConstantBuffer->SetConstantBuffer(myOnResizeBuffer.Get());

		if (FAILED(result) == true)
		{
			WF::AssertComAndWindowsError(result, "Failed to set constant buffer");
		}
	}

	void Effect::SetTextures(CU::StaticArray<Texture*, static_cast<int>(eTextureType::NR_OF_TEXTURETYPES)>& someTextures)
	{
		for (unsigned short i = 0; i < static_cast<unsigned short>(eTextureType::NR_OF_TEXTURETYPES); i++)
		{
			Texture* currentTexture = someTextures[i];
			
			if (currentTexture != nullptr)
			{
				if (myShaderViews[i]->IsValid() == true)
				{
					HRESULT result = myShaderViews[i]->SetResource(currentTexture->GetShaderView().Get());
					if (FAILED(result) == true)
					{
						WF::AssertComAndWindowsError(result, "Failed to set shader texture resource.");
					}

				}
			}
		}
	}

	void Effect::UpdateShaderResource(COMObjectPointer<ID3D11Buffer>& someBuffer, const void* someBufferData, const char* aBufferName)
	{
		COMObjectPointer<ID3D11DeviceContext> context;
		myDevice->GetImmediateContext(&context);

		context->UpdateSubresource(someBuffer.Get(), 0, nullptr, someBufferData, 0, 0);

		COMObjectPointer<ID3DX11EffectConstantBuffer> constantBufferVariable = myEffect->GetConstantBufferByName(aBufferName);
		if (constantBufferVariable->IsValid() == false)
		{
			return;
			//DL_ASSERT("Failed to get constant buffer");
		}
		HRESULT result = constantBufferVariable->SetConstantBuffer(someBuffer.Get());

		if (FAILED(result) == true)
		{
			WF::AssertComAndWindowsError(result, "Failed to set constant buffer");
		}
	}

	void Effect::SetCubeMap(Texture* aCubeMap)
	{
		COMObjectPointer<ID3DX11EffectShaderResourceVariable> cubeMap = myEffect->GetVariableByName("AmbientCubeMap")->AsShaderResource();

		HRESULT result = cubeMap->SetResource(aCubeMap->GetShaderView().Get());
		if (FAILED(result) == true)
		{
			WF::AssertComAndWindowsError(result, "Failed to set shader texture resource.");
		}
	}

	void Effect::SetTextTexture(COMObjectPointer<ID3D11ShaderResourceView>& aView)
	{
		COMObjectPointer<ID3DX11EffectShaderResourceVariable> textTexture = myEffect->GetVariableByName("TextTexture")->AsShaderResource();

		HRESULT result = textTexture->SetResource(aView.Get());
		if (FAILED(result) == true)
		{
			WF::AssertComAndWindowsError(result, "Failed to set shader texture resource.");
		}
	}

	void Effect::PerObjectShadowUpdate(const PerObjectShadowBuffer& aBuffer)
	{
		UpdateShaderResource(myObjectShadowBuffer, &aBuffer, "PerObjectShadowBuffer");
	}

	void Effect::SetIsUsingAlpha(bool aTruth)
	{
		myIsUsingAlpha = aTruth;
	}

	bool Effect::GetIsUsingAlpha() const
	{
		return myIsUsingAlpha;
	}

	void Effect::OnFileChange(const std::string& aFileName)
	{
		aFileName;
		ReInit();
	}

	bool Effect::ReInit()
	{
		std::wstring path = WF::WideStringConvertion(myFileName.c_str());
		HRESULT result = -1;
		while (result != 0)
		{
		result = D3DX11CreateEffectFromFile(path.c_str(), NULL, myDevice.Get(), &myEffect);

		if (FAILED(result) == true)
		{
			//std::string errorMessage = "Failed to create effect from file with path: " + myFileName;
		//	WF::AssertComAndWindowsError(result, errorMessage.c_str());
		}

		}

		myTechnique = myEffect->GetTechniqueByIndex(0);

		if (myTechnique->IsValid() == false)
		{
			DL_DEBUG("Technique in effect not valid");
		}

		COMObjectPointer<ID3DX11EffectPass> effectPass = myTechnique->GetPassByIndex(0);
		if (effectPass->IsValid() == false)
		{
			DL_DEBUG("EffectPass in effect not valid");
		}

		D3DX11_PASS_SHADER_DESC effectVsDesc;
		effectPass->GetVertexShaderDesc(&effectVsDesc);
		D3DX11_EFFECT_SHADER_DESC effectVsDesc2;
		effectVsDesc.pShaderVariable->GetShaderDesc(effectVsDesc.ShaderIndex, &effectVsDesc2);
		const void *vsCodePtr = effectVsDesc2.pBytecode;
		unsigned vsCodeLen = effectVsDesc2.BytecodeLength;

		CU::GrowingArray<D3D11_INPUT_ELEMENT_DESC> myInputDescription(4);

		switch (myVertexType)
		{
		case NONE:
			break;

		case VERTEX_POS_COL:
			myInputDescription.Add({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
			myInputDescription.Add({ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 });
			break;

		case VERTEX_POS_COL_UV:
			break;

		case VERTEX_POS_NORM_UV:
			myInputDescription.Add({ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
			myInputDescription.Add({ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
			myInputDescription.Add({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
			break;

		case VERTEX_POS_NORM_UV_TAN:
			DL_ASSERT("Failed to create input layout for effect.");
			break;

		case VERTEX_POS_UV:
			myInputDescription.Add({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
			myInputDescription.Add({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 });
			break;

		case VERTEX_POS_WEIGHT_BONES_NORM_UV_TAN:
			DL_ASSERT("Failed to create input layout for effect.");
			break;

		case VERTEX_POS_NORM_UV_BI_NORMAL_TAN:
			myInputDescription.Add({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
			myInputDescription.Add({ "PSIZE", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
			myInputDescription.Add({ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
			myInputDescription.Add({ "PSIZE", 1, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
			myInputDescription.Add({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
			myInputDescription.Add({ "TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
			myInputDescription.Add({ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
			myInputDescription.Add({ "PSIZE", 2, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
			myInputDescription.Add({ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
			myInputDescription.Add({ "PSIZE", 3, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
			myInputDescription.Add({ "WEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
			myInputDescription.Add({ "BONES", 0, DXGI_FORMAT_R32G32B32A32_SINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
			// Add animation stuff here

			break;

		case VERTEX_POS_VELOCITY_SIZE_ALPHA_TIME:
			myInputDescription.Add({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
			myInputDescription.Add({ "TEXCOORD", 0, DXGI_FORMAT_R32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 });
			myInputDescription.Add({ "TEXCOORD", 1, DXGI_FORMAT_R32G32B32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 });
			myInputDescription.Add({ "TEXCOORD", 2, DXGI_FORMAT_R32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 });
			myInputDescription.Add({ "PSIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 });
			myInputDescription.Add({ "TEXCOORD", 3, DXGI_FORMAT_R32_FLOAT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 });
			break;

		case VERTEX_POS_SCALE_ALPHA_TOTALTIME:
			myInputDescription.Add({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
			myInputDescription.Add({ "TEXCOORD", 0, DXGI_FORMAT_R32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 });
			myInputDescription.Add({ "TEXCOORD", 1, DXGI_FORMAT_R32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 });
			myInputDescription.Add({ "TEXCOORD", 2, DXGI_FORMAT_R32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 });
			break;

		case VERTEX_TERRAIN_POS_NORM_UV_BINORM_TAN:
			myInputDescription.Add({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
			myInputDescription.Add({ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
			myInputDescription.Add({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
			myInputDescription.Add({ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
			myInputDescription.Add({ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
			break;

		case VERTEX_SPRITE_POS_COL_UV:
			myInputDescription.Add({ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
			myInputDescription.Add({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
			myInputDescription.Add({ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
			break;

		default:
			DL_ASSERT("Failed to create input layout for effect.");
			break;
		}

		if (myVertexType != eVertexType::NONE)
		{
			result = myDevice->CreateInputLayout(
				&myInputDescription[0], myInputDescription.Size(), vsCodePtr, vsCodeLen, &myInputLayout);

			if (FAILED(result) == true)
			{
				WF::AssertComAndWindowsError(result, "Failed to create input layout for effect.");
			}

			for (int i = 0; i < static_cast<int>(eTextureType::NR_OF_TEXTURETYPES); i++)
			{
				std::string variableName = GetVariableName(static_cast<eTextureType>(i));
				myShaderViews[i] = myEffect->GetVariableByName(variableName.c_str())->AsShaderResource();
			}
		}
		return true;
	}

	void Effect::InitBuffers()
	{
		CreateBuffer(myObjectDataBuffer, sizeof(PerObjectDataBuffer));
		CreateBuffer(myCameraBuffer, sizeof(PerFrameCameraBuffer));
		CreateBuffer(myDirectionalLightBuffer, sizeof(PerFrameLightBuffer));
		CreateBuffer(myOnResizeBuffer, sizeof(OnResizeBuffer));
		CreateBuffer(myObjectShadowBuffer, sizeof(PerObjectShadowBuffer));
		CreateBuffer(myPerObjectLightBuffer, sizeof(PerObjectLightBuffer));
	}

	void Effect::PerObjectLightBufferUpdate(const PerObjectLightBuffer& aBuffer)
	{
		UpdateShaderResource(myPerObjectLightBuffer, &aBuffer, "PerObjectLightBuffer");
	}

	void Effect::SetIsUsingOneBitAlpha(bool aTruth)
	{
		myIsUsingOneBitAlpha = aTruth;
	}

	bool Effect::GetIsUsingOneBitAlpha() const
	{
		return myIsUsingOneBitAlpha;
	}

}
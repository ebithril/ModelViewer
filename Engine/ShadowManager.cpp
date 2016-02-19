#include "Engine_Precompiled.h"
#include "ShadowManager.h"
#include "Engine.h"
#include "d3dx11effect.h"
#include "LightBuffer.h"
#include "../Common Utilities/SceneRenderCommands.h"
#include "DirectXEngine.h"
#include "GfxFactoryWrapper.h"
#include "EffectFactory.h"
#include "Effect.h"
#include "../Common Utilities/Matrix44.h"
#include "VertexDataWrapper.h"
#include "WindowsFunctions.h"
#include "Texture.h"


namespace GraphicsEngine
{

	ShadowManager::ShadowManager()
	{
	}

	ShadowManager::~ShadowManager()
	{

	}

	void ShadowManager::Init()
	{
		myShadowMapEffect = nullptr;
		myTerrainShadowMapEffect = nullptr;
		myAlphaShadowMapEffect = nullptr;
		DirectXEngine* dxEngine = GE::Engine::GetInstance()->GetEngine();
		myDirectionalShadowMap = new Texture(dxEngine, Vector2<int>(4096, 4096), DXGI_FORMAT::DXGI_FORMAT_R32_TYPELESS, true, false, false, true);
	}

	void ShadowManager::Destroy()
	{
		SAFE_DELETE(myDirectionalShadowMap);
	}

	void ShadowManager::DrawShadowMaps(CU::SceneRenderCommands& someData)
	{
		if (myShadowMapEffect == nullptr)
		{
			myShadowMapEffect = GfxFactoryWrapper::GetInstance()->GetEffectFactory()->CreateEffect("ShadowMap", "Data/Shaders/ShadowMap.fso", GE::eVertexType::VERTEX_POS_NORM_UV_BI_NORMAL_TAN);
			myTerrainShadowMapEffect = GfxFactoryWrapper::GetInstance()->GetEffectFactory()->CreateEffect("TerrainShadowMap", "Data/Shaders/TerrainShadowMap.fso", GE::eVertexType::VERTEX_TERRAIN_POS_NORM_UV_BINORM_TAN);
			myAlphaShadowMapEffect = GfxFactoryWrapper::GetInstance()->GetEffectFactory()->CreateEffect("ShadowMapAlpha", "Data/Shaders/ShadowMapAlpha.fso", GE::eVertexType::VERTEX_POS_NORM_UV_BI_NORMAL_TAN, true);
		}
		DrawDirectionalShadowMap(someData);
	}

	Texture& ShadowManager::GetDirectionalShadowMap()
	{
		return *myDirectionalShadowMap;
	}

	void ShadowManager::DrawDirectionalShadowMap(CU::SceneRenderCommands& someData)
	{

		LightBuffer* lightBuffer = someData.mySceneData->myLightBuffer;
		if (lightBuffer->IsFirstFrame() == true)
		{
			lightBuffer->SetIsFirstFrame(false);
			return;
		}

		lightBuffer->GetReadDirectionalLightShadowBuffer();

		DirectXEngine* engine = Engine::GetInstance()->GetEngine();

		D3D11_VIEWPORT oldViewport;
		UINT viewportCount = 1;
		engine->GetContext()->RSGetViewports(&viewportCount, &oldViewport);

		D3D11_VIEWPORT newViewport;

		newViewport.TopLeftX = 0.f;
		newViewport.TopLeftY = 0.f;
		newViewport.Width = static_cast<float>(myDirectionalShadowMap->GetImageSize().x);
		newViewport.Height = static_cast<float>(myDirectionalShadowMap->GetImageSize().y);
		newViewport.MinDepth = 0.f;
		newViewport.MaxDepth = 1.f;

		engine->GetContext()->RSSetViewports(1, &newViewport);

		engine->GetContext()->ClearDepthStencilView(myDirectionalShadowMap->GetDepthStencilView().Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
		engine->GetContext()->OMSetRenderTargets(0, NULL, myDirectionalShadowMap->GetDepthStencilView().Get());
		engine->GetContext()->IASetInputLayout(myShadowMapEffect->GetInputLayout().Get());
		engine->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		COMObjectPointer<ID3DX11EffectMatrixVariable> lightViewProjectionMatrixVariable = myShadowMapEffect->GetEffect()->GetVariableByName("myLightViewProjectionMatrix")->AsMatrix();

		if (lightViewProjectionMatrixVariable->IsValid() == false)
		{
			DL_ASSERT("Effect lightViewProjectionMatrixVariable variable not valid");
		}

		COMObjectPointer<ID3DX11EffectMatrixVariable> lightViewProjectionMatrixVariableTerrain = myTerrainShadowMapEffect->GetEffect()->GetVariableByName("myLightViewProjectionMatrix")->AsMatrix();

		if (lightViewProjectionMatrixVariableTerrain->IsValid() == false)
		{
			DL_ASSERT("Effect lightViewProjectionMatrixVariable variable not valid");
		}

		COMObjectPointer<ID3DX11EffectMatrixVariable> lightViewProjectionMatrixVariableAlpha = myAlphaShadowMapEffect->GetEffect()->GetVariableByName("myLightViewProjectionMatrix")->AsMatrix();

		if (lightViewProjectionMatrixVariableAlpha->IsValid() == false)
		{
			DL_ASSERT("Effect lightViewProjectionMatrixVariable variable not valid");
		}

		Matrix44f lightProjection = lightBuffer->GetReadDirectionalLightShadowBuffer().GetViewProjection();

		lightViewProjectionMatrixVariable->SetMatrix(&lightProjection.myMatrix[0]);
		lightViewProjectionMatrixVariableTerrain->SetMatrix(&lightProjection.myMatrix[0]);
		lightViewProjectionMatrixVariableAlpha->SetMatrix(&lightProjection.myMatrix[0]);

		const CU::GrowingArray<GE::ShadowRenderData>& someRenderData = lightBuffer->GetReadDirectionalLightShadowBuffer().GetShadowRenderData();

		for (unsigned short i = 0; i < someRenderData.Size(); i++)
		{
			switch (someRenderData[i].myEffectType)
			{
			case eEffectType::NORMAL:
				someRenderData[i].myInstance->RenderShadowData(myShadowMapEffect, someRenderData[i].myOrientation);
				break;

			case eEffectType::TERRAIN:
				engine->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
				engine->GetContext()->IASetInputLayout(myTerrainShadowMapEffect->GetInputLayout().Get());
				someRenderData[i].myInstance->RenderShadowData(myTerrainShadowMapEffect, someRenderData[i].myOrientation);
				engine->GetContext()->IASetInputLayout(myShadowMapEffect->GetInputLayout().Get());
				engine->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				break;

			case eEffectType::ONE_BIT_ALPHA:
			{

				COMObjectPointer<ID3DX11EffectShaderResourceVariable> diffuseVariable = myAlphaShadowMapEffect->GetEffect()->GetVariableByName("DiffuseTexture")->AsShaderResource();

				if (diffuseVariable->IsValid() == true)
				{
					auto originalTexture = someRenderData[i].myInstance->GetOriginalDiffuse();
					
					if (originalTexture != nullptr)
					{
						auto texture = originalTexture->GetShaderView().Get();
						HRESULT result = diffuseVariable->SetResource(texture);
						if (FAILED(result) == true)
						{
							WF::AssertComAndWindowsError(result, "Failed to set shader texture resource.");
						}

						engine->GetContext()->IASetInputLayout(myAlphaShadowMapEffect->GetInputLayout().Get());
						someRenderData[i].myInstance->RenderShadowData(myAlphaShadowMapEffect, someRenderData[i].myOrientation);
						engine->GetContext()->IASetInputLayout(myShadowMapEffect->GetInputLayout().Get());
					}
				}
			}
				break;

			default:
				break;
			}
		}
		engine->GetContext()->RSSetViewports(1, &oldViewport);
	}

}

#include "Engine_Precompiled.h"
#include "TextureMerger.h"
#include "Engine.h"
#include "DirectXEngine.h"
#include "Texture.h"
#include "d3dx11effect.h"
#include "Effect.h"

namespace GraphicsEngine
{
	TextureMerger* TextureMerger::ourInstance = nullptr;

	TextureMerger::TextureMerger()
	{
	}


	TextureMerger::~TextureMerger()
	{
	}

	void TextureMerger::Create(Effect* anEffect)
	{
		if (ourInstance != nullptr)
		{
			DL_ASSERT("Texture Merger already created.");
		}
		ourInstance = new TextureMerger();
		ourInstance->Init(anEffect);
	}
	TextureMerger* TextureMerger::GetInstance()
	{
		return ourInstance;
	}

	void TextureMerger::Init(Effect* anEffect)
	{
		myEffect = anEffect;

		myTextureVertices[0].myPosition = Vector3<float>(-1, -1, 0.f);
		myTextureVertices[0].myUV = Vector2<float>(0, 1);

		myTextureVertices[1].myPosition = Vector3<float>(-1, 1, 0.f);
		myTextureVertices[1].myUV = Vector2<float>(0, 0);

		myTextureVertices[2].myPosition = Vector3<float>(1, 1, 0.f);
		myTextureVertices[2].myUV = Vector2<float>(1, 0);

		myTextureVertices[3].myPosition = Vector3<float>(1, 1, 0.f);
		myTextureVertices[3].myUV = Vector2<float>(1, 0);

		myTextureVertices[4].myPosition = Vector3<float>(1, -1, 0.f);
		myTextureVertices[4].myUV = Vector2<float>(1, 1);

		myTextureVertices[5].myPosition = Vector3<float>(-1, -1, 0.f);
		myTextureVertices[5].myUV = Vector2<float>(0, 1);

		myVertexData.myStride = sizeof(TextureVertex);
		myVertexData.mySize = sizeof(TextureVertex) * 6;
		myVertexData.myNumberOfVertexes = 6;
		myVertexData.myVertexData = reinterpret_cast<char*>(myTextureVertices);
		myVertexBuffer.Create(Engine::GetInstance()->GetEngine()->GetDevice(), &myVertexData);

		D3D11_INPUT_ELEMENT_DESC layout1[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		D3D11_INPUT_ELEMENT_DESC* layout = layout1;
		UINT numElements = ARRAYSIZE(layout1);

		ID3DX11EffectPass* effectPass = myEffect->GetTechnique()->GetPassByIndex(0);

		D3DX11_PASS_SHADER_DESC effectVsDesc;
		effectPass->GetVertexShaderDesc(&effectVsDesc);
		D3DX11_EFFECT_SHADER_DESC effectVsDesc2;
		effectVsDesc.pShaderVariable->GetShaderDesc(effectVsDesc.ShaderIndex, &effectVsDesc2);

		const void* vsCodePtr = effectVsDesc2.pBytecode;
		unsigned vsCodeLen = effectVsDesc2.BytecodeLength;

		HRESULT hr = Engine::GetInstance()->GetEngine()->GetDevice()->CreateInputLayout(layout, numElements, vsCodePtr, vsCodeLen, &myVertexLayout);
		if (FAILED(hr) == true)
		{
			//MessageBoxA(0, "2", "error", 0);
		}

		ID3D11VertexShader* g_pVertexShader = 0;
		HRESULT rezz = Engine::GetInstance()->GetEngine()->GetDevice()->CreateVertexShader(vsCodePtr, vsCodeLen, nullptr, &g_pVertexShader);
		rezz;


		// Set the input layout
		Engine::GetInstance()->GetEngine()->GetContext()->IASetInputLayout(myVertexLayout);
	}

	void TextureMerger::MergeTextures(Texture* aDestination, Texture* aFirstTexture, Texture* aSecondTexture, std::string aTechnique)
	{
		DirectXEngine* theEngine = Engine::GetInstance()->GetEngine();

		Vector2<int> theTexDimensions = aDestination->GetImageSize();

		D3D11_VIEWPORT oldViewPort[D3D11_VIEWPORT_AND_SCISSORRECT_MAX_INDEX];
		unsigned int numberOfViewPorts = 1;
		theEngine->GetContext()->RSGetViewports(&numberOfViewPorts, oldViewPort);

		D3D11_VIEWPORT viewport;
		viewport.Width = (float)theTexDimensions.x;
		viewport.Height = (float)theTexDimensions.y;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		theEngine->GetContext()->RSSetViewports(1, &viewport);

		theEngine->GetContext()->IASetInputLayout(myVertexLayout);
		theEngine->GetContext()->IASetVertexBuffers(0, 1, &myVertexBuffer.myVertexBuffer, &myVertexBuffer.myStride, &myVertexBuffer.myByteOffset);
		theEngine->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		ID3D11RenderTargetView* theTarget = aDestination->GetRenderTargetView().Get();
		theEngine->GetContext()->OMSetRenderTargets(1, &theTarget, NULL);

		D3DX11_TECHNIQUE_DESC techDesc;
		myEffect->GetNamedTechnique(aTechnique)->GetDesc(&techDesc);

		for (UINT p = 0; p < techDesc.Passes; ++p)
		{
			ID3DX11EffectShaderResourceVariable* shaderVar = myEffect->GetEffect()->GetVariableByName("DiffuseTexture")->AsShaderResource();
			HRESULT retz = shaderVar->SetResource(aFirstTexture->GetShaderView().Get());

			if (FAILED(retz))
			{
				DL_ASSERT("Failed to set texture shader resource");
			}

			ID3DX11EffectShaderResourceVariable* shaderVar2 = myEffect->GetEffect()->GetVariableByName("SecondaryDiffuseTexture")->AsShaderResource();
			retz = shaderVar2->SetResource(aSecondTexture->GetShaderView().Get());

			if (FAILED(retz))
			{
				DL_ASSERT("Failed to set texture shader resource");
			}

			HRESULT res = myEffect->GetNamedTechnique(aTechnique)->GetPassByIndex(p)->Apply(0, theEngine->GetContext().Get());
			if (FAILED(res) == true)
			{
				DL_ASSERT("Failed To Merge Textures ,Runtime Error!");
			}
			theEngine->GetContext()->Draw(6, 0);
		}

		theEngine->GetContext()->RSSetViewports(numberOfViewPorts, oldViewPort);
		//theEngine->ResetTargetView();
	}
}

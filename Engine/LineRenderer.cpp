#include "Engine_Precompiled.h"
#include "LineRenderer.h"
#include "../Common Utilities/RenderCommandLine.h"
#include "d3dx11effect.h"
#include "DirectXEngine.h"
#include "WindowsFunctions.h"
#include "VertexDataWrapper.h"
#include "Camera.h"
#include <d3d11.h>
#include "COMObjectPointer.h"

namespace GraphicsEngine
{
	LineRenderer::LineRenderer()
	{
	}

	LineRenderer::~LineRenderer()
	{
	}

	void LineRenderer::DrawLines(CU::GrowingArray<CU::RenderCommandLine, unsigned int>& someLines)
	{
		if (someLines.Size() > 0)
		{
			D3D11_MAPPED_SUBRESOURCE resource;
			ZeroMemory(&resource, sizeof(resource));
			myEngine->GetContext()->Map(myLineDummyBuffer.myVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

			memcpy(resource.pData, reinterpret_cast<void*>(&(someLines[0].myFirstPoint.x)), someLines.Size() * sizeof(float) * 8);

			myEngine->GetContext()->Unmap(myLineDummyBuffer.myVertexBuffer.Get(), 0);
			myEngine->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
			myEngine->GetContext()->IASetVertexBuffers(0, 1, &myLineDummyBuffer.myVertexBuffer, &myLineDummyBuffer.myStride, &myLineDummyBuffer.myByteOffset);
			myEngine->GetContext()->IASetInputLayout(myLineInputLayout.Get());

			COMObjectPointer<ID3DX11EffectPass> pass = myLineTechnique->GetPassByIndex(0);

			if (pass->IsValid() == false)
			{
				DL_ASSERT("Pass invalid");
			}
			HRESULT passResult = pass->Apply(NULL, myEngine->GetContext().Get());
			if (FAILED(passResult))
			{
				DL_ASSERT("Pass failed.");
			}
			myEngine->GetContext()->Draw(someLines.Size() * 2, 0);
		}
	}

	void LineRenderer::Init(DirectXEngine* anEngine)
	{
		myEngine = anEngine;

		LineSetup();
	}

	void LineRenderer::LineSetup()
	{

		std::wstring path = WF::WideStringConvertion("Data\\Shaders\\Line.fso");
		HRESULT result = D3DX11CreateEffectFromFile(path.c_str(), NULL, myEngine->GetDevice().Get(), &myLineEffect);

		if (FAILED(result) == true)
		{
			std::string errorMessage = "Failed to create effect from file with path: Data\\Shaders\\Line.fso";
			WF::AssertComAndWindowsError(result, errorMessage.c_str());
		}

		myLineTechnique = myLineEffect->GetTechniqueByIndex(0);

		if (myLineTechnique->IsValid() == false)
		{
			DL_DEBUG("Technique in effect not valid");
		}

		COMObjectPointer<ID3DX11EffectPass> effectPass = myLineTechnique->GetPassByIndex(0);
		if (effectPass->IsValid() == false)
		{
			DL_DEBUG("EffectPass in effect not valid");
		}

		myLineProjectionMatrix = myLineEffect->GetVariableByName("Projection")->AsMatrix();
		myLineCameraMatrix = myLineEffect->GetVariableByName("Camera")->AsMatrix();

		D3DX11_PASS_SHADER_DESC effectVsDesc;
		effectPass->GetVertexShaderDesc(&effectVsDesc);
		D3DX11_EFFECT_SHADER_DESC effectVsDesc2;
		effectVsDesc.pShaderVariable->GetShaderDesc(effectVsDesc.ShaderIndex, &effectVsDesc2);
		const void *vsCodePtr = effectVsDesc2.pBytecode;
		unsigned vsCodeLen = effectVsDesc2.BytecodeLength;

		CU::GrowingArray<D3D11_INPUT_ELEMENT_DESC> inputDescription(4);

		inputDescription.Add({ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });


		result = myEngine->GetDevice()->CreateInputLayout(&inputDescription[0], inputDescription.Size(), vsCodePtr, vsCodeLen, &myLineInputLayout);

		if (FAILED(result) == true)
		{
			WF::AssertComAndWindowsError(result, "Input layout error");
		}

		VertexDataWrapper vertexData;

		vertexData.myNumberOfVertexes = 10000;
		vertexData.mySize = sizeof(float) * 4 * vertexData.myNumberOfVertexes;
		vertexData.myVertexData = new char[vertexData.mySize];

		vertexData.myStride = 16;

		myLineDummyBuffer.CreateDynamic(myEngine->GetDevice(), &vertexData);


		if (FAILED(result) == true)
		{
			WF::AssertComAndWindowsError(result, "Failed to create input layout for effect.");
		}
	}

	void LineRenderer::UpdateLineShader(const Camera& aCamera)
	{
		Matrix44f projectionMatrix = aCamera.GetProjection();
		Matrix44f inverseCameraMatrix = aCamera.GetOrientation().GetInverse();

		myLineProjectionMatrix->SetMatrix(&projectionMatrix.myMatrix[0]);
		myLineCameraMatrix->SetMatrix(&inverseCameraMatrix.myMatrix[0]);
	}
}


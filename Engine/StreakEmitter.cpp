#include "Engine_Precompiled.h"
#include "StreakEmitter.h"
#include "StreakEmitterData.h"
#include "../Common Utilities/EventManager.h"
#include "Effect.h"
#include "d3dx11effect.h"
#include "WindowsFunctions.h"
#include "Camera.h"
#include "Texture.h"
#include "StreakData.h"
#include "../Common Utilities/Macros.h"
#include "VertexDataWrapper.h"
#include "COMObjectPointer.h"
#include "Engine.h"
#include "DirectXEngine.h"

#define MAX_NUMBER_OF_POINTS 512

namespace GraphicsEngine
{
	StreakEmitter::StreakEmitter()
	{
		myStreakEmitterData = nullptr;
	}

	StreakEmitter::StreakEmitter(SharedPointer<StreakEmitterData>& anEmitterData)
	{
		Init(anEmitterData);
	}

	StreakEmitter::~StreakEmitter()
	{
	}

	void StreakEmitter::Init(SharedPointer<StreakEmitterData>& anEmitterData)
	{
		myStreakEmitterData = anEmitterData;
		myProcessTime = 0.f;
		myStreaks.Init(MAX_NUMBER_OF_POINTS);
		myNumberOfMappedStreaks = 0;
		myTotalTime = 0;
	}

	StreakEmitter& StreakEmitter::operator=(const StreakEmitter& aStreakEmitter)
	{
		myStreakEmitterData = aStreakEmitter.myStreakEmitterData;
		myEmissionRotation = aStreakEmitter.myEmissionRotation;
		myOrientation = aStreakEmitter.myOrientation;
		myPosition = aStreakEmitter.myPosition;
		myGravity = aStreakEmitter.myGravity;
		myProcessTime = 0;
		myTotalTime = 0;
		myStreaks = aStreakEmitter.myStreaks;
		myNumberOfMappedStreaks = aStreakEmitter.myNumberOfMappedStreaks;
		myVertexBufferWrapper = aStreakEmitter.myVertexBufferWrapper;

		return *this;
	}

	void StreakEmitter::Update()
	{
		float deltaTime = CU::EventManager::GetInstance()->GetDeltaTime();
		EmitterUpdate(deltaTime);
		UpdateStreaks(deltaTime);
	}

	void StreakEmitter::Render(const Camera& aCamera)
	{
		if (myStreaks.Size() > 4)
		{
			UpdateVertexBuffer();
			UpdateShaderResources(aCamera);

			COMObjectPointer<ID3D11Device> device3D;
			COMObjectPointer<ID3D11DeviceContext> context;

			HRESULT result = myStreakEmitterData->myEffect->GetEffect()->GetDevice(&device3D);

			if (FAILED(result))
			{
				WF::AssertComAndWindowsError(result, "Failed to get 3Ddevice");
			}

			device3D->GetImmediateContext(&context);

			context->IASetInputLayout(myStreakEmitterData->myStreakInputLayout.Get());
			context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ);
			context->IASetVertexBuffers(myVertexBufferWrapper.myStartSlot, myVertexBufferWrapper.myNumberOfBuffers, &(myVertexBufferWrapper.myVertexBuffer), &myVertexBufferWrapper.myStride, &myVertexBufferWrapper.myByteOffset);

			myStreakEmitterData->myEffect->GetEffect()->GetVariableByName("LastPointTime")->AsScalar()->SetFloat(myStreaks.GetLast().myTime);
			myStreakEmitterData->myEffect->GetEffect()->GetVariableByName("LifeTime")->AsScalar()->SetFloat(myStreakEmitterData->myStreakData->myLifeTime);

			D3DX11_TECHNIQUE_DESC techDesc;
			myStreakEmitterData->myEffect->GetTechnique()->GetDesc(&techDesc);

			for (UINT p = 0; p < techDesc.Passes; ++p)
			{
				myStreakEmitterData->myEffect->GetTechnique()->GetPassByIndex(p)->Apply(0, context.Get());
				myNumberOfMappedStreaks = myStreaks.Size();
				context->Draw(myNumberOfMappedStreaks, 0);
			}
		}
	}

	void StreakEmitter::PerformRotation(Matrix33<float>& aRotation)
	{
		myEmissionRotation *= aRotation;
	}

	void StreakEmitter::PerformTransformation(Matrix44<float>& anOrientation)
	{
		myOrientation *= anOrientation;
	}

	const Vector3<float>& StreakEmitter::GetPosition() const
	{
		return myPosition;
	}

	void StreakEmitter::SetPosition(const Vector3<float>& aPositon)
	{
		myPosition = aPositon;
	}

	const Matrix44<float>& StreakEmitter::GetOrientation() const
	{
		return myOrientation;
	}

	void StreakEmitter::SetOrientation(const Matrix44<float>& anOrientation)
	{
		myOrientation = anOrientation;
		myPosition = myOrientation.GetTranslation();
	}

	bool StreakEmitter::GetIsDone() const
	{
		if (myStreakEmitterData == nullptr)
		{
			return true;
		}

		return (myTotalTime > myStreakEmitterData->myEmissionLifeTime && myStreaks.Size() == 0);
	}

	void StreakEmitter::UpdateVertexBuffer()
	{
		if (myStreaks.Size() > 0)
		{
			COMObjectPointer<ID3D11Device> device3D;
			COMObjectPointer<ID3D11DeviceContext> context;

			HRESULT result = myStreakEmitterData->myEffect->GetEffect()->GetDevice(&device3D);

			if (FAILED(result))
			{
				WF::AssertComAndWindowsError(result, "Failed to get 3Ddevice");
			}

			device3D->GetImmediateContext(&context);


			D3D11_MAPPED_SUBRESOURCE mappedResource;

			result = context->Map(myVertexBufferWrapper.myVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

			if (FAILED(result))
			{
				DL_ASSERT("Failed to map resource");
			}

			if (mappedResource.pData != nullptr)
			{
				Streak* data = (Streak*)mappedResource.pData;
				memcpy(data, &myStreaks[0], sizeof(Streak)* myStreaks.Size());
			}
			context->Unmap(myVertexBufferWrapper.myVertexBuffer.Get(), 0);

		}
		myNumberOfMappedStreaks = myStreaks.Size();
	}

	void StreakEmitter::UpdateShaderResources(const Camera& aCamera)
	{
		ID3DX11EffectMatrixVariable* projectionMatrixVariable = myStreakEmitterData->myEffect->GetEffect()->GetVariableByName("myProjectionMatrix")->AsMatrix();

		if (projectionMatrixVariable->IsValid() == false)
		{
			DL_ASSERT("Effect projectionMatrix variable not valid");
		}

		ID3DX11EffectMatrixVariable* viewMatrixVariable = myStreakEmitterData->myEffect->GetEffect()->GetVariableByName("myToViewMatrix")->AsMatrix();

		if (viewMatrixVariable->IsValid() == false)
		{
			DL_ASSERT("Effect viewMatrix variable not valid");
		}

		ID3DX11EffectShaderResourceVariable* textureVariable = myStreakEmitterData->myEffect->GetEffect()->GetVariableByName("DiffuseTexture")->AsShaderResource();
		if (textureVariable->IsValid() == false)
		{
			DL_ASSERT("Effect texture variable not valid");
		}

		projectionMatrixVariable->SetMatrix(&aCamera.GetProjection().myMatrix[0]);
		viewMatrixVariable->SetMatrix(&aCamera.GetInverse().myMatrix[0]);
		textureVariable->SetResource(myStreakEmitterData->myTexture->GetShaderView().Get());
	}

	void StreakEmitter::UpdateStreaks(const float aDeltaTime)
	{
		StreakData* data = myStreakEmitterData->myStreakData;

		for (unsigned short i = 0; i < myStreaks.Size(); i++)
		{
			myStreaks[i].myTime += aDeltaTime;
			myStreaks[i].mySize += data->myDeltaSize * aDeltaTime;
			myStreaks[i].myAlpha += data->myDeltaAlpha * aDeltaTime;
		}

		if (myStreaks.Size() > 1)
		{
			myStreaks.GetLast().myPosition = myPosition;
			myStreaks[myStreaks.Size() - 2] = myStreaks.GetLast();

			if (myStreaks[0].myTime >= data->myLifeTime)
			{
				myStreaks.RemoveAtIndex(1);
				myStreaks[0] = myStreaks[1];
			}
		}
		else if (myStreaks.Size() == 1)
		{
			if (myStreaks[0].myTime >= data->myLifeTime)
			{
				myStreaks.RemoveAtIndex(0);
			}
		}

	}

	void StreakEmitter::CreateVertexBuffer()
	{
		VertexDataWrapper data;
		data.myNumberOfVertexes = MAX_NUMBER_OF_POINTS;
		data.mySize = data.myNumberOfVertexes * sizeof(Streak);
		data.myStride = sizeof(Streak);
		data.myType = eVertexType::VERTEX_POS_VELOCITY_SIZE_ALPHA_TIME;

		COMObjectPointer<ID3D11Device> device3D = Engine::GetInstance()->GetEngine()->GetDevice();

		myVertexBufferWrapper.Create(device3D, &data);
	}

	void StreakEmitter::SetIsDead()
	{
		myStreaks.RemoveAll();
		myTotalTime = myStreakEmitterData->myEmissionLifeTime + 1;
	}

	void StreakEmitter::EmitterUpdate(const float aDeltaTime)
	{
		myTotalTime += aDeltaTime;
		if (myTotalTime <= myStreakEmitterData->myEmissionLifeTime)
		{
			myProcessTime += aDeltaTime;

			float timeBetweenEmission = 1 / myStreakEmitterData->myEmissionRatePerSecond;

			if (myProcessTime > timeBetweenEmission)
			{
				myProcessTime = 0;

				EmittStreak();
			}
		}

	}

	void StreakEmitter::EmittStreak()
	{
		if (myStreaks.Size() < MAX_NUMBER_OF_POINTS)
		{
			Streak newStreak;

			newStreak.myPosition = myPosition;
			newStreak.myAlpha = myStreakEmitterData->myStreakData->myStartAlpha;
			newStreak.mySize = myStreakEmitterData->myStreakData->myStartSize;
			newStreak.myTime = 0;

			switch (myStreaks.Size())
			{
			case 0:
				myStreaks.Add(newStreak);
				break;

			case 1:
				myStreaks.Add(myStreaks[0]);
				myStreaks.Add(newStreak);
				myStreaks.Add(newStreak);
				break;

			default:
				myStreaks.GetLast() = newStreak;
				myStreaks.Add(newStreak);
				break;
			}
		}
	}
}
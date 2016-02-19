#include "Engine_Precompiled.h"
#include "ParticleEmitter.h"
#include "EmitterData.h"
#include "../Common Utilities/EventManager.h"
#include "Effect.h"
#include "d3dx11effect.h"
#include "WindowsFunctions.h"
#include "Camera.h"
#include "Texture.h"

#include "ParticleData.h"
#include "../Common Utilities/Macros.h"
#include "VertexDataWrapper.h"
#include "COMObjectPointer.h"
#include "EmitterData.h"
#include "Engine.h"
#include "DirectXEngine.h"

#define MAX_NUMBER_OF_PARTICLES 128

namespace GraphicsEngine
{
	ParticleEmitter::ParticleEmitter()
	{
		myProcessTime = 0.f;
		myNumberOfMappedParticles = 0;
		myTotalTime = 0;
		myParticles.Init(MAX_NUMBER_OF_PARTICLES + 1);
	}

	ParticleEmitter::ParticleEmitter(SharedPointer<EmitterData>& anEmitterData)
	{
		Init(anEmitterData);
	}

	ParticleEmitter::ParticleEmitter(const ParticleEmitter& aEmitter)
	{
		*this = aEmitter;
	}

	ParticleEmitter::~ParticleEmitter()
	{
	}

	void ParticleEmitter::Destroy()
	{
	}

	bool ParticleEmitter::Init(SharedPointer<EmitterData>& anEmitterData)
	{
		myEmitterData = anEmitterData;
		myProcessTime = 0.f;
		myParticles.Init(MAX_NUMBER_OF_PARTICLES + 1);
		myNumberOfMappedParticles = 0;
		myTotalTime = 0;

		return true;
	}

	const ParticleEmitter& ParticleEmitter::operator=(const ParticleEmitter& anEmitter)
	{
		myEmitterData = anEmitter.myEmitterData;

		myEmissionRotation = anEmitter.myEmissionRotation;
		myOrientation = anEmitter.myOrientation;
		myPosition = anEmitter.myPosition;
		myGravity = anEmitter.myGravity;
		myProcessTime = 0;
		myTotalTime = 0;
		myParticles = anEmitter.myParticles;
		myNumberOfMappedParticles = anEmitter.myNumberOfMappedParticles;

		CreateVertexBuffer();

		return *this;
	}

	void ParticleEmitter::Update()
	{
		assert(myEmitterData != nullptr && "Trying to use a non initializied emitter");

		float deltaTime = CU::EventManager::GetInstance()->GetDeltaTime();
		EmitterUpdate(deltaTime);
		UpdateParticles(deltaTime);
	}

	void ParticleEmitter::Render(const Camera& aCamera)
	{
		if (myParticles.Size() > 0)
		{
			UpdateVertexBuffer();
			UpdateShaderResources(aCamera);

			COMObjectPointer<ID3D11Device> device3D;
			COMObjectPointer<ID3D11DeviceContext> context;

			HRESULT result = myEmitterData->myEffect->GetEffect()->GetDevice(&device3D);

			if (FAILED(result))
			{
				WF::AssertComAndWindowsError(result, "Failed to get 3Ddevice");
			}

			device3D->GetImmediateContext(&context);

			context->IASetInputLayout(myEmitterData->myParticleInputLayout.Get());
			context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
			context->IASetVertexBuffers(myVertexBufferWrapper.myStartSlot, myVertexBufferWrapper.myNumberOfBuffers, &(myVertexBufferWrapper.myVertexBuffer), &myVertexBufferWrapper.myStride, &myVertexBufferWrapper.myByteOffset);

			D3DX11_TECHNIQUE_DESC techDesc;
			myEmitterData->myEffect->GetTechnique()->GetDesc(&techDesc);

			for (UINT p = 0; p < techDesc.Passes; ++p)
			{
				myEmitterData->myEffect->GetTechnique()->GetPassByIndex(p)->Apply(0, context.Get());
				context->Draw(myNumberOfMappedParticles, 0);
			}
		}
	}

	void ParticleEmitter::PerformRotation(Matrix33<float>& aRotation)
	{
		myEmissionRotation *= aRotation;
	}

	void ParticleEmitter::PerformTransformation(Matrix44<float>& anOrientation)
	{
		myOrientation *= anOrientation;
	}

	const Vector3<float>& ParticleEmitter::GetPosition() const
	{
		return myPosition;
	}

	void ParticleEmitter::SetPosition(Vector3<float>& aPositon)
	{
		myPosition = aPositon;
	}

	const Matrix44<float>& ParticleEmitter::GetOrientation() const
	{
		return myOrientation;
	}

	void ParticleEmitter::SetOrientation(const Matrix44<float>& anOrientation)
	{
		myOrientation = anOrientation;
		myPosition = myOrientation.GetTranslation();
	}

	bool ParticleEmitter::GetIsDone() const
	{
		if (myEmitterData == nullptr)
		{
			return true;
		}
		return (myTotalTime >= myEmitterData->myEmissionLifeTime && myParticles.Size() == 0 && myEmitterData->myEmissionLifeTime > 0);
	}

	void ParticleEmitter::UpdateShaderResources(const Camera& aCamera)
	{
		COMObjectPointer<ID3DX11EffectMatrixVariable> projectionMatrixVariable = myEmitterData->myEffect->GetEffect()->GetVariableByName("myProjectionMatrix")->AsMatrix();

		if (projectionMatrixVariable->IsValid() == false)
		{
			DL_ASSERT("Effect projectionMatrix variable not valid");
		}

		COMObjectPointer<ID3DX11EffectMatrixVariable> viewMatrixVariable = myEmitterData->myEffect->GetEffect()->GetVariableByName("myToViewMatrix")->AsMatrix();

		if (viewMatrixVariable->IsValid() == false)
		{
			DL_ASSERT("Effect viewMatrix variable not valid");
		}

		COMObjectPointer<ID3DX11EffectShaderResourceVariable> textureVariable = myEmitterData->myEffect->GetEffect()->GetVariableByName("DiffuseTexture")->AsShaderResource();
		if (textureVariable->IsValid() == false)
		{
			DL_ASSERT("Effect texture variable not valid");
		}

		projectionMatrixVariable->SetMatrix(&aCamera.GetProjection().myMatrix[0]);
		viewMatrixVariable->SetMatrix(&aCamera.GetInverse().myMatrix[0]);
		textureVariable->SetResource(myEmitterData->myTexture->GetShaderView().Get());
	}

	void ParticleEmitter::UpdateVertexBuffer()
	{
		if (myParticles.Size() > 0)
		{
			COMObjectPointer<ID3D11Device> device3D;
			COMObjectPointer<ID3D11DeviceContext> context;

			HRESULT result = myEmitterData->myEffect->GetEffect()->GetDevice(&device3D);

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

			if (mappedResource.pData != nullptr && myParticles.Size() > 0)
			{
				Particle* data = (Particle*)mappedResource.pData;
				memcpy(data, &myParticles[0], sizeof(Particle)* myParticles.Size());
			}

			context->Unmap(myVertexBufferWrapper.myVertexBuffer.Get(), 0);
		}

		myNumberOfMappedParticles = myParticles.Size();
	}

	void ParticleEmitter::UpdateParticles(float aDeltaTime)
	{
		ParticleData* data = myEmitterData->myParticleData;

		for (unsigned short i = 0; i < myParticles.Size(); ++i)
		{
			myParticles[i].myTime += aDeltaTime;

			myParticles[i].myVelocity -= GetNormalized(myParticles[i].myVelocity) * myEmitterData->myParticleData->myDeAcceleration * aDeltaTime;
			myParticles[i].myPosition += myParticles[i].myVelocity * aDeltaTime;
			myParticles[i].myScale += data->myScaleDelta * aDeltaTime;
			myParticles[i].myAlpha += data->myDeltaAlpha * aDeltaTime;

			if (myParticles[i].myTime >= data->myLifeTime)
			{
				myParticles.RemoveCyclicAtIndex(i);
				--i;
			}
		}
	}

	void ParticleEmitter::CreateVertexBuffer()
	{
		VertexDataWrapper data;
		data.myNumberOfVertexes = MAX_NUMBER_OF_PARTICLES;
		data.mySize = data.myNumberOfVertexes * sizeof(Particle);
		data.myStride = sizeof(Particle);
		data.myType = eVertexType::VERTEX_POS_VELOCITY_SIZE_ALPHA_TIME;

		COMObjectPointer<ID3D11Device> device3D = Engine::GetInstance()->GetEngine()->GetDevice();

		myVertexBufferWrapper.Create(device3D, &data);
	}

	void ParticleEmitter::EmitterUpdate(float anElapsedTime)
	{
		myTotalTime += anElapsedTime;
		if (myTotalTime <= myEmitterData->myEmissionLifeTime || myEmitterData->myEmissionLifeTime < 0)
		{
			myProcessTime += anElapsedTime;

			float timeBetweenEmission = 1.f / myEmitterData->myEmissionRatePerSecond;

			if (myProcessTime > timeBetweenEmission)
			{
				myProcessTime = 0;//timeBetweenEmission - myProcessTime;
				EmittParticle(0);
			}
		}
	}

	void ParticleEmitter::EmittParticle(float anElapsedTime)
	{
		if (myParticles.Size() < MAX_NUMBER_OF_PARTICLES)
		{
			Particle newParticle;

			newParticle.myPosition.myX = RANDOM_FLOAT(myPosition.myX - myEmitterData->myEmissionExtents.myX, myPosition.myX + myEmitterData->myEmissionExtents.myX);
			newParticle.myPosition.myY = RANDOM_FLOAT(myPosition.myY - myEmitterData->myEmissionExtents.myY, myPosition.myY + myEmitterData->myEmissionExtents.myY);
			newParticle.myPosition.myZ = RANDOM_FLOAT(myPosition.myZ - myEmitterData->myEmissionExtents.myZ, myPosition.myZ + myEmitterData->myEmissionExtents.myZ);
			newParticle.myAlpha = myEmitterData->myParticleData->myStartAlpha;
			newParticle.myScale = myEmitterData->myParticleData->myStartScale;
			newParticle.myTime = anElapsedTime;

			newParticle.myVelocity.myX = RANDOM_FLOAT(myEmitterData->myParticleData->myMinVelocity.myX, myEmitterData->myParticleData->myMaxVelocity.myX);
			newParticle.myVelocity.myY = RANDOM_FLOAT(myEmitterData->myParticleData->myMinVelocity.myY, myEmitterData->myParticleData->myMaxVelocity.myY);
			newParticle.myVelocity.myZ = RANDOM_FLOAT(myEmitterData->myParticleData->myMinVelocity.myZ, myEmitterData->myParticleData->myMaxVelocity.myZ);

			Vector4f velocity = { newParticle.myVelocity.x, newParticle.myVelocity.y, newParticle.myVelocity.z, 0.f };
			velocity = velocity * myOrientation;
			newParticle.myVelocity = { velocity.x, velocity.y, velocity.z };

			myParticles.Add(newParticle);
		}
	}
}
#pragma once
#include "DynamicVertexBufferWrapper.h"
#include "Particle.h"
#include "../Common Utilities/SharedPointer.h"

namespace GraphicsEngine
{
	class Camera;
	class EmitterData;

	class ParticleEmitter
	{
	public:
		ParticleEmitter();
		ParticleEmitter(const ParticleEmitter& aEmitter);
		ParticleEmitter(SharedPointer<EmitterData>& anEmitterData);

		bool Init(SharedPointer<EmitterData>& anEmitterData);

		~ParticleEmitter();

		const ParticleEmitter& operator=(const ParticleEmitter& anEmitter);

		void Update();
		void Render(const Camera& aCamera);
		
		void Destroy();

		void PerformRotation(Matrix33<float>& aRotation);
		void PerformTransformation(Matrix44<float>& anOrientation);
		
		const Vector3<float>& GetPosition() const;
		void SetPosition(Vector3<float>& aPositon);
		const Matrix44<float>& GetOrientation() const;
		void SetOrientation(const Matrix44<float>& anOrientation);

		bool GetIsDone() const;

		void CreateVertexBuffer();
	private:
		void UpdateVertexBuffer();
		void UpdateShaderResources(const Camera& aCamera);
		void UpdateParticles(float anElapsedTime);
		void EmitterUpdate(float anElapsedTime);
		void EmittParticle(float anElapsedTime);

		Matrix44<float> myOrientation;
		Matrix33<float> myEmissionRotation;
		Vector3<float> myPosition;
		SharedPointer<EmitterData> myEmitterData;
		float myGravity;
		float myProcessTime;
		float myTotalTime;
		int myNumberOfMappedParticles;

		CU::GrowingArray<Particle, unsigned int> myParticles;

		DynamicVertexBufferWrapper myVertexBufferWrapper;
	};
}
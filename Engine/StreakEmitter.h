#pragma once

#include "DynamicVertexBufferWrapper.h"
#include "Streak.h"
#include "../Common Utilities/GrowingArray.h"
#include "../Common Utilities/Matrix.h"

namespace GraphicsEngine
{
	class Camera;
	class StreakEmitterData;

	class StreakEmitter
	{
	public:
		StreakEmitter();
		StreakEmitter(SharedPointer<StreakEmitterData>& anEmitterData);
		~StreakEmitter();

		StreakEmitter& operator=(const StreakEmitter& aStreakEmitter);

		void Init(SharedPointer<StreakEmitterData>& anEmitterData);

		void Update();
		void Render(const Camera& aCamera);

		void PerformRotation(Matrix33<float>& aRotation);
		void PerformTransformation(Matrix44<float>& anOrientation);

		const Vector3<float>& GetPosition() const;
		void SetPosition(const Vector3<float>& aPositon);
		const Matrix44<float>& GetOrientation() const;
		void SetOrientation(const Matrix44<float>& anOrientation);

		bool GetIsDone() const;

		void CreateVertexBuffer();

		void SetIsDead();

	private:
		void UpdateVertexBuffer();
		void UpdateShaderResources(const Camera& aCamera);
		void UpdateStreaks(const float aDeltaTime);
		void EmitterUpdate(const float aDeltaTime);
		void EmittStreak();

		Matrix33<float> myEmissionRotation;
		Matrix44<float> myOrientation;
		Vector3<float> myPosition;
		SharedPointer<StreakEmitterData> myStreakEmitterData;
		float myGravity;
		float myProcessTime;
		float myTotalTime;
		int myNumberOfMappedStreaks;

		CU::GrowingArray<Streak, unsigned int> myStreaks;

		DynamicVertexBufferWrapper myVertexBufferWrapper;
	};

}

namespace GE = GraphicsEngine;
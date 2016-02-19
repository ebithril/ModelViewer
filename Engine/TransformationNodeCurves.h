#pragma once
#include "TransformationNode.h"

#include "../Common Utilities/StaticArray.h"
#include "CurveAnimationFrame.h"

namespace GraphicsEngine
{
	class AnimationCurve;

	class TransformationNodeCurves : public TransformationNode
	{
	public:
		TransformationNodeCurves();
		~TransformationNodeCurves();

		void Init(float aStartTime, float aEndTime);
		void SetBaseFrame(Vector3f aDirection, Vector3f aTranslation, float aAngle);

		void SetRotationCurve(int i, AnimationCurve* aAnimationCurve);
		void SetTranslationCurve(int i, AnimationCurve* aAnimationCurve);

		const Matrix44f GetTransformationForTime(float aTime) override;

		float GetEndTime() override;
	private:
		CU::StaticArray<AnimationCurve*, 3> myRotationCurves;
		CU::StaticArray<AnimationCurve*, 3> myTranslationCurves;
		
		float myStartTime;
		float myEndTime;
	};
}

namespace GE = GraphicsEngine;
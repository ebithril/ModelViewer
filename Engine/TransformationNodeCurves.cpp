#include "Engine_Precompiled.h"
#include "TransformationNodeCurves.h"

#include "AnimationCurve.h"

namespace GraphicsEngine
{
	TransformationNodeCurves::TransformationNodeCurves()
	{
		myStartTime = 0;
		myEndTime = 0;
	}

	TransformationNodeCurves::~TransformationNodeCurves()
	{
	}

	void TransformationNodeCurves::Init(float aStartTime, float aEndTime)
	{
		myStartTime = aStartTime;
		myEndTime = aEndTime;
	}

	void TransformationNodeCurves::SetRotationCurve(int i, AnimationCurve* aAnimationCurve)
	{
		myRotationCurves[i] = aAnimationCurve;
	}

	void TransformationNodeCurves::SetTranslationCurve(int i, AnimationCurve* aAnimationCurve)
	{
		myTranslationCurves[i] = aAnimationCurve;
	}

	const Matrix44<float> TransformationNodeCurves::GetTransformationForTime(float aTime)
	{
		Matrix44<float> transform;

		float radianX = -myRotationCurves[0]->GetValueFromTime(aTime) * 0.0174532925f;
		float radianY = myRotationCurves[1]->GetValueFromTime(aTime) * 0.0174532925f;
		float radianZ = myRotationCurves[2]->GetValueFromTime(aTime) * 0.0174532925f;

		float translateX = -myTranslationCurves[0]->GetValueFromTime(aTime);
		float translateY = myTranslationCurves[1]->GetValueFromTime(aTime);
		float translateZ = myTranslationCurves[2]->GetValueFromTime(aTime);

		transform *= Matrix44<float>::CreateRotateAroundX(radianX);
		transform *= Matrix44<float>::CreateRotateAroundY(radianY);
		transform *= Matrix44<float>::CreateRotateAroundZ(radianZ);

		transform.SetTranslation(Vector3<float>(translateX, translateY, translateZ));

		return transform;
	}

	float TransformationNodeCurves::GetEndTime()
	{
		return myEndTime;
	}
}
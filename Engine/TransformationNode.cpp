#include "Engine_Precompiled.h"
#include "TransformationNode.h"

namespace GraphicsEngine
{
	TransformationNode::TransformationNode()
	{
	}

	TransformationNode::TransformationNode(const Matrix44<float> aOrientation)
	{
		myOrientation = aOrientation;
	}

	TransformationNode::~TransformationNode()
	{
	}

	const Matrix44<float> TransformationNode::GetTransformationForTime(float /*aTime*/)
	{
		return myOrientation;
	}

	void TransformationNode::SetOrientation(const Matrix44<float>& aOrientation)
	{
		myOrientation = aOrientation;
	}

	float TransformationNode::GetEndTime()
	{
		return 0.f;
	}
}
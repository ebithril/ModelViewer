#include "Engine_Precompiled.h"
#include "TransformationNodeHierarchy.h"

namespace GraphicsEngine
{
	TransformationNodeHierarchy::TransformationNodeHierarchy()
	{
		myTransformationNodes.Init(4);
	}

	TransformationNodeHierarchy::~TransformationNodeHierarchy()
	{
	}

	float TransformationNodeHierarchy::GetEndTime()
	{
		float endTime = 0;

		for (TransformationNode* node : myTransformationNodes)
		{
			endTime = max(endTime, node->GetEndTime());
		}

		return endTime;
	}

	void TransformationNodeHierarchy::AddTransformation(TransformationNode* aTransformation)
	{
		myTransformationNodes.Add(aTransformation);
	}

	const Matrix44f TransformationNodeHierarchy::GetTransformationForTime(float aTime)
	{
		Matrix44f returnMatrix;

		for (TransformationNode* node : myTransformationNodes)
		{
			returnMatrix *= node->GetTransformationForTime(aTime);
		}

		return returnMatrix;
	}
}
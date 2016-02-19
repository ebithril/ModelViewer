#pragma once
#include "TransformationNode.h"

#include "../Common Utilities/GrowingArray.h"

namespace GraphicsEngine
{
	class TransformationNodeHierarchy : public TransformationNode
	{
	public:
		TransformationNodeHierarchy();
		~TransformationNodeHierarchy();

		const Matrix44f GetTransformationForTime(float aTime) override;
		void AddTransformation(TransformationNode* aTransformation);

		float GetEndTime() override;

	private:
		CU::GrowingArray<TransformationNode*> myTransformationNodes;
	};
}

namespace GE = GraphicsEngine;
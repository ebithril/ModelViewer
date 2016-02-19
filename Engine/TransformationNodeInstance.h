#pragma once

#include "../Common Utilities/GrowingArray.h"
#include "../Common Utilities/Matrix44.h"

namespace GraphicsEngine
{
	class TransformationNode;

	class TransformationNodeInstance
	{
	public:
		TransformationNodeInstance();
		~TransformationNodeInstance();

		void Init();

		const TransformationNodeInstance& operator=(const TransformationNodeInstance& aTransformationNode);

		void AddChildNode(const TransformationNodeInstance& aChildNode);
		CU::GrowingArray<TransformationNodeInstance>& GetChildren();

		const Matrix44<float> GetTransformation();
		
		TransformationNode* GetTransformationNode();
		void SetTransformationNode(TransformationNode* const aTransformationNode);

		void Update();
	private:
		CU::GrowingArray<TransformationNodeInstance> myChildren;
		float myElapsedTime;
		TransformationNode* myTransFormationNode;
	};
}

namespace GE = GraphicsEngine;
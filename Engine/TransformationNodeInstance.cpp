#include "Engine_Precompiled.h"
#include "TransformationNodeInstance.h"

#include "TransformationNode.h"
#include "../Common Utilities/EventManager.h"

namespace GraphicsEngine
{
	TransformationNodeInstance::TransformationNodeInstance()
	{
		myElapsedTime = 0;
		myChildren = CU::GrowingArray<TransformationNodeInstance>();
		myTransFormationNode = nullptr;

	}

	TransformationNodeInstance::~TransformationNodeInstance()
	{
	}

	void TransformationNodeInstance::Init()
	{
		myChildren.Init(4);
	}

	void TransformationNodeInstance::AddChildNode(const TransformationNodeInstance& aChildNode)
	{
		myChildren.Add(aChildNode);
	}

	CU::GrowingArray<TransformationNodeInstance>& TransformationNodeInstance::GetChildren()
	{
		return myChildren;
	}

	const Matrix44<float> TransformationNodeInstance::GetTransformation()
	{
		if (myTransFormationNode != nullptr)
		{
			return myTransFormationNode->GetTransformationForTime(myElapsedTime);
		}

		return Matrix44<float>();
	}

	TransformationNode* TransformationNodeInstance::GetTransformationNode()
	{
		return myTransFormationNode;
	}

	void TransformationNodeInstance::SetTransformationNode(TransformationNode* const aTransformationNode)
	{
		myTransFormationNode = aTransformationNode;
	}

	void TransformationNodeInstance::Update()
	{
		myElapsedTime += CU::EventManager::GetInstance()->GetDeltaTime();

		if (myTransFormationNode == nullptr || myElapsedTime > myTransFormationNode->GetEndTime())
		{
			myElapsedTime = 0;
		}

		for (TransformationNodeInstance& child : myChildren)
		{
			child.Update();
		}
	}

	const TransformationNodeInstance& TransformationNodeInstance::operator=(const TransformationNodeInstance& aTransformationNode)
	{
		if (myChildren.IsInited() == false)
		{
			Init();
		}

		myTransFormationNode = aTransformationNode.myTransFormationNode;
		myElapsedTime = aTransformationNode.myElapsedTime;

		myChildren.RemoveAll();

		for (const TransformationNodeInstance& child : aTransformationNode.myChildren)
		{
			myChildren.Add(child);
		}

		return *this;
	}
}
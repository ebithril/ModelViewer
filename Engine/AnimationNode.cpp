#include "Engine_Precompiled.h"
#include "AnimationNode.h"

namespace GraphicsEngine
{
	AnimationNode::AnimationNode(int aNumOfFrames)
		: myValues(max(1, unsigned short(aNumOfFrames)))
		, myEndTime(0.f)
	{
	}

	AnimationNode::~AnimationNode()
	{
	}

	void AnimationNode::AddValue(const AnimationNodeValue& aFrame)
	{
		myValues.Add(aFrame);
		myEndTime = max(myEndTime, aFrame.myTime);
	}

	bool AnimationNode::HasValues() const
	{
		return myValues.Size() > 0;
	}

	Matrix44<float> AnimationNode::GetCurrentMatrix(float aTime)
	{
		Matrix44<float>* startValue = &myValues[0].myMatrix;
		Matrix44<float>* endValue = &myValues.GetLast().myMatrix;
		float alpha = 1.0f;
		if (aTime > myValues[0].myTime)
		{
			for (unsigned short i = 0; i < myValues.Size() - 1; i++)
			{
				const unsigned short currentId = i;
				float nextValue = myValues[currentId + 1].myTime;
				if (nextValue > aTime)
				{
					float currentValue = myValues[currentId].myTime;
					startValue = &myValues[currentId].myMatrix;
					endValue = &myValues[currentId + 1].myMatrix;
					alpha = (aTime - currentValue) / (nextValue - currentValue);
					break;
				}
			}
		}
		else
		{
			startValue = &myValues.GetLast().myMatrix;
			endValue = &myValues[0].myMatrix;
			if (myValues[0].myTime > 0 || myValues[0].myTime < 0)
			{
				alpha = (aTime) / (myValues[0].myTime);
			}
		}

		return (*startValue * (1.0f - alpha)) + (*endValue * alpha);
	}

	float AnimationNode::GetAnimationLenght()
	{
		return myEndTime;
	}
}
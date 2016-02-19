#pragma once

#include "../Common Utilities/Matrix44.h"
#include "../Common Utilities/GrowingArray.h"

namespace GraphicsEngine
{
	struct AnimationNodeValue
	{
		float myTime;
		Matrix44<float> myMatrix;
	};

	class AnimationNode
	{
		friend class Animation;
		friend class FBXFactory;
	public:
		AnimationNode(int aNumOfValues);
		~AnimationNode();

		void AddValue(const AnimationNodeValue& aFrame);
		bool HasValues() const;

		Matrix44<float> GetCurrentMatrix(float aTime);
		float GetAnimationLenght();

	private:
		CU::GrowingArray<AnimationNodeValue> myValues;
		float myEndTime;
		std::string myBoneName;
	};
}

namespace GE = GraphicsEngine;
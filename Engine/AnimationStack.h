#pragma once

#include "AnimationDefines.h"
#include "../Common Utilities/GrowingArray.h"
#include "..\Common Utilities\Matrix44.h"

namespace GraphicsEngine
{
	class Animation;

	struct OngoingAnimation
	{
		std::string myName;
		float myTimeStamp;
		bool myShouldLoop;
		Animation* myAnimation;
	};

	class AnimationStack
	{
	public:
		AnimationStack();
		~AnimationStack();

		void Pop();
		void RemoveAnimation(const std::string& aName);
		void Add(Animation* aAnimation, bool aShouldLoop, const std::string& aName);
		void GetBoneMatrices(CU::StaticArray<Matrix44<float>, MAX_NR_OF_BONES>& aApplyArray);
		void Update(float aDeltaTime);
		bool Empty()const;
	private:
		CU::GrowingArray<OngoingAnimation> myAnimations;
		float myTotalTime;
	};
}

namespace GE = GraphicsEngine;
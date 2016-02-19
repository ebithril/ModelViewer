#pragma once

#include "AnimationStack.h"

namespace GraphicsEngine
{
	class AnimationContainer;

	class AnimationInstance
	{
	public:
		AnimationInstance();
		~AnimationInstance();

		void PlayAnimation(const std::string& aName, bool aShouldLoop);
		void StopAnimation(const std::string& aName);
		void GetBoneMatrices(CU::StaticArray<Matrix44<float>, MAX_NR_OF_BONES>& aApplyArray);

		void Update(float aDeltaTime);

		void SetAnimations(AnimationContainer* someAnimations);

		const bool AnimationExists(const std::string& anAnimationName)const ;
	private:
		AnimationStack myStack;
		AnimationContainer* myAnimations;
	};
}

namespace GE = GraphicsEngine;
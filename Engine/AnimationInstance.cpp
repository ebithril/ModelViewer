#include "Engine_Precompiled.h"
#include "AnimationInstance.h"

#include "AnimationContainer.h"

namespace GraphicsEngine
{
	AnimationInstance::AnimationInstance()
	{
	}

	AnimationInstance::~AnimationInstance()
	{
	}

	void AnimationInstance::PlayAnimation(const std::string& aName, bool aShouldLoop)
	{
		if (myAnimations != nullptr)
		{
			Animation* animationToPlay = myAnimations->GetAnimation(aName);

			myStack.Add(animationToPlay, aShouldLoop, aName);
		}
		else
		{
			myStack.Add(nullptr, true, aName);
		}
	}

	void AnimationInstance::StopAnimation(const std::string& aName)
	{
		myStack.RemoveAnimation(aName);
	}

	void AnimationInstance::GetBoneMatrices(CU::StaticArray<Matrix44<float>, MAX_NR_OF_BONES>& aApplyArray)
	{
		if (myStack.Empty() == true)
		{
			return;
		}
		else
		{
			myStack.GetBoneMatrices(aApplyArray);
		}
	}

	void AnimationInstance::Update(float aDeltaTime)
	{
		myStack.Update(aDeltaTime);
	}

	void AnimationInstance::SetAnimations(AnimationContainer* someAnimations)
	{
		myAnimations = someAnimations;
	}

	const bool AnimationInstance::AnimationExists(const std::string& anAnimationName) const
	{
		if (myAnimations == nullptr)
		{
			return false;
		}
		return myAnimations->CheckIfExist(anAnimationName);
	}

}
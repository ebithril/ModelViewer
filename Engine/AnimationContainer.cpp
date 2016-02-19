#include "Engine_Precompiled.h"
#include "AnimationContainer.h"

namespace GraphicsEngine
{
	AnimationContainer::AnimationContainer()
	{
	}

	AnimationContainer::~AnimationContainer()
	{
	}

	Animation* AnimationContainer::GetAnimation(const std::string& aAnimationName)
	{
		if (CheckIfExist(aAnimationName) == true)
		{
			return myAnimations[aAnimationName];
		}
		else
		{
			return nullptr;
		}
	}

	void AnimationContainer::AddAnimation(const std::string& aAnimationName, Animation* aAnimation)
	{
		myAnimations[aAnimationName] = aAnimation;
	}

	const bool AnimationContainer::CheckIfExist(const std::string& aAnimationName) const
	{
		return myAnimations.find(aAnimationName) != myAnimations.end();
	}
}
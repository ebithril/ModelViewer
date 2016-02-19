#include "Engine_Precompiled.h"
#include "AnimationStack.h"

#include "Animation.h"

namespace GraphicsEngine
{
	AnimationStack::AnimationStack()
	{
		myAnimations.Init(4);
		myTotalTime = 0;
	}

	AnimationStack::~AnimationStack()
	{
	}

	void AnimationStack::Pop()
	{
		myAnimations.RemoveLast();
	}

	void AnimationStack::RemoveAnimation(const std::string& aName)
	{
		for (int i = myAnimations.Size() - 1; i >= 0; i--)
		{
			if (myAnimations[unsigned short(i)].myName == aName)
			{
				myAnimations.RemoveAtIndex(unsigned short(i));

				if (i > 0 && myAnimations[unsigned short(i - 1)].myShouldLoop == true)
				{
					myAnimations[unsigned short(i - 1)].myTimeStamp = myTotalTime;
				}

				break;
			}
		}
	}

	void AnimationStack::Add(Animation* aAnimation, bool aShouldLoop, const std::string& aName)
	{
		for (OngoingAnimation& animation : myAnimations)
		{
			if (animation.myName == aName)
			{
				return;
			}
		}

		OngoingAnimation animation;
		animation.myName = aName;
		animation.myAnimation = aAnimation;
		animation.myShouldLoop = aShouldLoop;
		animation.myTimeStamp = myTotalTime;

		myAnimations.Add(animation);

		if (animation.myShouldLoop == false && animation.myAnimation == nullptr)
		{
			Pop();
		}
	}
	
	void AnimationStack::GetBoneMatrices(CU::StaticArray<Matrix44<float>, MAX_NR_OF_BONES>& aApplyArray)
	{
		OngoingAnimation animation = myAnimations.GetLast();
		if (animation.myAnimation != nullptr)
		{
			animation.myAnimation->Update(myTotalTime - animation.myTimeStamp, aApplyArray);
		}
		else
		{
			for (int i = 0; i < MAX_NR_OF_BONES; i++)
			{
				aApplyArray[i] = Matrix44<float>();
			}
		}
	}

	void AnimationStack::Update(float aDeltaTime)
	{
		myTotalTime += aDeltaTime;

		OngoingAnimation animation = myAnimations.GetLast();

		while (animation.myAnimation != nullptr && animation.myShouldLoop == false)
		{
			if (myTotalTime - animation.myTimeStamp > animation.myAnimation->GetAnimationLenght())
			{
				Pop();
			}
			else
			{
				break;
			}
			animation = myAnimations.GetLast();
		}
	}

	bool AnimationStack::Empty() const
	{
		if (myAnimations.Size() == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

}
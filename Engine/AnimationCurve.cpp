#include "Engine_Precompiled.h"
#include "AnimationCurve.h"

#define LERP(v0, v1, t) (v0)*(1-(t)) + (v1)*(t)

namespace GraphicsEngine
{
	AnimationCurve::AnimationCurve()
	{
		myKeyFrames.Init(4);
	}

	AnimationCurve::~AnimationCurve()
	{
	}

	void AnimationCurve::AddKeyFrame(const KeyFrame& aKeyFrame)
	{
		myKeyFrames.Add(aKeyFrame);
	}

	void AnimationCurve::FinalizeCurve()
	{
		myStartTime = myKeyFrames[0].myTime;
		myEndTime = myKeyFrames.GetLast().myTime;
	}

	float AnimationCurve::GetValueFromTime(float aTime)
	{
		if (aTime < myStartTime)
		{
			return myKeyFrames[0].myValue;
		}
		if (aTime > myEndTime)
		{
			return myKeyFrames.GetLast().myValue;
		}

		KeyFrame* myFramesToLERP[2];

		for (short i = myKeyFrames.Size() - 1; i >= 0; i--)
		{
			if (myKeyFrames[i].myTime < aTime)
			{
				myFramesToLERP[0] = &myKeyFrames[i];
				if (myKeyFrames.Size() > i + 1)
				{
					myFramesToLERP[1] = &myKeyFrames[i + 1];
				}
				else
				{
					return myFramesToLERP[0]->myValue;
				}

				break;
			}
		}

		float frameTime = myFramesToLERP[1]->myTime - myFramesToLERP[0]->myTime;
		float currentFrameTime = aTime - myFramesToLERP[0]->myTime;

		float lerpValue = currentFrameTime / frameTime;

		return LERP(myFramesToLERP[0]->myValue, myFramesToLERP[1]->myValue, lerpValue);
	}
}
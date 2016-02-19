#pragma once

#include "KeyFrame.h"
#include "../Common Utilities/GrowingArray.h"

namespace GraphicsEngine
{
	class AnimationCurve
	{
	public:
		AnimationCurve();
		~AnimationCurve();

		void AddKeyFrame(const KeyFrame& aKeyFrame);
		void FinalizeCurve();
		float GetValueFromTime(float aTime);
	private:
		CU::GrowingArray<KeyFrame> myKeyFrames;
		float myEndTime;
		float myStartTime;
	};
}
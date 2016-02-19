#include "../Common Utilities/Vector3.h"


namespace GraphicsEngine
{
	struct StreakData
	{
		Vector3<float> myVelocity;
		float myDeAcceleration;

		float myLifeTime;

		float myStartSize;
		float myDeltaSize;

		float myStartAlpha;
		float myDeltaAlpha;

		bool myIsAffectedByGravity;
	};
}
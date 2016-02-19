#include "../Common Utilities/Vector.h"

namespace GraphicsEngine
{
	struct ParticleData
	{
		Vector3<float> myMaxVelocity;
		Vector3<float> myMinVelocity;

		float myDeAcceleration;

		float myMaxRotation;
		float myMinRotation;

		float myLifeTime;

		Vector2f myStartScale;
		Vector2f myScaleDelta;

		float myStartAlpha;
		float myDeltaAlpha;

		bool myIsAffectedByGravity;
	};
}

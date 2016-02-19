#pragma once

#include "../Common Utilities/Vector3.h"

struct Particle
{
public:
	Vector3f myPosition;
	float myTime;
	Vector3<float> myVelocity;
	float myRotation;
	Vector2f myScale;
	float myAlpha;
};
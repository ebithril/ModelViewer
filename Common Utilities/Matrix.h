#pragma once
#include "Matrix33.h"
#include "Matrix33decl.h"
#include "Matrix44.h"
#include "Matrix44decl.h"

typedef Matrix44<float> Matrix44f;
typedef Matrix33<float> Matrix33f;


template<typename Type>
Vector3<Type> Get3DPositionFromViewPosition(const Vector3<Type> &aViewPosition, const Matrix44<float>& aCameraProjection, const Matrix44<float>& aCameraInverse)
{
	float oldZ = aViewPosition.z;
	Vector3<float> viewPosition;
	viewPosition.myX = ((2.0f * aViewPosition.myX) - 1) / aCameraProjection[0];
	viewPosition.myY = -((2.0f * aViewPosition.myY) - 1) / aCameraProjection[5];
	viewPosition.myZ = 1.f;

	Vector3<float> rayDirection;

	rayDirection.myX = viewPosition.myX * aCameraInverse[0] + viewPosition.myY * aCameraInverse[4] + viewPosition.myZ * aCameraInverse[8];
	rayDirection.myY = viewPosition.myX * aCameraInverse[1] + viewPosition.myY * aCameraInverse[5] + viewPosition.myZ * aCameraInverse[9];
	rayDirection.myZ = viewPosition.myX * aCameraInverse[2] + viewPosition.myY * aCameraInverse[6] + viewPosition.myZ * aCameraInverse[10];

	return rayDirection * oldZ;
}
#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Vector2decl.h"
#include "Vector3decl.h"
#include "Vector4decl.h"

typedef Vector3<float> Position;
typedef Vector2<float> Vector2f;
typedef Vector3<float> Vector3f;
typedef Vector4<float> Vector4f;

template<typename Type>
Vector3<Type> ToVector3(const Vector4<Type> &aVector4)
{
	return Vector3<Type>(aVector4.myX, aVector4.myY, aVector4.myZ);
}

template<typename Type>
Vector4<Type> ToVector4(const Vector3<Type> &aVector3, const Type& aW)
{
	return Vector4<Type>(aVector3.myX, aVector3.myY, aVector3.myZ, aW);
}
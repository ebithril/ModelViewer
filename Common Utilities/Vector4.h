#pragma once
#include "Vector4decl.h"
#include <assert.h>
#include <cmath>

template<typename Type>
bool operator==(const Vector4<Type> &aVector4, const Vector4<Type> &aSecondVector4)
{
	if (aVector4.myX == aSecondVector4.myX && aVector4.myY == aSecondVector4.myY &&
		aVector4.myZ == aSecondVector4.myZ && aVector4.myW == aSecondVector4.myW)
	{
		return true;
	}
	else
	{
		return false;
	}
}

template<typename Type>
bool operator!=(const Vector4<Type> &aVector4, const Vector4<Type> &aSecondVector4)
{
	if ((aVector4 == aSecondVector4) == true)
	{
		return false;
	}
	else
	{
		return true;
	}
}

//Vector operators
template<typename Type>
Vector4<Type> operator+(const Vector4<Type>& aVector4Left, const Vector4<Type>& aVector4Right) // Add vector with a vector
{
	Vector4<Type> newVector4;

	newVector4.myX = aVector4Left.myX + aVector4Right.myX;
	newVector4.myY = aVector4Left.myY + aVector4Right.myY;
	newVector4.myZ = aVector4Left.myZ + aVector4Right.myZ;
	newVector4.myW = aVector4Left.myW + aVector4Right.myW;

	return newVector4;
}

template<typename Type>
void operator+= (Vector4<Type> &aVector4Left, const Vector4<Type> &aVector4Right) // Add vector with a vector to the vector
{
	aVector4Left.myX += aVector4Right.myX;
	aVector4Left.myY += aVector4Right.myY;
	aVector4Left.myZ += aVector4Right.myZ;
	aVector4Left.myW += aVector4Right.myW;
}

template<typename Type>
Vector4<Type> operator- (const Vector4<Type> &aVector4Left, const Vector4<Type> &aVector4Right) // Subtract Vector with a vector
{
	Vector4<Type> newVector4;

	newVector4.myX = aVector4Left.myX - aVector4Right.myX;
	newVector4.myY = aVector4Left.myY - aVector4Right.myY;
	newVector4.myZ = aVector4Left.myZ - aVector4Right.myZ;
	newVector4.myW = aVector4Left.myW - aVector4Right.myW;

	return newVector4;
}

template<typename Type>
void operator-=(Vector4<Type> &aVector4Left, const Vector4<Type> &aVector4Right)  //Subtract vector with a vector to the vector
{
	aVector4Left.myX -= aVector4Right.myX;
	aVector4Left.myY -= aVector4Right.myY;
	aVector4Left.myZ -= aVector4Right.myZ;
	aVector4Left.myW -= aVector4Right.myW;
}
template<typename Type>
Vector4<Type> operator/(const Vector4<Type> &aVector4Left, const Vector4<Type> &aVector4Right) // Divivde a vector with a vector
{
	Vector4<Type> newVector4(aVector4Left);

	newVector4 /= aVector4Right;

	return newVector4;
}

template<typename Type>
void operator/=(Vector4<Type> &aVector4Left, const Vector4<Type> &aVector4Right) // Divide a vector with a vector and put data in vector
{
	assert(aVector4Right.myX != 0 && aVector4Right.myY != 0 && aVector4Right.myZ != 0 && aVector4Right.myW != 0 && "Can't divide a vector value by zero.");


	aVector4Left.myX /= aVector4Right.myX;
	aVector4Left.myY /= aVector4Right.myY;
	aVector4Left.myZ /= aVector4Right.myZ;
	aVector4Left.myW /= aVector4Right.myW;
}

template<typename Type>
Vector4<Type> operator*(const Vector4<Type> &aVector4Left, const Vector4<Type> &aVector4Right) // Multiply a vector with a vector
{
	Vector4<Type> newVector4;

	newVector4.myX = aVector4Left.myX * aVector4Right.myX;
	newVector4.myY = aVector4Left.myY * aVector4Right.myY;
	newVector4.myZ = aVector4Left.myZ * aVector4Right.myZ;
	newVector4.myW = aVector4Left.myW * aVector4Right.myW;

	return newVector4;
}

template<typename Type>
void operator*=(Vector4<Type> &aVector4Left, const Vector4<Type> &aVector4Right) // Multiply a vector with a vector and put data in vector
{
	aVector4Left.myX *= aVector4Right.myX;
	aVector4Left.myY *= aVector4Right.myY;
	aVector4Left.myZ *= aVector4Right.myZ;
	aVector4Left.myW *= aVector4Right.myW;
}

//Scalar operators
template<typename Type>
Vector4<Type> operator*(const Type &aScalar, const Vector4<Type> &aVector4)//Multiply vector with a scalar that has the same type as the vector
{
	Vector4<Type> newVector4;

	newVector4.myX = aScalar * aVector4.myX;
	newVector4.myY = aScalar * aVector4.myY;
	newVector4.myZ = aScalar * aVector4.myZ;
	newVector4.myW = aScalar * aVector4.myW;

	return newVector4;
}

template<typename Type>
Vector4<Type> operator*(const Vector4<Type> &aVector4, const Type &aScalar)
{
	return aScalar * aVector4;
}


template<typename Type>
void operator*=(Vector4<Type> &aVector4, const Type &aScalar) //Multiply vector with a scalar to the vector that has the same data type as the vector
{
	aVector4.myX *= aScalar;
	aVector4.myY *= aScalar;
	aVector4.myZ *= aScalar;
	aVector4.myW *= aScalar;
}

template<typename Type>
Vector4<Type> operator/(const Vector4<Type> &aVector4, const Type &aScalar)//Divide vector with a scalar that has the same type as the vector
{
	assert(aScalar != 0 && "Can't divide a vector value by zero.");

	Vector4<Type> newVector4;

	newVector4.myX = aVector4.myX / aScalar;
	newVector4.myY = aVector4.myY / aScalar;
	newVector4.myZ = aVector4.myZ / aScalar;
	newVector4.myW = aVector4.myW / aScalar;

	return newVector4;
}

template<typename Type>
void operator/=(Vector4<Type> &aVector4, const Type &aScalar) //Divide vector with a scalar that has the same type as the vector and put data in the vector
{
	assert(aScalar != 0 && "Can't divide a vector value by zero.");

	aVector4.myX /= aScalar;
	aVector4.myY /= aScalar;
	aVector4.myZ /= aScalar;
	aVector4.myW /= aScalar;
}

//Vector Functions

template<typename Type>
Type Length(const Vector4<Type> &aVector4) //Returns vector magnitude;
{
	return sqrt((aVector4.myX * aVector4.myX) + (aVector4.myY * aVector4.myY) + (aVector4.myZ * aVector4.myZ) + (aVector4.myW * aVector4.myW));
}

template<typename Type>
Type Length2(const Vector4<Type> &aVector4) 
{
	return ((aVector4.myX * aVector4.myX) + (aVector4.myY * aVector4.myY) + (aVector4.myZ * aVector4.myZ) + (aVector4.myW * aVector4.myW));
}

template<typename Type>
void Normalize(Vector4<Type> &aVector4) //Makes the vector a Unit Vector.
{
	const Type vectorLength = Length(aVector4);

	assert(vectorLength != 0 && "Can't divide a vector value by zero.");

	aVector4.myX /= vectorLength;
	aVector4.myY /= vectorLength;
	aVector4.myZ /= vectorLength;
	aVector4.myW /= vectorLength;
}

template<typename Type>
Vector4<Type> GetNormalized(const Vector4<Type> &aVector4)
{
	const Type vectorLength = Length(aVector4);
	Vector4<Type> newVector;

	assert(vectorLength != 0 && "Can't divide a vector value by zero.");

	newVector.myX = aVector4.myX / vectorLength;
	newVector.myY = aVector4.myY / vectorLength;
	newVector.myZ = aVector4.myZ / vectorLength;
	newVector.myW = aVector4.myW / vectorLength;

	return newVector;
}

template<typename Type>
Type Dot(Vector4<Type> aVector4First, Vector4<Type> aVector4Second)
{
	return ((aVector4First.myX * aVector4Second.myX) + (aVector4First.myY * aVector4Second.myY) + (aVector4First.myZ * aVector4Second.myZ) + (aVector4First.myW * aVector4Second.myW));
}
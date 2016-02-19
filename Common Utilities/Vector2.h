#pragma once
#include "Vector2decl.h"
#include <assert.h>
#include <cmath>

//Vector operators
template<typename Type>
Vector2<Type> operator+(const Vector2<Type>& aVector2Left, const Vector2<Type>& aVector2Right) // Add vector with a vector
{
	Vector2<Type> newVector2(aVector2Left);

	newVector2 += aVector2Right;

	return newVector2;
}

template<typename Type>
void operator+= (Vector2<Type> &aVector2Left, const Vector2<Type> &aVector2Right) // Add vector with a vector to the vector
{
	aVector2Left.myX += aVector2Right.myX;
	aVector2Left.myY += aVector2Right.myY;
}

template<typename Type>
Vector2<Type> operator- (const Vector2<Type> &aVector2Left, const Vector2<Type> &aVector2Right) // Subtract Vector with a vector
{
	Vector2<Type> newVector2;

	newVector2.myX = aVector2Left.myX - aVector2Right.myX;
	newVector2.myY = aVector2Left.myY - aVector2Right.myY;

	return newVector2;
}

template<typename Type>
void operator-=(Vector2<Type> &aVector2Left, const Vector2<Type> &aVector2Right)  //Subtract vector with a vector to the vector
{
	aVector2Left.myX -= aVector2Right.myX;
	aVector2Left.myY -= aVector2Right.myY;
}
template<typename Type>
Vector2<Type> operator/(const Vector2<Type> &aVector2Left, const Vector2<Type> &aVector2Right) // Divivde a vector with a vector
{
	assert(aVector2Right.myX != 0 && aVector2Right.myY != 0 && "Can't divide a vector value by zero.");
	Vector2<Type> newVector2;

	newVector2.myX = aVector2Left.myX / aVector2Right.myX;
	newVector2.myY = aVector2Left.myY / aVector2Right.myY;

	return newVector2;
}

template<typename Type>
void operator/=(Vector2<Type> &aVector2Left, const Vector2<Type> &aVector2Right) // Divide a vector with a vector and put data in vector
{
	assert(aVector2Right.myX != 0 && aVector2Right.myY != 0 && "Can't divide a vector value by zero.");
	aVector2Left.myX /= aVector2Right.myX;
	aVector2Left.myY /= aVector2Right.myY;
}

template<typename Type>
Vector2<Type> operator*(const Vector2<Type> &aVector2Left, const Vector2<Type> &aVector2Right) // Multiply a vector with a vector
{
	Vector2<Type> newVector2;

	newVector2.myX = aVector2Left.myX * aVector2Right.myX;
	newVector2.myY = aVector2Left.myY * aVector2Right.myY;

	return newVector2;
}

template<typename Type>
void operator*=(Vector2<Type> &aVector2Left, const Vector2<Type> &aVector2Right) // Multiply a vector with a vector and put data in vector
{
	aVector2Left.myX *= aVector2Right.myX;
	aVector2Left.myY *= aVector2Right.myY;
}

//Scalar operators
template<typename Type>
Vector2<Type> operator*(const Type &aScalar, const Vector2<Type> &aVector2)//Multiply vector with a scalar that has the same type as the vector
{
	Vector2<Type> newVector2;

	newVector2.myX = aScalar * aVector2.myX;
	newVector2.myY = aScalar * aVector2.myY;

	return newVector2;
}

template<typename Type>
Vector2<Type> operator*(const Vector2<Type> &aVector2, const Type &aScalar )
{
	return aScalar * aVector2;
}

template<typename Type>
bool operator==(const Vector2<Type> &aVector2, const Vector2<Type> &aSecondVector2)
{
	if (aVector2.myX == aSecondVector2.myX && aVector2.myY == aSecondVector2.myY)
	{
		return true;
	}
	else
	{
		return false;
	}
}

template<typename Type>
bool operator!=(const Vector2<Type> &aVector2, const Vector2<Type> &aSecondVector2)
{
	if ((aVector2 == aSecondVector2) == true)
	{
		return false;
	}
	else
	{
		return true;
	}
}


template<typename Type>
void operator*=(Vector2<Type> &aVector2, const Type &aScalar) //Multiply vector with a scalar to the vector that has the same data type as the vector
{
	aVector2.myX *= aScalar;
	aVector2.myY *= aScalar;
}

template<typename Type>
Vector2<Type> operator/(const Vector2<Type> &aVector2, const Type &aScalar)//Divide vector with a scalar that has the same type as the vector
{
	assert(aScalar != 0 && "Can't divide a vector value by zero.");

	Vector2<Type> newVector2;

	newVector2.myX = aVector2.myX / aScalar;
	newVector2.myY = aVector2.myY / aScalar;

	return newVector2;
}

template<typename Type>
void operator/=(Vector2<Type> &aVector2, const Type &aScalar) //Divide vector with a scalar that has the same type as the vector and put data in the vector
{
	assert(aScalar != 0 && "Can't divide a vector value by zero.");

	aVector2.myX /= aScalar;
	aVector2.myY /= aScalar;
}

//Vector Functions

template<typename Type>
Type Length(const Vector2<Type> &aVector2) //Returns vector magnitude;
{
	return sqrt((aVector2.myX * aVector2.myX) + (aVector2.myY * aVector2.myY));
}

template<typename Type>
Type Length2(const Vector2<Type> &aVector2) //Returnerar vectorns längd upphöjt med 2;
{
	return ((aVector2.myX * aVector2.myX) + (aVector2.myY * aVector2.myY));
}

template<typename Type>
void Normalize(Vector2<Type> &aVector2) //Makes the vector a Unit Vector.
{
	Type vectorLength = Length(aVector2);

	if (vectorLength == 0)
	{
		return;
	}

	aVector2.myX /= vectorLength;
	aVector2.myY /= vectorLength;
}

template<typename Type>
Vector2<Type> GetNormalized(const Vector2<Type> &aVector2)
{
	Type vectorLength = Length(aVector2);

	if (vectorLength == 0)
	{
		return aVector2;
	}

	Vector2<Type> newVector;

	newVector.myX =  aVector2.myX / vectorLength;
	newVector.myY = aVector2.myY / vectorLength;

	return newVector;
}

template<typename Type>
Type Dot(Vector2<Type> aVector2First, Vector2<Type> aVector2Second)		//Utför Dot produkten för vector med de inskickade vectorerna
{
	return ((aVector2First.myX * aVector2Second.myX) + (aVector2First.myY * aVector2Second.myY));
}
#pragma once

#include "Vector3decl.h"
#include <assert.h>
#include <cmath>
#include "DL_Debug.h"


template<typename Type>
bool operator==(const Vector3<Type> &aVector3, const Vector3<Type> &aSecondVector3)
{
	if (aVector3.myX == aSecondVector3.myX && aVector3.myY == aSecondVector3.myY &&
		aVector3.myZ == aSecondVector3.myZ)
	{
		return true;
	}
	else
	{
		return false;
	}
}

template<typename Type>
bool operator!=(const Vector3<Type> &aVector3, const Vector3<Type> &aSecondVector3)
{
	if ((aVector3 == aSecondVector3) == true)
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
Vector3<Type> operator+(const Vector3<Type>& aVector3Left, const Vector3<Type>& aVector3Right) // Add vector with a vector
{
	Vector3<Type> newVector3;

	newVector3.myX = aVector3Left.myX + aVector3Right.myX;
	newVector3.myY = aVector3Left.myY + aVector3Right.myY;
	newVector3.myZ = aVector3Left.myZ + aVector3Right.myZ;

	return newVector3;
}

template<typename Type>
void operator+= (Vector3<Type> &aVector3Left, const Vector3<Type> &aVector3Right) // Add vector with a vector to the vector
{
	aVector3Left.myX += aVector3Right.myX;
	aVector3Left.myY += aVector3Right.myY;
	aVector3Left.myZ += aVector3Right.myZ;
}

template<typename Type>
Vector3<Type> operator- (const Vector3<Type> &aVector3Left, const Vector3<Type> &aVector3Right) // Subtract Vector with a vector
{
	Vector3<Type> newVector3;

	newVector3.myX = aVector3Left.myX - aVector3Right.myX;
	newVector3.myY = aVector3Left.myY - aVector3Right.myY;
	newVector3.myZ = aVector3Left.myZ - aVector3Right.myZ;

	return newVector3;
}

template<typename Type>
void operator-=(Vector3<Type> &aVector3Left, const Vector3<Type> &aVector3Right)  //Subtract vector with a vector to the vector
{
	aVector3Left.myX -= aVector3Right.myX;
	aVector3Left.myY -= aVector3Right.myY;
	aVector3Left.myZ -= aVector3Right.myZ;
}
template<typename Type>
Vector3<Type> operator/(const Vector3<Type> &aVector3Left, const Vector3<Type> &aVector3Right) // Divivde a vector with a vector
{
	assert(aVector3Right.myX != 0 && aVector3Right.myY != 0 && aVector3Right.myZ != 0 && "Can't divide a vector value by zero.");
	Vector3<Type> newVector3;

	newVector3.myX = aVector3Left.myX / aVector3Right.myX;
	newVector3.myY = aVector3Left.myY / aVector3Right.myY;
	newVector3.myZ = aVector3Left.myZ / aVector3Right.myZ;

	return newVector3;
}

template<typename Type>
void operator/=(Vector3<Type> &aVector3Left, const Vector3<Type> &aVector3Right) // Divide a vector with a vector and put data in vector
{
	assert(aVector3Right.myX != 0 && aVector3Right.myY != 0 && aVector3Right.myZ != 0 && "Can't divide a vector value by zero.");
	aVector3Left.myX /= aVector3Right.myX;
	aVector3Left.myY /= aVector3Right.myY;
	aVector3Left.myZ /= aVector3Right.myZ;
}

template<typename Type>
Vector3<Type> operator*(const Vector3<Type> &aVector3Left, const Vector3<Type> &aVector3Right) // Multiply a vector with a vector
{
	Vector3<Type> newVector3;

	newVector3.myX = aVector3Left.myX * aVector3Right.myX;
	newVector3.myY = aVector3Left.myY * aVector3Right.myY;
	newVector3.myZ = aVector3Left.myZ * aVector3Right.myZ;

	return newVector3;
}

template<typename Type>
void operator*=(Vector3<Type> &aVector3Left, const Vector3<Type> &aVector3Right) // Multiply a vector with a vector and put data in vector
{
	aVector3Left.myX *= aVector3Right.myX;
	aVector3Left.myY *= aVector3Right.myY;
	aVector3Left.myZ *= aVector3Right.myZ;

}

//Scalar operators
template<typename Type>
Vector3<Type> operator*(const Type &aScalar, const Vector3<Type> &aVector3)//Multiply vector with a scalar that has the same type as the vector
{
	Vector3<Type> newVector3;

	newVector3.myX = aScalar * aVector3.myX;
	newVector3.myY = aScalar * aVector3.myY;
	newVector3.myZ = aScalar * aVector3.myZ;

	return newVector3;
}

template<typename Type>
Vector3<Type> operator*(const Vector3<Type> &aVector3, const Type &aScalar)
{
	return aScalar * aVector3;
}


template<typename Type>
void operator*=(Vector3<Type> &aVector3, const Type &aScalar) //Multiply vector with a scalar to the vector that has the same data type as the vector
{
	aVector3.myX *= aScalar;
	aVector3.myY *= aScalar;
	aVector3.myZ *= aScalar;

}

template<typename Type>
Vector3<Type> operator/(const Vector3<Type> &aVector3, const Type &aScalar)//Divide vector with a scalar that has the same type as the vector
{
	assert(aScalar != 0 && "Can't divide a vector value by zero.");

	Vector3<Type> newVector3;

	newVector3.myX = aVector3.myX / aScalar;
	newVector3.myY = aVector3.myY / aScalar;
	newVector3.myZ = aVector3.myZ / aScalar;

	return newVector3;
}

template<typename Type>
void operator/=(Vector3<Type> &aVector3, const Type &aScalar) //Divide vector with a scalar that has the same type as the vector and put data in the vector
{
	assert(aScalar != 0 && "Can't divide a vector value by zero.");

	aVector3.myX /= aScalar;
	aVector3.myY /= aScalar;
	aVector3.myZ /= aScalar;
}

//Vector Functions

template<typename Type>
Type Length(const Vector3<Type> &aVector3) //Returns vector magnitude;
{
	return sqrt((aVector3.myX * aVector3.myX) + (aVector3.myY * aVector3.myY) + (aVector3.myZ * aVector3.myZ));
}

template<typename Type>
Type Length2(const Vector3<Type> &aVector3) //Returnerar vectorns längd upphöjt med 2;
{
	return ((aVector3.myX * aVector3.myX) + (aVector3.myY * aVector3.myY) + (aVector3.myZ * aVector3.myZ));
}

template<typename Type>
void Normalize(Vector3<Type> &aVector3) //Makes the vector a Unit Vector.
{
	const Type vectorLength = Length(aVector3);
	if (vectorLength == 0)
	{
		return;
		//DL_ASSERT("Kek");
	}

	assert(vectorLength != 0 && "Can't divide a vector value by zero.");

	aVector3.myX /= vectorLength;
	aVector3.myY /= vectorLength;
	aVector3.myZ /= vectorLength;
}

template<typename Type>
Vector3<Type> GetNormalized(const Vector3<Type> &aVector3)
{
	const Type vectorLength = Length(aVector3);
	Vector3<Type> newVector(0, 0, 0);

	if (vectorLength == 0)
	{
		return newVector;
	}

	newVector.myX = aVector3.myX / vectorLength;
	newVector.myY = aVector3.myY / vectorLength;
	newVector.myZ = aVector3.myZ / vectorLength;

	return newVector;
}

template<typename Type>
Type Dot(Vector3<Type> aVector3First, Vector3<Type> aVector3Second)		//Utför Dot produkten för vector med de inskickade vectorerna
{
	return ((aVector3First.myX * aVector3Second.myX) + (aVector3First.myY * aVector3Second.myY) + (aVector3First.myZ * aVector3Second.myZ));
}

template<typename Type>
Vector3<Type> Cross(const Vector3<Type> &aVector3First, const Vector3<Type> &aVector3Second) //Utför cross produkten mellan de inskickade vectorerna och returnerar den, Denna behöver bara göras för 3D.
{
	Vector3<Type> newVector3;

	newVector3.myX = ((aVector3First.myY * aVector3Second.myZ) - (aVector3First.myZ * aVector3Second.myY));
	newVector3.myY = ((aVector3First.myZ * aVector3Second.myX) - (aVector3First.myX * aVector3Second.myZ));
	newVector3.myZ = ((aVector3First.myX * aVector3Second.myY) - (aVector3First.myY * aVector3Second.myX));

	return newVector3;
}


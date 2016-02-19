#pragma once
#include "Matrix33decl.h"
#include "Vector.h"

//Matrix operators
MATRIX33_TEMPLATE
MATRIX33_OBJECT operator+(const MATRIX33_OBJECT& aLeftValue, const MATRIX33_OBJECT& aRightValue)
{
	MATRIX33_OBJECT newMatrix;
	for (int i = 0; i < 9; i++)
	{
		newMatrix.myMatrix[i] = aLeftValue.myMatrix[i] + aRightValue.myMatrix[i];
	}
	return newMatrix;
}

MATRIX33_TEMPLATE
MATRIX33_OBJECT operator+=(MATRIX33_OBJECT& aLeftValue, const MATRIX33_OBJECT& aRightValue)
{
	for (int i = 0; i < 9; i++)
	{
		aLeftValue.myMatrix[i] += aRightValue.myMatrix[i];
	}
	return aLeftValue;
}

MATRIX33_TEMPLATE
MATRIX33_OBJECT operator-(const MATRIX33_OBJECT& aLeftValue, const MATRIX33_OBJECT& aRightValue)
{
	MATRIX33_OBJECT newMatrix;
	for (int i = 0; i < 9; i++)
	{
		newMatrix.myMatrix[i] = aLeftValue.myMatrix[i] - aRightValue.myMatrix[i];
	}
	return newMatrix;
}

MATRIX33_TEMPLATE
MATRIX33_OBJECT operator-=(MATRIX33_OBJECT& aLeftValue, const MATRIX33_OBJECT& aRightValue)
{
	for (int i = 0; i < 9; i++)
	{
		aLeftValue.myMatrix[i] -= aRightValue.myMatrix[i];
	}
	return aLeftValue;
}

MATRIX33_TEMPLATE
MATRIX33_OBJECT operator*(const MATRIX33_OBJECT& aLeftValue, const MATRIX33_OBJECT& aRightValue)
{
	MATRIX33_OBJECT newMatrix;
	for (unsigned short i = 0; i < 3; i++)
	{
		Vector3<Type> aRow;
		aRow.myX = Dot(aLeftValue.GetRow(i), aRightValue.GetColumn(0));
		aRow.myY = Dot(aLeftValue.GetRow(i), aRightValue.GetColumn(1));
		aRow.myZ = Dot(aLeftValue.GetRow(i), aRightValue.GetColumn(2));
		newMatrix.SetRow(i, aRow);
	}
	return newMatrix;
}

MATRIX33_TEMPLATE
MATRIX33_OBJECT operator*=(MATRIX33_OBJECT& aLeftValue, const MATRIX33_OBJECT& aRightValue)
{
	aLeftValue = aLeftValue * aRightValue;
		return aLeftValue;
}

MATRIX33_TEMPLATE
Vector3<Type> operator*(const Vector3<Type>& aLeftValue, const MATRIX33_OBJECT& aRightValue)
{
	Vector3<Type> newVector3;

	newVector3.myX = Dot(aLeftValue, aRightValue.GetColumn(0));
	newVector3.myY = Dot(aLeftValue, aRightValue.GetColumn(1));
	newVector3.myZ = Dot(aLeftValue, aRightValue.GetColumn(2));

	return newVector3;
}

MATRIX33_TEMPLATE
Vector3<Type> operator*=(Vector3<Type>& aLeftValue, const MATRIX33_OBJECT& aRightValue)
{
	return aLeftValue = aLeftValue * aRightValue;
}

MATRIX33_TEMPLATE
bool operator==(const MATRIX33_OBJECT& aLeftValue, const MATRIX33_OBJECT& aRightValue)
{
	for (int i = 0; i < 9; i++)
	{
		if (aLeftValue.myMatrix[i] != aRightValue.myMatrix[i])
		{
			return false;
		}
	}
	return true;
}

MATRIX33_TEMPLATE
MATRIX33_OBJECT operator!=(const MATRIX33_OBJECT& aLeftValue, const MATRIX33_OBJECT& aRightValue)
{
	if (aLeftValue == aRightValue)
	{
		return false;
	}
	else
	{
		return true;
	}
}

MATRIX33_TEMPLATE
Vector2<Type> operator*(const Vector2<Type>& aLeftValue, const MATRIX33_OBJECT& aRightValue)
{
	Vector3<Type> newVector3(aLeftValue.myX, aLeftValue.myY, 1);
	newVector3 *= aRightValue;
	return Vector2<Type>(newVector3.myX, newVector3.myY);
}
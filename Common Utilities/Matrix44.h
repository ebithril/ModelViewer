#pragma once
#include "Matrix44decl.h"
#include "Matrix33decl.h"
#include <intrin.h>

//Matrix operators
MATRIX44_TEMPLATE
MATRIX44_OBJECT operator+(const MATRIX44_OBJECT& aLeftValue, const MATRIX44_OBJECT& aRightValue)
{
	MATRIX44_OBJECT newMatrix;
	for (int i = 0; i < 16; i++)
	{
		newMatrix.myMatrix[i] = aLeftValue.myMatrix[i] + aRightValue.myMatrix[i];
	}
	return newMatrix;
}

MATRIX44_TEMPLATE
MATRIX44_OBJECT operator+=(MATRIX44_OBJECT& aLeftValue, const MATRIX44_OBJECT& aRightValue)
{
	for (int i = 0; i < 16; i++)
	{
		aLeftValue.myMatrix[i] += aRightValue.myMatrix[i];
	}
	return aLeftValue;
}

MATRIX44_TEMPLATE
MATRIX44_OBJECT operator-(const MATRIX44_OBJECT& aLeftValue, const MATRIX44_OBJECT& aRightValue)
{
	MATRIX44_OBJECT newMatrix;
	for (int i = 0; i < 16; i++)
	{
		newMatrix.myMatrix[i] = aLeftValue.myMatrix[i] - aRightValue.myMatrix[i];
	}
	return newMatrix;
}

MATRIX44_TEMPLATE
MATRIX44_OBJECT operator-=(MATRIX44_OBJECT& aLeftValue, const MATRIX44_OBJECT& aRightValue)
{
	for (int i = 0; i < 16; i++)
	{
		aLeftValue.myMatrix[i] -= aRightValue.myMatrix[i];
	}
	return aLeftValue;
}

MATRIX44_TEMPLATE
MATRIX44_OBJECT operator*(const MATRIX44_OBJECT& aLeftValue, const MATRIX44_OBJECT& aRightValue)
{
	/*MATRIX44_OBJECT leftValue = aLeftValue;

	MATRIX44_OBJECT rightValue = aRightValue;

	__m128 row1 = _mm_load_ps(&rightValue.myMatrix[0]);
	__m128 row2 = _mm_load_ps(&rightValue.myMatrix[4]);
	__m128 row3 = _mm_load_ps(&rightValue.myMatrix[8]);
	__m128 row4 = _mm_load_ps(&rightValue.myMatrix[12]);
	 MATRIX44_OBJECT newMatrix = Matrix44f();
	for (int i = 0; i < 4; i++)
	{
		__m128 brod1 = _mm_set1_ps(leftValue.myMatrix[4 * i + 0]);
		__m128 brod2 = _mm_set1_ps(leftValue.myMatrix[4 * i + 1]);
		__m128 brod3 = _mm_set1_ps(leftValue.myMatrix[4 * i + 2]);
		__m128 brod4 = _mm_set1_ps(leftValue.myMatrix[4 * i + 3]);
		__m128 row = _mm_add_ps(
			_mm_add_ps(
			_mm_mul_ps(brod1, row1),
			_mm_mul_ps(brod2, row2)),
			_mm_add_ps(
			_mm_mul_ps(brod3, row3),
			_mm_mul_ps(brod4, row4)));
		_mm_store_ps(&newMatrix.myMatrix[4 * i], row);
	}

	return newMatrix;*/

	MATRIX44_OBJECT newMatrix;
	for (unsigned short i = 0; i < 4; i++)
	{
		Vector4<Type> aRow;
		aRow.myX = Dot(aLeftValue.GetRow(i), aRightValue.GetColumn(0));
		aRow.myY = Dot(aLeftValue.GetRow(i), aRightValue.GetColumn(1));
		aRow.myZ = Dot(aLeftValue.GetRow(i), aRightValue.GetColumn(2));
		aRow.myW = Dot(aLeftValue.GetRow(i), aRightValue.GetColumn(3));
		newMatrix.SetRow(i, aRow);
	}
	return newMatrix;
}

MATRIX44_TEMPLATE
MATRIX44_OBJECT SlowMatrixMultiplication(const MATRIX44_OBJECT& aLeftValue, const MATRIX44_OBJECT& aRightValue)
{
	MATRIX44_OBJECT newMatrix;
	for (unsigned short i = 0; i < 4; i++)
	{
	Vector4<Type> aRow;
	aRow.myX = Dot(aLeftValue.GetRow(i), aRightValue.GetColumn(0));
	aRow.myY = Dot(aLeftValue.GetRow(i), aRightValue.GetColumn(1));
	aRow.myZ = Dot(aLeftValue.GetRow(i), aRightValue.GetColumn(2));
	aRow.myW = Dot(aLeftValue.GetRow(i), aRightValue.GetColumn(3));
	newMatrix.SetRow(i, aRow);
	}
	return newMatrix;
}


MATRIX44_TEMPLATE
MATRIX44_OBJECT operator*=(MATRIX44_OBJECT& aLeftValue, const MATRIX44_OBJECT& aRightValue)
{
	return aLeftValue = (aLeftValue * aRightValue);
}

MATRIX44_TEMPLATE
Vector4<Type> operator*(const Vector4<Type>& aLeftValue, const MATRIX44_OBJECT& aRightValue)
{
	Vector4<Type> newVector4;

	newVector4.myX = Dot(aLeftValue, aRightValue.GetColumn(0));
	newVector4.myY = Dot(aLeftValue, aRightValue.GetColumn(1));
	newVector4.myZ = Dot(aLeftValue, aRightValue.GetColumn(2));
	newVector4.myW = Dot(aLeftValue, aRightValue.GetColumn(3));

	return newVector4;
}

MATRIX44_TEMPLATE
Vector4<Type> operator*=(Vector4<Type>& aLeftValue, const MATRIX44_OBJECT& aRightValue)
{
	return aLeftValue = aLeftValue * aRightValue;
}

MATRIX44_TEMPLATE
bool operator==(const MATRIX44_OBJECT& aLeftValue, const MATRIX44_OBJECT& aRightValue)
{
	for (int i = 0; i < 16; i++)
	{
		if (aLeftValue.myMatrix[i] != aRightValue.myMatrix[i])
		{
			return false;
		}
	}
	return true;
}

MATRIX44_TEMPLATE
bool operator!=(const MATRIX44_OBJECT& aLeftValue, const MATRIX44_OBJECT& aRightValue)
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

MATRIX44_TEMPLATE
MATRIX44_OBJECT ConvertMatrix(const Matrix33<Type>& aRightMatrix)
{
	MATRIX44_OBJECT newMatrix;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			newMatrix[(4 * i) + j] = aRightMatrix.myMatrix[(i * 3) + j];
		}
	}

	return newMatrix;
}

template <typename T>
Matrix44<T> operator*(const Matrix44<T>& aMatrix, float aScalar)
{
	Matrix44<T> result(aMatrix);
	result *= aScalar;
	return result;
}

template <typename T>
Matrix44<T> operator*=(Matrix44<T>& aMatrix, float aScalar)
{
	aMatrix.myMatrix[0] *= aScalar;
	aMatrix.myMatrix[1] *= aScalar;
	aMatrix.myMatrix[2] *= aScalar;
	aMatrix.myMatrix[3] *= aScalar;

	aMatrix.myMatrix[4] *= aScalar;
	aMatrix.myMatrix[5] *= aScalar;
	aMatrix.myMatrix[6] *= aScalar;
	aMatrix.myMatrix[7] *= aScalar;

	aMatrix.myMatrix[8] *= aScalar;
	aMatrix.myMatrix[9] *= aScalar;
	aMatrix.myMatrix[10] *= aScalar;
	aMatrix.myMatrix[11] *= aScalar;

	aMatrix.myMatrix[12] *= aScalar;
	aMatrix.myMatrix[13] *= aScalar;
	aMatrix.myMatrix[14] *= aScalar;
	aMatrix.myMatrix[15] *= aScalar;

	return aMatrix;
}
#pragma once
#include "Matrix44decl.h"
#include "Vector3.h"
#include "Vector2.h"

#define MATRIX33_TEMPLATE template <typename Type>
#define MATRIX33_OBJECT Matrix33<Type>

MATRIX33_TEMPLATE
struct Matrix33
{
private:
	static const short mySize = 9;
public:

	Matrix33();
	Matrix33(const Matrix33<Type>& aMatrix);
	Matrix33(const Matrix44<Type>& aMatrix); // initsialiserar 3x3 matrisen med 3x3 delen av 4x4 
	static Matrix33<Type> Transpose(const Matrix33<Type>& aMatrixToTranspose);
	static Matrix33<Type> CreateRotateAroundX(const Type& aAngleInRadians);
	static Matrix33<Type> CreateRotateAroundY(const Type& aAngleInRadians);
	static Matrix33<Type> CreateRotateAroundZ(const Type& aAngleInRadians);
	static Matrix33<Type> GetInverse(const Matrix33<Type>& aMatrixToInverse);
	Vector3<Type> GetColumn(const unsigned short aColumnNumber) const;
	Vector3<Type> GetRow(const unsigned short aRowNumber) const;
	void SetColumn(const unsigned short aColumnNumber, const Vector3<Type>& aVector3);
	void SetRow(const unsigned short aRowNumber, const Vector3<Type>& aVector3);
	void SetRotation2D(const Type& aRadian);
	void Rotate2D(const Type& aRadian);
	Vector3<Type> GetTranslation()const;
	void SetTranslation(const Vector3<Type>& aVector3);

	Vector2<Type> GetPosition() const;
	void SetPosition(const Vector2<Type>& aPosition);

	Vector3<Type> ConvertVector4ToVector3(const Vector4<Type>& aVector4);
	Matrix33<Type> operator=(const Matrix44<Type>& aRightValue);
	Matrix33<Type> operator=(const Matrix33<Type>& aRightValue);
	static Matrix33<Type> CreateReflectionMatrixAboutAxis(Vector3<Type> reflectionVector);
	CU::StaticArray<Type , mySize> myMatrix;
};

MATRIX33_TEMPLATE
MATRIX33_OBJECT MATRIX33_OBJECT::CreateReflectionMatrixAboutAxis(Vector3<Type> reflectionVector)
{
	Matrix33<Type> reflectionMatrix;
	reflectionMatrix.myMatrix[0] = 1 - 2 * (reflectionVector.x*reflectionVector.x);
	reflectionMatrix.myMatrix[1] = -2 * (reflectionVector.x*reflectionVector.y);
	reflectionMatrix.myMatrix[2] = -2 * (reflectionVector.x*reflectionVector.z);
					
	reflectionMatrix.myMatrix[3] = -2 * (reflectionVector.y*reflectionVector.x);
	reflectionMatrix.myMatrix[4] = 1 - 2 * (reflectionVector.y*reflectionVector.y);
	reflectionMatrix.myMatrix[5] = -2 * (reflectionVector.y*reflectionVector.z);
					
	reflectionMatrix.myMatrix[6] = -2 * (reflectionVector.z*reflectionVector.x);
	reflectionMatrix.myMatrix[7] = -2 * (reflectionVector.z*reflectionVector.y);
	reflectionMatrix.myMatrix[8] = 1 - 2 * (reflectionVector.z*reflectionVector.z);
	return reflectionMatrix;
};

MATRIX33_TEMPLATE
Vector3<Type> MATRIX33_OBJECT::ConvertVector4ToVector3(const Vector4<Type>& aVector4)
{
	return Vector3<Type>(aVector4.myX, aVector4.myY, aVector4.myZ);
}

MATRIX33_TEMPLATE
Vector3<Type> MATRIX33_OBJECT::GetRow(const unsigned short aRowNumber) const
{
	switch (aRowNumber)
	{
	case 0:
		return Vector3<Type>(
			myMatrix[0],
			myMatrix[1],
			myMatrix[2]);
		break;

	case 1:
		return Vector3<Type>(
			myMatrix[3],
			myMatrix[4],
			myMatrix[5]);
		break;

	case 2:
		return Vector3<Type>(
			myMatrix[6],
			myMatrix[7],
			myMatrix[8]);
		break;

	default:
		assert(false && "There is no Row with that number");
		return Vector3<Type>();
		break;
	}
}

MATRIX33_TEMPLATE
Vector3<Type> MATRIX33_OBJECT::GetColumn(const unsigned short aColumnNumber) const
{
	switch (aColumnNumber)
	{
	case 0:
		return Vector3<Type>(
			myMatrix[0],
			myMatrix[3],
			myMatrix[6]);
		break;

	case 1:
		return Vector3<Type>(
			myMatrix[1],
			myMatrix[4],
			myMatrix[7]);
		break;

	case 2:
		return Vector3<Type>(
			myMatrix[2],
			myMatrix[5],
			myMatrix[8]);
		break;

	default:
		assert(false && "There is no Column with that number");
		return Vector3<Type>();
		break;
	}
}

MATRIX33_TEMPLATE
void MATRIX33_OBJECT::SetRow(const unsigned short aRowNumber, const Vector3<Type>& aVector3)
{
	switch (aRowNumber)
	{
	case 0:

		myMatrix[0] = aVector3.myX;
		myMatrix[1] = aVector3.myY;
		myMatrix[2] = aVector3.myZ;
		break;

	case 1:

		myMatrix[3] = aVector3.myX;
		myMatrix[4] = aVector3.myY;
		myMatrix[5] = aVector3.myZ;

	case 2:

		myMatrix[6] = aVector3.myX;
		myMatrix[7] = aVector3.myY;
		myMatrix[8] = aVector3.myZ;
		break;

	default:
		assert(false && "There is no Row with that number");
		break;
	}
}

MATRIX33_TEMPLATE
void MATRIX33_OBJECT::SetColumn(const unsigned short aColumnNumber, const Vector3<Type>& aVector3)
{
	switch (aColumnNumber)
	{
	case 0:

		myMatrix[0] = aVector3.myX;
		myMatrix[3] = aVector3.myY;
		myMatrix[6] = aVector3.myZ;
		break;

	case 1:

		myMatrix[1] = aVector3.myX;
		myMatrix[4] = aVector3.myY;
		myMatrix[7] = aVector3.myZ;

	case 2:

		myMatrix[2] = aVector3.myX;
		myMatrix[5] = aVector3.myY;
		myMatrix[8] = aVector3.myZ;
		break;

	default:
		assert(false && "There is no Column with that number");
		break;
	}
}

MATRIX33_TEMPLATE
MATRIX33_OBJECT::Matrix33()//Initialized to Identity Matrix
{
	SetRow(0, Vector3<Type>(1, 0, 0));
	SetRow(1, Vector3<Type>(0, 1, 0));
	SetRow(2, Vector3<Type>(0, 0, 1));
}

MATRIX33_TEMPLATE
MATRIX33_OBJECT MATRIX33_OBJECT::operator=(const MATRIX33_OBJECT& aRightValue)
{
	this->SetRow(0,aRightValue.GetRow(0));
	this->SetRow(1,aRightValue.GetRow(1));
	this->SetRow(2,aRightValue.GetRow(2));
	return *this;
}

MATRIX33_TEMPLATE
MATRIX33_OBJECT::Matrix33(const Matrix33<Type>& aMatrix)
{
	this->SetRow(0, aMatrix.GetRow(0));
	this->SetRow(1, aMatrix.GetRow(1));
	this->SetRow(2, aMatrix.GetRow(2));
}

MATRIX33_TEMPLATE
MATRIX33_OBJECT MATRIX33_OBJECT::operator=(const Matrix44<Type>& aRightValue)
{
	return MATRIX33_OBJECT(aRightValue);

}

MATRIX33_TEMPLATE
MATRIX33_OBJECT::Matrix33(const Matrix44<Type>& aMatrix)
{
	this->SetRow(0, ConvertVector4ToVector3(aMatrix.GetRow(0)));
	this->SetRow(1, ConvertVector4ToVector3(aMatrix.GetRow(1)));
	this->SetRow(2, ConvertVector4ToVector3(aMatrix.GetRow(2)));
}

MATRIX33_TEMPLATE
Matrix33<Type> MATRIX33_OBJECT::Transpose(const Matrix33<Type>& aMatrixToTranspose)
{
	Matrix33<Type> aTransposedMatrix;

	aTransposedMatrix.SetRow(0, aMatrixToTranspose.GetColumn(0));
	aTransposedMatrix.SetRow(1, aMatrixToTranspose.GetColumn(1));
	aTransposedMatrix.SetRow(2, aMatrixToTranspose.GetColumn(2));

	return aTransposedMatrix;
}

MATRIX33_TEMPLATE
MATRIX33_OBJECT MATRIX33_OBJECT::CreateRotateAroundX(const Type& aAngleInRadians)
{
	Matrix33<Type> newMatrix(MATRIX44_OBJECT::CreateRotateAroundX(aAngleInRadians));
	return newMatrix;

}

MATRIX33_TEMPLATE
MATRIX33_OBJECT MATRIX33_OBJECT::CreateRotateAroundY(const Type& aAngleInRadians)
{
	return Matrix33<Type>(MATRIX44_OBJECT::CreateRotateAroundY(aAngleInRadians));
}

MATRIX33_TEMPLATE
MATRIX33_OBJECT MATRIX33_OBJECT::CreateRotateAroundZ(const Type& aAngleInRadians)
{
	return Matrix33<Type>(MATRIX44_OBJECT::CreateRotateAroundZ(aAngleInRadians));
}

MATRIX33_TEMPLATE
Matrix33<Type> MATRIX33_OBJECT::GetInverse(const Matrix33<Type>& aMatrixToInverse)
{
	Vector3<Type> myTranslation = aMatrixToInverse.GetTranslation();
	Matrix33<Type> inverseMatrix = aMatrixToInverse;
	inverseMatrix.SetTranslation(Vector3<Type>(0, 0, 1));
	myTranslation *= -1.0f;
	myTranslation.myZ = 1;
	inverseMatrix = Transpose(inverseMatrix);
	myTranslation *= inverseMatrix;
	inverseMatrix.SetTranslation(myTranslation);
	return inverseMatrix;
}

MATRIX33_TEMPLATE
void MATRIX33_OBJECT::SetRotation2D(const Type& aAngleInRadians)
{
	Type cosinus = cos(aAngleInRadians);
	Type sinus = sin(aAngleInRadians);

	myMatrix[0] = cosinus;
	myMatrix[1] = sinus;
	myMatrix[3] = -sinus;
	myMatrix[4] = cosinus;
}

MATRIX33_TEMPLATE
void MATRIX33_OBJECT::Rotate2D(const Type& aAngleInRadians)
{
	Matrix33<Type> rotationMatrix = Matrix33::CreateRotateAroundZ(aAngleInRadians);

	Matrix33<Type> newMatrix = *this;
	newMatrix *= rotationMatrix;

	myMatrix[0] = newMatrix.myMatrix[0];
	myMatrix[1] = newMatrix.myMatrix[1];
	myMatrix[3] = newMatrix.myMatrix[3];
	myMatrix[4] = newMatrix.myMatrix[4];
}

MATRIX33_TEMPLATE
Vector3<Type> MATRIX33_OBJECT::GetTranslation() const
{
	return GetRow(2);
}

MATRIX33_TEMPLATE
void MATRIX33_OBJECT::SetTranslation(const Vector3<Type>& aVector3)
{
	SetRow(2, aVector3);
}

MATRIX33_TEMPLATE
Vector2<Type> MATRIX33_OBJECT::GetPosition() const
{
	return Vector2<Type> (myMatrix[6], myMatrix[7]);
}

MATRIX33_TEMPLATE
void MATRIX33_OBJECT::SetPosition(const Vector2<Type>& aPosition)
{
	myMatrix[6] = aPosition.myX;
	myMatrix[7] = aPosition.myY;
}

#pragma once
#include "Vector.h"
#include <assert.h>
#include "StaticArray.h"
#include "DL_Assert.h"


#define MATRIX44_TEMPLATE template <typename Type>
#define MATRIX44_OBJECT Matrix44<Type>

template <typename Type>

struct Matrix44
{

public:
	union
	{
		//__myMatrix[1]8 myAlign[4];

		Type myMatrix[16];
	};

	Matrix44();
	Matrix44(const Matrix44<Type>& aMatrix);
	Matrix44<Type> Transpose(const Matrix44<Type>& aMatrixToTranspose) const;
	static Matrix44<Type> CreateRotateAroundX(Type aAngleInRadians);
	static Matrix44<Type> CreateRotateAroundY(Type aAngleInRadians);
	static Matrix44<Type> CreateRotateAroundZ(Type aAngleInRadians);
	static Matrix44<Type> CreateProjectionMatrixLH(const Type aNearZ, const Type aFarZ, const Type aAspectRatio, const Type aFovAngleRadians, const Type aScale);
	static Matrix44<Type> CreateMatrixFromForwardVector(const Vector3<Type>& aForwardVector);
	static Matrix44<Type> CreateBiasMatrix();
	Vector4<Type> GetColumn(const unsigned short aColumnNumber) const;
	Vector4<Type> GetRow(const unsigned short aRowNumber) const;
	void SetColumn(const unsigned short aColumnNumber, const Vector4<Type>& aVector4);
	void SetRow(const unsigned short aRowNumber, const Vector4<Type>& aVector4);
	Matrix44<Type> operator=(const Matrix44<Type>& aRightValue);
	const Matrix44<Type> GetInverse() const;
	const Matrix44<Type> GetCorrectInverse() const;
	const float CalculateDeterminant() const;
	const Vector3<Type> GetTranslation() const;
	void SetTranslation(const Vector3<Type>& aTranslation);
	void Move(const Vector3<Type>& aRelativePosition);
	void Scale(const Vector3<Type>& aScaleVector);

	void Rotate(const Vector3<Type>& rotationVectorInRadians);
	void RotateAroundAxis(const float anAngleInRadians, float u, float v, float w);
	void TranslationRotation(const Vector3<Type>& rotationVectorInRadians, const Position& aPosition);

	void SetRotationWithQuaternion(const Vector4<Type>& aQuarternion);

	float& operator[](const unsigned short anIndex);
	const float& operator[](const unsigned short anIndex) const;

	const Vector3<Type> GetForwardVector() const;

	const Vector3<Type> GetRightVector() const;
	const Vector3<Type> GetUpVector() const;

	void LookAt(const Vector3<Type>& aDirection, const Vector3<Type>& anUpVector = Vector3<Type>(0, 1, 0));
};

MATRIX44_TEMPLATE
MATRIX44_OBJECT MATRIX44_OBJECT::CreateBiasMatrix()
{
	Matrix44<Type> newMatrix;
	newMatrix.SetRow(0, Vector4<Type>(0.5f, 0, 0, 0.5f));
	newMatrix.SetRow(1, Vector4<Type>(0, 0.5f, 0, 0.5f));
	newMatrix.SetRow(2, Vector4<Type>(0, 0, 0.5f, 0.5f));
	newMatrix.SetRow(3, Vector4<Type>(0, 0, 0, 1.f));

	return newMatrix;
}


MATRIX44_TEMPLATE
const Vector3<Type> MATRIX44_OBJECT::GetUpVector() const
{
	Vector4<Type> tempVector = GetRow(1);
	Vector3<Type> upVector{ tempVector.myX, tempVector.myY, tempVector.myZ };

	return upVector;
}

MATRIX44_TEMPLATE
const Vector3<Type> MATRIX44_OBJECT::GetRightVector() const
{
	Vector4<Type> tempVector = GetRow(0);
	Vector3<Type> rightVector{ tempVector.myX, tempVector.myY, tempVector.myZ };

	return rightVector;
}

MATRIX44_TEMPLATE
void MATRIX44_OBJECT::SetRotationWithQuaternion(const Vector4<Type>& aQuarternion)
{
	Type x = aQuarternion.myX;
	Type y = aQuarternion.myY;
	Type z = aQuarternion.myZ;
	Type w = aQuarternion.myW;

	Type x2 = x * x;
	Type y2 = y * y;
	Type z2 = z * z;
	//	Type w2 = w * w;

	SetRow(0, Vector4<Type>(1 - 2 * y2 - 2 * z2, 2 * x * y + 2 * w*z, 2 * x*z - 2 * w*y, 0));
	SetRow(1, Vector4<Type>(2 * x*y - 2 * w*z, 1 - 2 * x2 - 2 * z2, 2 * y* z + 2 * w * x, 0));
	SetRow(2, Vector4<Type>(2 * x* z + 2 * w*y, 2 * y*z - 2 * w * x, 1 - 2 * x2 - 2 * y2, 0));

}

MATRIX44_TEMPLATE
void MATRIX44_OBJECT::RotateAroundAxis(float angle, float u, float v, float w)
{
	MATRIX44_OBJECT rotationMatrix;

	Vector3<Type> position = GetTranslation();
	SetTranslation({ 0, 0, 0 });

	float L = (u*u + v * v + w * w);
	float Lsqrt = sqrt(L);
	float u2 = u * u;
	float v2 = v * v;
	float w2 = w * w;

	rotationMatrix[0] = (u2 + (v2 + w2) * cos(angle)) / L;
	rotationMatrix[1] = (u * v * (1 - cos(angle)) - w * Lsqrt * sin(angle)) / L;
	rotationMatrix[2] = (u * w * (1 - cos(angle)) + v * Lsqrt * sin(angle)) / L;
	rotationMatrix[3] = 0.0;

	rotationMatrix[4] = (u * v * (1 - cos(angle)) + w * Lsqrt * sin(angle)) / L;
	rotationMatrix[5] = (v2 + (u2 + w2) * cos(angle)) / L;
	rotationMatrix[6] = (v * w * (1 - cos(angle)) - u * Lsqrt * sin(angle)) / L;
	rotationMatrix[7] = 0.0;

	rotationMatrix[8] = (u * w * (1 - cos(angle)) - v * Lsqrt * sin(angle)) / L;
	rotationMatrix[9] = (v * w * (1 - cos(angle)) + u * Lsqrt * sin(angle)) / L;
	rotationMatrix[10] = (w2 + (u2 + v2) * cos(angle)) / L;
	rotationMatrix[11] = 0.0;

	rotationMatrix[12] = 0.0;
	rotationMatrix[13] = 0.0;
	rotationMatrix[14] = 0.0;
	rotationMatrix[15] = 1.0;

	*this *= rotationMatrix;
	SetTranslation(position);
}

MATRIX44_TEMPLATE
void MATRIX44_OBJECT::TranslationRotation(const Vector3<Type>& rotationVectorInRadians, const Position& aPosition)
{
	Vector4<Type> oldPosition = GetRow(3);
	Vector4<Type> translatedPosition{ oldPosition.myX - aPosition.myX, oldPosition.myY - aPosition.myY, oldPosition.myZ - aPosition.myZ, 1 };

	Matrix44f rotationMatrix;

	rotationMatrix *= CreateRotateAroundX(rotationVectorInRadians.myX);
	rotationMatrix *= CreateRotateAroundY(rotationVectorInRadians.myY);
	rotationMatrix *= CreateRotateAroundZ(rotationVectorInRadians.myZ);

	translatedPosition = translatedPosition * rotationMatrix;

	translatedPosition = Vector4 < Type > {aPosition.myX + translatedPosition.myX, aPosition.myY + translatedPosition.myY, aPosition.myZ + translatedPosition.myZ, 1};

	SetRow(3, translatedPosition);
}

MATRIX44_TEMPLATE
void MATRIX44_OBJECT::Rotate(const Vector3<Type>& rotationVectorInRadians)
{
	Position oldPosition = GetTranslation();
	Type oldValue = myMatrix[15];
	myMatrix[15] = 0;

	*this *= CreateRotateAroundX(rotationVectorInRadians.myX);
	*this *= CreateRotateAroundY(rotationVectorInRadians.myY);
	*this *= CreateRotateAroundZ(rotationVectorInRadians.myZ);
	SetTranslation(oldPosition);
	myMatrix[15] = oldValue;
}

MATRIX44_TEMPLATE
Vector4<Type> MATRIX44_OBJECT::GetRow(const unsigned short aRowNumber) const
{

	if (aRowNumber < 0 || aRowNumber > 3)
	{
		DL_ASSERT("Row number outside of range");
	}

	const unsigned short offset = aRowNumber * 4;

	return Vector4<Type>(
		myMatrix[0 + offset],
		myMatrix[1 + offset],
		myMatrix[2 + offset],
		myMatrix[3 + offset]);
}

MATRIX44_TEMPLATE
Vector4<Type> MATRIX44_OBJECT::GetColumn(const unsigned short aColumnNumber) const
{
	if (aColumnNumber < 0 || aColumnNumber > 3)
	{
		DL_ASSERT("Column number outside of range");
	}

	return Vector4<Type>(

		myMatrix[0 + aColumnNumber],
		myMatrix[4 + aColumnNumber],
		myMatrix[8 + aColumnNumber],
		myMatrix[12 + aColumnNumber]);
}

MATRIX44_TEMPLATE
void MATRIX44_OBJECT::SetRow(const unsigned short aRowNumber, const Vector4<Type>& aVector4)
{
	if (aRowNumber < 0 || aRowNumber > 3)
	{
		DL_ASSERT("Row number outside of range");
	}

	const unsigned short offset = aRowNumber * 4;

	myMatrix[0 + offset] = aVector4.myX;
	myMatrix[1 + offset] = aVector4.myY;
	myMatrix[2 + offset] = aVector4.myZ;
	myMatrix[3 + offset] = aVector4.myW;

}

MATRIX44_TEMPLATE
void MATRIX44_OBJECT::SetColumn(const unsigned short aColumnNumber, const Vector4<Type>& aVector4)
{
	if (aColumnNumber < 0 || aColumnNumber > 3)
	{
		DL_ASSERT("Column number outside of range");
	}

	myMatrix[0 + aColumnNumber] = aVector4.myX;
	myMatrix[4 + aColumnNumber] = aVector4.myY;
	myMatrix[8 + aColumnNumber] = aVector4.myZ;
	myMatrix[12 + aColumnNumber] = aVector4.myW;

}

MATRIX44_TEMPLATE
MATRIX44_OBJECT::Matrix44()
{
	SetRow(0, Vector4<Type>(1, 0, 0, 0));
	SetRow(1, Vector4<Type>(0, 1, 0, 0));
	SetRow(2, Vector4<Type>(0, 0, 1, 0));
	SetRow(3, Vector4<Type>(0, 0, 0, 1));
}

MATRIX44_TEMPLATE
MATRIX44_OBJECT MATRIX44_OBJECT::operator=(const MATRIX44_OBJECT& aRightValue)
{
	this->SetRow(0, aRightValue.GetRow(0));
	this->SetRow(1, aRightValue.GetRow(1));
	this->SetRow(2, aRightValue.GetRow(2));
	this->SetRow(3, aRightValue.GetRow(3));

	return *this;
}

MATRIX44_TEMPLATE
MATRIX44_OBJECT::Matrix44(const Matrix44<Type>& aMatrix)
{
	for (int i = 0; i < 16; i++)
	{
		myMatrix[i] = aMatrix.myMatrix[i];
	}
}

MATRIX44_TEMPLATE
Matrix44<Type> MATRIX44_OBJECT::Transpose(const Matrix44& aMatrixToTranspose) const
{
	Matrix44<Type> aTransposedMatrix;

	aTransposedMatrix.SetRow(0, aMatrixToTranspose.GetColumn(0));
	aTransposedMatrix.SetRow(1, aMatrixToTranspose.GetColumn(1));
	aTransposedMatrix.SetRow(2, aMatrixToTranspose.GetColumn(2));
	aTransposedMatrix.SetRow(3, aMatrixToTranspose.GetColumn(3));

	return aTransposedMatrix;
}

MATRIX44_TEMPLATE
MATRIX44_OBJECT MATRIX44_OBJECT::CreateRotateAroundX(Type aAngleInRadians)
{
	Type cosinus = cos(aAngleInRadians);
	Type sinus = sin(aAngleInRadians);

	MATRIX44_OBJECT newMatrix;

	newMatrix.SetRow(0, Vector4<Type>(1, 0, 0, 0));
	newMatrix.SetRow(1, Vector4<Type>(0, cosinus, sinus, 0));
	newMatrix.SetRow(2, Vector4<Type>(0, -sinus, cosinus, 0));
	newMatrix.SetRow(3, Vector4<Type>(0, 0, 0, 1));

	return newMatrix;
}

MATRIX44_TEMPLATE
MATRIX44_OBJECT MATRIX44_OBJECT::CreateRotateAroundY(Type aAngleInRadians)
{
	Type cosinus = cos(aAngleInRadians);
	Type sinus = sin(aAngleInRadians);

	MATRIX44_OBJECT newMatrix;

	newMatrix.SetRow(0, Vector4<Type>(cosinus, 0, -sinus, 0));
	newMatrix.SetRow(1, Vector4<Type>(0, 1, 0, 0));
	newMatrix.SetRow(2, Vector4<Type>(sinus, 0, cosinus, 0));
	newMatrix.SetRow(3, Vector4<Type>(0, 0, 0, 1));

	return newMatrix;
}

MATRIX44_TEMPLATE
MATRIX44_OBJECT MATRIX44_OBJECT::CreateRotateAroundZ(Type aAngleInRadians)
{
	Type cosinus = cos(aAngleInRadians);
	Type sinus = sin(aAngleInRadians);

	MATRIX44_OBJECT newMatrix;

	newMatrix.SetRow(0, Vector4<Type>(cosinus, sinus, 0, 0));
	newMatrix.SetRow(1, Vector4<Type>(-sinus, cosinus, 0, 0));
	newMatrix.SetRow(2, Vector4<Type>(0, 0, 1, 0));
	newMatrix.SetRow(3, Vector4<Type>(0, 0, 0, 1));

	return newMatrix;
}

MATRIX44_TEMPLATE
MATRIX44_OBJECT MATRIX44_OBJECT::CreateMatrixFromForwardVector(const Vector3<Type>& aForwardVector)
{
	Matrix44<Type> toReturn;
	Vector3<Type> forwardVector = aForwardVector;
	Vector3<Type> upVector = forwardVector * Matrix33<Type>::CreateRotateAroundX(-(PI / 2));
	Vector3<Type> rightVector = Cross(forwardVector, upVector) * -1.f;

	toReturn.SetRow(2, { forwardVector.x, forwardVector.y, forwardVector.z, 0 });
	toReturn.SetRow(1, { upVector.x, upVector.y, upVector.z, 0 });
	toReturn.SetRow(0, { rightVector.x, rightVector.y, rightVector.z, 0 });
	return toReturn;
}


MATRIX44_TEMPLATE
MATRIX44_OBJECT MATRIX44_OBJECT::CreateProjectionMatrixLH(const Type aNearZ, const Type aFarZ, const Type aAspectRatio, const Type aFovAngleRadians, const Type aScale)
{
	Matrix44 newMatrix;
	Type SinFov;
	Type CosFov;
	Type Height;
	Type Width;

	SinFov = sin(0.5f * aFovAngleRadians);
	CosFov = cos(0.5f * aFovAngleRadians);

	Width = CosFov / SinFov;
	Height = Width / aAspectRatio;

	Type scaling = aFarZ / (aFarZ - aNearZ);
	scaling *= aScale;

	newMatrix.SetRow(0, Vector4<float>(Width, 0.0f, 0.0f, 0.0f));
	newMatrix.SetRow(1, Vector4<float>(0.0f, Height, 0.0f, 0.0f));
	newMatrix.SetRow(2, Vector4<float>(0.0f, 0.0f, scaling, 1.0f));
	newMatrix.SetRow(3, Vector4<float>(0.0f, 0.0f, -scaling * aNearZ, 0.0f));

	return newMatrix;
}

MATRIX44_TEMPLATE
const MATRIX44_OBJECT MATRIX44_OBJECT::GetInverse() const
{
	Vector4<float> myTranslation = GetRow(3);
	Matrix44<float> inverseMatrix = *this;
	inverseMatrix.SetRow(3, Vector4<float>(0, 0, 0, 1));
	myTranslation *= -1.0f;
	myTranslation.myW = 1;
	inverseMatrix = Matrix44<float>::Transpose(inverseMatrix);
	myTranslation *= inverseMatrix;
	inverseMatrix.SetRow(3, myTranslation);
	return inverseMatrix;
}

MATRIX44_TEMPLATE
const MATRIX44_OBJECT MATRIX44_OBJECT::GetCorrectInverse() const
{
	Matrix44<float> inverse;

	inverse.myMatrix[0] = (myMatrix[6] * myMatrix[11] * myMatrix[13]) - (myMatrix[7] * myMatrix[10] * myMatrix[13]) + (myMatrix[7] * myMatrix[9] * myMatrix[14]) - (myMatrix[5] * myMatrix[11] * myMatrix[14]) - (myMatrix[6] * myMatrix[9] * myMatrix[15]) + (myMatrix[5] * myMatrix[10] * myMatrix[15]);
	inverse.myMatrix[1] = (myMatrix[3] * myMatrix[10] * myMatrix[13]) - (myMatrix[2] * myMatrix[11] * myMatrix[13]) - (myMatrix[3] * myMatrix[9] * myMatrix[14]) + (myMatrix[1] * myMatrix[11] * myMatrix[14]) + (myMatrix[2] * myMatrix[9] * myMatrix[15]) - (myMatrix[1] * myMatrix[10] * myMatrix[15]);
	inverse.myMatrix[2] = (myMatrix[2] * myMatrix[7] * myMatrix[13]) - (myMatrix[3] * myMatrix[6] * myMatrix[13]) + (myMatrix[3] * myMatrix[5] * myMatrix[14]) - (myMatrix[1] * myMatrix[7] * myMatrix[14]) - (myMatrix[2] * myMatrix[5] * myMatrix[15]) + (myMatrix[1] * myMatrix[6] * myMatrix[15]);
	inverse.myMatrix[3] = (myMatrix[3] * myMatrix[6] * myMatrix[9]) - (myMatrix[2] * myMatrix[7] * myMatrix[9]) - (myMatrix[3] * myMatrix[5] * myMatrix[10]) + (myMatrix[1] * myMatrix[7] * myMatrix[10]) + (myMatrix[2] * myMatrix[5] * myMatrix[11]) - (myMatrix[1] * myMatrix[6] * myMatrix[11]);
	inverse.myMatrix[4] = (myMatrix[7] * myMatrix[10] * myMatrix[12]) - (myMatrix[6] * myMatrix[11] * myMatrix[12]) - (myMatrix[7] * myMatrix[8] * myMatrix[14]) + (myMatrix[4] * myMatrix[11] * myMatrix[14]) + (myMatrix[6] * myMatrix[8] * myMatrix[15]) - (myMatrix[4] * myMatrix[10] * myMatrix[15]);
	inverse.myMatrix[5] = (myMatrix[2] * myMatrix[11] * myMatrix[12]) - (myMatrix[3] * myMatrix[10] * myMatrix[12]) + (myMatrix[3] * myMatrix[8] * myMatrix[14]) - (myMatrix[0] * myMatrix[11] * myMatrix[14]) - (myMatrix[2] * myMatrix[8] * myMatrix[15]) + (myMatrix[0] * myMatrix[10] * myMatrix[15]);
	inverse.myMatrix[6] = (myMatrix[3] * myMatrix[6] * myMatrix[12]) - (myMatrix[2] * myMatrix[7] * myMatrix[12]) - (myMatrix[3] * myMatrix[4] * myMatrix[14]) + (myMatrix[0] * myMatrix[7] * myMatrix[14]) + (myMatrix[2] * myMatrix[4] * myMatrix[15]) - (myMatrix[0] * myMatrix[6] * myMatrix[15]);
	inverse.myMatrix[7] = (myMatrix[2] * myMatrix[7] * myMatrix[8]) - (myMatrix[3] * myMatrix[6] * myMatrix[8]) + (myMatrix[3] * myMatrix[4] * myMatrix[10]) - (myMatrix[0] * myMatrix[7] * myMatrix[10]) - (myMatrix[2] * myMatrix[4] * myMatrix[11]) + (myMatrix[0] * myMatrix[6] * myMatrix[11]);
	inverse.myMatrix[8] = (myMatrix[5] * myMatrix[11] * myMatrix[12]) - (myMatrix[7] * myMatrix[9] * myMatrix[12]) + (myMatrix[7] * myMatrix[8] * myMatrix[13]) - (myMatrix[4] * myMatrix[11] * myMatrix[13]) - (myMatrix[5] * myMatrix[8] * myMatrix[15]) + (myMatrix[4] * myMatrix[9] * myMatrix[15]);
	inverse.myMatrix[9] = (myMatrix[3] * myMatrix[9] * myMatrix[12]) - (myMatrix[1] * myMatrix[11] * myMatrix[12]) - (myMatrix[3] * myMatrix[8] * myMatrix[13]) + (myMatrix[0] * myMatrix[11] * myMatrix[13]) + (myMatrix[1] * myMatrix[8] * myMatrix[15]) - (myMatrix[0] * myMatrix[9] * myMatrix[15]);
	inverse.myMatrix[10] = (myMatrix[1] * myMatrix[7] * myMatrix[12]) - (myMatrix[3] * myMatrix[5] * myMatrix[12]) + (myMatrix[3] * myMatrix[4] * myMatrix[13]) - (myMatrix[0] * myMatrix[7] * myMatrix[13]) - (myMatrix[1] * myMatrix[4] * myMatrix[15]) + (myMatrix[0] * myMatrix[5] * myMatrix[15]);
	inverse.myMatrix[11] = (myMatrix[3] * myMatrix[5] * myMatrix[8]) - (myMatrix[1] * myMatrix[7] * myMatrix[8]) - (myMatrix[3] * myMatrix[4] * myMatrix[9]) + (myMatrix[0] * myMatrix[7] * myMatrix[9]) + (myMatrix[1] * myMatrix[4] * myMatrix[11]) - (myMatrix[0] * myMatrix[5] * myMatrix[11]);
	inverse.myMatrix[12] = (myMatrix[6] * myMatrix[9] * myMatrix[12]) - (myMatrix[5] * myMatrix[10] * myMatrix[12]) - (myMatrix[6] * myMatrix[8] * myMatrix[13]) + (myMatrix[4] * myMatrix[10] * myMatrix[13]) + (myMatrix[5] * myMatrix[8] * myMatrix[14]) - (myMatrix[4] * myMatrix[9] * myMatrix[14]);
	inverse.myMatrix[13] = (myMatrix[1] * myMatrix[10] * myMatrix[12]) - (myMatrix[2] * myMatrix[9] * myMatrix[12]) + (myMatrix[2] * myMatrix[8] * myMatrix[13]) - (myMatrix[0] * myMatrix[10] * myMatrix[13]) - (myMatrix[1] * myMatrix[8] * myMatrix[14]) + (myMatrix[0] * myMatrix[9] * myMatrix[14]);
	inverse.myMatrix[14] = (myMatrix[2] * myMatrix[5] * myMatrix[12]) - (myMatrix[1] * myMatrix[6] * myMatrix[12]) - (myMatrix[2] * myMatrix[4] * myMatrix[13]) + (myMatrix[0] * myMatrix[6] * myMatrix[13]) + (myMatrix[1] * myMatrix[4] * myMatrix[14]) - (myMatrix[0] * myMatrix[5] * myMatrix[14]);
	inverse.myMatrix[15] = (myMatrix[1] * myMatrix[6] * myMatrix[8]) - (myMatrix[2] * myMatrix[5] * myMatrix[8]) + (myMatrix[2] * myMatrix[4] * myMatrix[9]) - (myMatrix[0] * myMatrix[6] * myMatrix[9]) - (myMatrix[1] * myMatrix[4] * myMatrix[10]) + (myMatrix[0] * myMatrix[5] * myMatrix[10]);

	float determinant = CalculateDeterminant();
	if (determinant == 0)
		return *this;

	determinant = 1.0f / determinant;
	inverse.myMatrix[0] *= determinant;
	inverse.myMatrix[1] *= determinant;
	inverse.myMatrix[2] *= determinant;
	inverse.myMatrix[3] *= determinant;

	inverse.myMatrix[4] *= determinant;
	inverse.myMatrix[5] *= determinant;
	inverse.myMatrix[6] *= determinant;
	inverse.myMatrix[7] *= determinant;

	inverse.myMatrix[8] *= determinant;
	inverse.myMatrix[9] *= determinant;
	inverse.myMatrix[10] *= determinant;
	inverse.myMatrix[11] *= determinant;

	inverse.myMatrix[12] *= determinant;
	inverse.myMatrix[13] *= determinant;
	inverse.myMatrix[14] *= determinant;
	inverse.myMatrix[15] *= determinant;

	return inverse;
}

MATRIX44_TEMPLATE
const float MATRIX44_OBJECT::CalculateDeterminant() const
{
	return (
		myMatrix[3] * myMatrix[6] * myMatrix[9] * myMatrix[12] - myMatrix[2] * myMatrix[7] * myMatrix[9] * myMatrix[12] -
		myMatrix[3] * myMatrix[5] * myMatrix[10] * myMatrix[12] + myMatrix[1] * myMatrix[7] * myMatrix[10] * myMatrix[12] +
		myMatrix[2] * myMatrix[5] * myMatrix[11] * myMatrix[12] - myMatrix[1] * myMatrix[6] * myMatrix[11] * myMatrix[12] -
		myMatrix[3] * myMatrix[6] * myMatrix[8] * myMatrix[13] + myMatrix[2] * myMatrix[7] * myMatrix[8] * myMatrix[13] +
		myMatrix[3] * myMatrix[4] * myMatrix[10] * myMatrix[13] - myMatrix[0] * myMatrix[7] * myMatrix[10] * myMatrix[13] -
		myMatrix[2] * myMatrix[4] * myMatrix[11] * myMatrix[13] + myMatrix[0] * myMatrix[6] * myMatrix[11] * myMatrix[13] +
		myMatrix[3] * myMatrix[5] * myMatrix[8] * myMatrix[14] - myMatrix[1] * myMatrix[7] * myMatrix[8] * myMatrix[14] -
		myMatrix[3] * myMatrix[4] * myMatrix[9] * myMatrix[14] + myMatrix[0] * myMatrix[7] * myMatrix[9] * myMatrix[14] +
		myMatrix[1] * myMatrix[4] * myMatrix[11] * myMatrix[14] - myMatrix[0] * myMatrix[5] * myMatrix[11] * myMatrix[14] -
		myMatrix[2] * myMatrix[5] * myMatrix[8] * myMatrix[15] + myMatrix[1] * myMatrix[6] * myMatrix[8] * myMatrix[15] +
		myMatrix[2] * myMatrix[4] * myMatrix[9] * myMatrix[15] - myMatrix[0] * myMatrix[6] * myMatrix[9] * myMatrix[15] -
		myMatrix[1] * myMatrix[4] * myMatrix[10] * myMatrix[15] + myMatrix[0] * myMatrix[5] * myMatrix[10] * myMatrix[15]);
}

MATRIX44_TEMPLATE
const Vector3<Type> MATRIX44_OBJECT::GetTranslation() const
{
	Vector4<float> translation = GetRow(3);
	return Vector3<Type>(translation.myX, translation.myY, translation.myZ);
}

MATRIX44_TEMPLATE
void MATRIX44_OBJECT::SetTranslation(const Vector3<Type>& aTranslation)
{

	SetRow(3, Vector4<float>(aTranslation.myX, aTranslation.myY, aTranslation.myZ, myMatrix[15]));
}

MATRIX44_TEMPLATE
void MATRIX44_OBJECT::Move(const Vector3<Type>& aRelativePosition)
{
	Vector3<float> newPosition = aRelativePosition + GetTranslation();

	SetTranslation(newPosition);
}

MATRIX44_TEMPLATE
void MATRIX44_OBJECT::Scale(const Vector3<Type>& aScaleVector)
{
	Matrix44<Type> scaleMatrix;

	Vector3<Type> oldPosition = GetTranslation();
	SetTranslation({ 0, 0, 0 });
	scaleMatrix[0] = aScaleVector.myX;
	scaleMatrix[5] = aScaleVector.myY;
	scaleMatrix[10] = aScaleVector.myZ;
	*this = scaleMatrix * *this;
	SetTranslation(oldPosition);
}


MATRIX44_TEMPLATE
float& MATRIX44_OBJECT::operator[](const unsigned short anIndex)
{
	return myMatrix[anIndex];
}

MATRIX44_TEMPLATE
const float& MATRIX44_OBJECT::operator[](const unsigned short anIndex) const
{
	return myMatrix[anIndex];
}

MATRIX44_TEMPLATE
const Vector3<Type> MATRIX44_OBJECT::GetForwardVector() const
{
	Vector4<Type> row = GetRow(2);
	Vector3<Type> forwardVector(row.myX, row.myY, row.myZ);
	return forwardVector;
}

MATRIX44_TEMPLATE
void MATRIX44_OBJECT::LookAt(const Vector3<Type>& aDirection, const Vector3<Type>& anUpVector /*= Vector3<Type>(0, 1, 0)*/)
{
	Vector3<Type> zAxis = GetNormalized(aDirection);

	Vector3<Type> xAxis = Cross(anUpVector, zAxis);

	if (Length(xAxis) < 0.01f)
	{
		xAxis = Cross(Vector3<float>(0.0f, 0.0f, 1.0f), zAxis);
	}

	Normalize(xAxis);

	Vector3<Type> yAxis = GetNormalized(Cross(zAxis, xAxis));

	zAxis *= Type(-1);
	// Row Major
	myMatrix[0] = xAxis.myX;
	myMatrix[1] = xAxis.myY;
	myMatrix[2] = xAxis.myZ;

	myMatrix[4] = yAxis.myX;
	myMatrix[5] = yAxis.myY;
	myMatrix[6] = yAxis.myZ;

	myMatrix[8] = zAxis.myX;
	myMatrix[9] = zAxis.myY;
	myMatrix[10] = zAxis.myZ;
}
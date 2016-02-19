#pragma once

#include "Vector.h"
#include <assert.h>

#define PLANE_TEMPLATE template <typename Type>
#define PLANE_OBJECT Plane<Type>

PLANE_TEMPLATE
class Plane
{
public:
	Plane();
	~Plane();
	Plane(Vector3<Type> aFirstPoint, Vector3<Type> aSecondPoint, Vector3<Type> aThirdPoint);
	Plane(Vector3<Type> aPoint, Vector3<Type> aNormal);
	void InitWith3Points(Vector3<Type> aFirstPoint, Vector3<Type> aSecondPoint, Vector3<Type> aThirdPoint);
	void InitWithPointAndNormal(Vector3<Type> aPoint, Vector3<Type> aNormal);
	bool Inside(const Vector3<Type>& aPosition) const;
	bool Inside(const Vector3<Type>& aPosition, const float aRadius)const;

	bool operator==(const Plane<Type>& aPlane);
	bool operator!=(const Plane<Type>& aPlane);

	const Vector3<Type>& GetNormal() const;
	const Vector3<Type>& GetPoint() const;


private:
	Vector3<Type> myPoint;
	Vector3<Type> myNormal;
	bool myIsInitialized;
};

PLANE_TEMPLATE
bool PLANE_OBJECT::Inside(const Vector3<Type>& aPosition, const float aRadius) const
{
	Position nearestPositionToPlane = aPosition + (aRadius * myNormal * -1.f);

	return Inside(nearestPositionToPlane);
}

PLANE_TEMPLATE
const Vector3<Type>& PLANE_OBJECT::GetNormal() const
{
	return myNormal;
}

PLANE_TEMPLATE
const Vector3<Type>& PLANE_OBJECT::GetPoint() const
{
	return myPoint;
}


PLANE_TEMPLATE
bool PLANE_OBJECT::operator==(const Plane<Type>& aPlane)
{
	if (myPoint == aPlane.myPoint && myNormal == aPlane.myNormal)
	{
		return true;
	}
	return false;
}

PLANE_TEMPLATE
bool PLANE_OBJECT::operator!=(const Plane<Type>& aPlane)
{
	if (*this == aPlane)
	{
		return false;
	}
	return true;
}

PLANE_TEMPLATE
PLANE_OBJECT::Plane()
{

}

PLANE_TEMPLATE
PLANE_OBJECT::~Plane()
{

}

PLANE_TEMPLATE
PLANE_OBJECT::Plane(Vector3<Type> aFirstPoint, Vector3<Type> aSecondPoint,
Vector3<Type> aThirdPoint)
{
	InitWith3Points(aFirstPoint, aSecondPoint, aThirdPoint);
}

PLANE_TEMPLATE
PLANE_OBJECT::Plane(Vector3<Type> aPoint, Vector3<Type> aNormal)
{
	InitWithPointAndNormal(aPoint, aNormal);
}

PLANE_TEMPLATE
void PLANE_OBJECT::InitWith3Points(Vector3<Type> aFirstPoint,
Vector3<Type> aSecondPoint, Vector3<Type> aThirdPoint)
{
	myPoint = aFirstPoint;
	Vector3<Type> edge3 = aSecondPoint - aFirstPoint;
	Vector3<Type> edge1 = aThirdPoint - aSecondPoint;

	Vector3<Type> edgeCrossProduct = Cross(edge3, edge1);
	myNormal = GetNormalized(edgeCrossProduct);
	myNormal *= -1.f;
	myIsInitialized = true;
}

PLANE_TEMPLATE
void PLANE_OBJECT::InitWithPointAndNormal(Vector3<Type> aPoint, Vector3<Type> aNormal)
{
	myPoint = aPoint;
	myNormal = aNormal;
	myIsInitialized = true;
}

PLANE_TEMPLATE bool PLANE_OBJECT::Inside(const Vector3<Type>& aPosition) const
{
	assert(myIsInitialized == true && "Plane not initialized");
	if (Dot(GetNormalized(aPosition - myPoint), myNormal) < 0)
	{
		return true;
	}
	return false;
}
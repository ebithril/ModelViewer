#pragma once
#include "Vector.h"
#include <assert.h>

#define LINE_TEMPLATE 	template <typename Type>
#define LINE_OBJECT Line<Type>

LINE_TEMPLATE
class Line
{
public:
	Line();
	~Line();

	Line(Vector2<Type> aFirstPoint, Vector2<Type> aSecondPoint);
	void InitWith2Points(Vector2<Type> afirstPoint, Vector2<Type> aSecondPoint);
	void InitWithPointAndDirection(Vector2<Type> aPoint, Vector2<Type> aDirection);
	bool Inside(const Vector2<Type>& aPosition) const ;
	bool operator==(const Line<Type>& aLine);
	bool operator!=(const Line<Type>& aLine);

	const Vector2<Type>& GetMyPoint() const; 
	const Vector2<Type>& GetMySecondPoint() const;
	const Vector2<Type>& GetMyNormal() const;

private:

	Vector2<Type> myPoint;
	Vector2<Type> mySecondPoint;
	Vector2<Type> myNormal;
	bool myIsInitialized;

};

LINE_TEMPLATE
const Vector2<Type>& LINE_OBJECT::GetMySecondPoint() const
{
	return mySecondPoint;
}


LINE_TEMPLATE
const Vector2<Type>& LINE_OBJECT::GetMyPoint() const
{
	return myPoint;
}

LINE_TEMPLATE
const Vector2<Type>& LINE_OBJECT::GetMyNormal() const
{
	return myNormal;
}

LINE_TEMPLATE
bool LINE_OBJECT::operator==(const Line<Type>& aLine)
{
	if (myPoint == aLine.myPoint )
		
	{
		return true;
	}
	return false;
}

LINE_TEMPLATE
bool LINE_OBJECT::operator!=(const Line<Type>& aLine)
{
	if (*this == aLine)
	{
		return false;
	}
	return true;
}

LINE_TEMPLATE
LINE_OBJECT::Line()
{

}

LINE_TEMPLATE
LINE_OBJECT::~Line()
{

}	

LINE_TEMPLATE
LINE_OBJECT::Line(Vector2<Type> aFirstPoint, Vector2<Type> aSecondPoint)
{
	InitWith2Points(aFirstPoint, aSecondPoint);
}

LINE_TEMPLATE
void LINE_OBJECT::InitWith2Points(Vector2<Type> aFirstPoint, Vector2<Type> aSecondPoint)
{
	myPoint = aFirstPoint;
	mySecondPoint = aSecondPoint;
	
	Vector2<Type> myNormalizedVector = GetNormalized(aSecondPoint - aFirstPoint);

	myNormal.myX = -myNormalizedVector.myY;
	myNormal.myY = myNormalizedVector.myX;
	myIsInitialized = true;
}

LINE_TEMPLATE
void LINE_OBJECT::InitWithPointAndDirection(Vector2<Type> aPoint, Vector2<Type> aDirection)
{
	myPoint = aPoint;
	mySecondPoint = 2.f * aPoint;
	/*myNormal.myX = -aDirection.myY;
	myNormal.myY = aDirection.myX;*/
	myNormal = aDirection;

	Normalize(myNormal);
	myIsInitialized = true;
}

LINE_TEMPLATE
bool LINE_OBJECT::Inside(const Vector2<Type>& aPosition) const
{
	assert(myIsInitialized == true && "Line not initialized");
	if (Dot(aPosition - myPoint, myNormal) < 0)
	{
		return true;
	}
	return false;
}


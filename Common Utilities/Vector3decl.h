#pragma once

template<typename Type>
struct Vector3
{
public:
	Vector3();
	Vector3(Type aX, Type aY, Type aZ);

	union
	{
		Type myX;
		Type x;
		Type r;
		Type myR;
	};

	union
	{
		Type myY;
		Type y;
		Type g;
		Type myG;
	};

	union
	{
		Type myZ;
		Type z;
		Type b;
		Type myB;
	};
};
template <typename Type>
Vector3<Type>::Vector3()
	: myX(0)
	, myY(0)
	, myZ(0)
{
	x = 0;
	y = 0;
	z = 0;
}

template <typename Type>
Vector3<Type>::Vector3(Type aX, Type aY, Type aZ)
{
	myX = aX;
	myY = aY;
	myZ = aZ;
}
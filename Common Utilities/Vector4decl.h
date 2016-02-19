#pragma once

template<typename Type>
struct Vector4
{
public:

	Vector4();
	Vector4(Type aX, Type aY, Type aZ, Type aW);

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

	union
	{
		Type myW;
		Type w;
		Type a;
		Type myA;
	};
};

template <typename Type>
Vector4<Type>::Vector4()
{

}

template <typename Type>
Vector4<Type>::Vector4(Type aX, Type aY, Type aZ, Type aW)
{
	myX = aX;
	myY = aY;
	myZ = aZ;
	myW = aW;
}
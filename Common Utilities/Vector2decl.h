#pragma once

template<typename Type>
struct Vector2
{
public:
	Vector2();
	Vector2(Type aX, Type aY);
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
};

template<typename Type>
Vector2<Type>::Vector2(Type aX, Type aY)
{
	myX = aX;
	myY = aY;
}

template<typename Type>
Vector2<Type>::Vector2()
{

}

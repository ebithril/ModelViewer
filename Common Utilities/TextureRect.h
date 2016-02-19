#pragma once 
#include "Vector.h"
#include <new>

struct TextureRect
{
public:
	inline TextureRect();
	inline TextureRect(const Vector2<int>& aPosition, const Vector2<int>& aWidthAndHeigth, const Vector2<int>& aHotspot);
	inline ~TextureRect();

	inline TextureRect& operator=(const TextureRect& aTextureRect);

	const Vector2<int> myPosition;
	const Vector2<int> myWidthAndHeigth;
	const Vector2<int> myHotspot;
	const bool myIsUsed;

};

TextureRect::TextureRect() :
myPosition(Vector2<int>(0, 0)),
myWidthAndHeigth(Vector2<int>(0, 0)),
myHotspot(Vector2<int>(0, 0)),
myIsUsed(false)
{

}



TextureRect::TextureRect(const Vector2<int>& aPosition, const Vector2<int>& aWidthAndHeigth, const Vector2<int>& aHotspot) :
myPosition(aPosition), myWidthAndHeigth(aWidthAndHeigth), myHotspot(aHotspot), myIsUsed(true)
{

}

TextureRect::~TextureRect()
{

}

TextureRect& TextureRect::operator=(const TextureRect& aTextureRect)
{
	this->~TextureRect();
	*new(this) TextureRect(aTextureRect);

	return *this;
}
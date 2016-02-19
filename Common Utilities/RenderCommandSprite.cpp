#include "Common Utilities_Precompiled.h"
#include "RenderCommandSprite.h"

namespace CommonUtilities
{
	RenderCommandSprite::RenderCommandSprite(const Position& aPosition, const Vector2<int>& aTilePosition, GE::Sprite* aSprite, const eLayers aLayer,
		const Vector2<float>& aScale, const float aRotation, const Vector4<float>& aColor , const int aBlendMode)
	{
		myPosition = aPosition;
		myTilePosition = aTilePosition;
		mySprite = aSprite;
		myLayer = aLayer;
		myScale = aScale;
		myRotation = aRotation;
		myBlendMode = aBlendMode;
		myColor = aColor;
		
	}

	RenderCommandSprite::~RenderCommandSprite()
	{

	}

	RenderCommandSprite::RenderCommandSprite()
	{
		mySprite = nullptr;
	}

}
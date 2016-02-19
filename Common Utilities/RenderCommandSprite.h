#pragma once
#include "Vector.h"
#include "TextureRect.h"
#include "GraphicsEnums.h"
#include "Macros.h"

namespace GraphicsEngine
{
	class Sprite;
}

namespace GE = GraphicsEngine;


namespace CommonUtilities
{
	
	struct RenderCommandSprite
	{
	public:
		RenderCommandSprite(const Position& aPosition, const Vector2<int>& aTilePosition, GE::Sprite* aSprite, eLayers aLayer = eLayers::DEFAULT,
			const Vector2<float>& aScale = Vector2<float>(1,1), const float aRotation = 0.0f, 
			const Vector4<float>& aColor = Vector4<float>(1.f,1.f,1.f,1.f), const int aBlendMode = 2);
		~RenderCommandSprite();
		RenderCommandSprite();
		Position myPosition;
		Vector2<int> myTilePosition;
		GE::Sprite* mySprite;
		Vector2<float> myScale;
		float myRotation;
		Vector4<float> myColor;
		TextureRect myTextureRect;
		int myBlendMode; 
		eLayers myLayer;
	};
};

namespace CU = CommonUtilities;
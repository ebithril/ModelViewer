#pragma once
#include "Sprite.h"
#include "../Common Utilities/ObjectPool.h"
#include "../Common Utilities/Vector.h"
#define NUMBER_OF_SPRITES 256

namespace GraphicsEngine
{
	class SpriteFactory
	{
	public:
		SpriteFactory();
		~SpriteFactory();

		void Init();
		CU::PoolPointer<Sprite> CreateEmptySprite();

	private:
		CU::ObjectPool<NUMBER_OF_SPRITES * sizeof(Sprite)> mySprites;
	};
}

namespace GE = GraphicsEngine;


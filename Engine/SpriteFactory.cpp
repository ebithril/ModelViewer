#include "Engine_Precompiled.h"
#include "SpriteFactory.h"

namespace GraphicsEngine
{
	SpriteFactory::SpriteFactory()
	{
	}


	SpriteFactory::~SpriteFactory()
	{
	}

	void SpriteFactory::Init()
	{
		mySprites.Init();
	}

	CU::PoolPointer<Sprite> SpriteFactory::CreateEmptySprite()
	{
		return mySprites.Allocate<Sprite, Sprite>();
	}
}

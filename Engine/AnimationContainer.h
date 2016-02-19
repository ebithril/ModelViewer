#pragma once

#include <unordered_map>

namespace GraphicsEngine
{
	class Animation;

	class AnimationContainer
	{
	public:
		AnimationContainer();
		~AnimationContainer();
	
		Animation* GetAnimation(const std::string& aAnimationName);
		void AddAnimation(const std::string& aAnimationName, Animation* aAnimation);
		const bool CheckIfExist(const std::string& aAnimationName) const;
	private:

		std::unordered_map<std::string, Animation*> myAnimations;
	};
}

namespace GE = GraphicsEngine;
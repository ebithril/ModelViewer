#pragma once
#include "../Common Utilities/Vector.h"
#include <string>

namespace GraphicsEngine
{
	struct SetupInfo
	{
	public:
		int myColorDepth;
		Vector2<int> myWindowLocation;
		Vector2<int> myResolution;
		std::string mySessionName;
		int myStencilDepth;
		bool myWindowedMode;
		int myZBufferDepth;

		inline const float GetAspectRatio() const;
	};

	const float SetupInfo::GetAspectRatio() const
	{
		return static_cast<float>(myResolution.myX) / static_cast<float>(myResolution.myY);
	}

}

namespace GE = GraphicsEngine;

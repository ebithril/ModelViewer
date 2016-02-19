#pragma once
#include "Texture.h"

namespace GraphicsEngine
{
	struct SceneRenderData;

	class CubeMapGenerator
	{
	public:
		CubeMapGenerator();
		~CubeMapGenerator();
		static Texture GenerateCubeMap(const Vector3<float>& aPosition);
	};

}

namespace GE = GraphicsEngine;

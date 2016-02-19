#pragma once


namespace CommonUtilities
{
	struct SceneRenderCommands;
}

namespace GraphicsEngine
{
	class Texture;
	class Effect;
	class ShadowManager
	{
	public:

		ShadowManager();
		~ShadowManager();

		void Init();
		void Destroy();

		void DrawShadowMaps(CU::SceneRenderCommands& someData);

		Texture& GetDirectionalShadowMap();

	private:

		Effect* myShadowMapEffect;
		Effect* myAlphaShadowMapEffect;
		Effect* myTerrainShadowMapEffect;
		void DrawDirectionalShadowMap(CU::SceneRenderCommands& someData);
		Texture* myDirectionalShadowMap;
	};
}
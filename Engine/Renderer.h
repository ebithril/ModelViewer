#pragma once
#include "FullScreenHelper.h"
#include "BloomManager.h"
#include "DispatchHelper.h"
#include "HDRManager.h"
#include "MotionBlurManager.h"
#include "RenderTargetData.h"
#include "../Common Utilities/Observer.h"
#include "FaderHelper.h"
#include "ShadowManager.h"

namespace CommonUtilities
{
	struct SceneRenderCommands;
}

namespace CU = CommonUtilities;

namespace GraphicsEngine
{
	class DirectXEngine;
	class DirectX2D;
	class Texture;
	class LineRenderer;

	class Renderer : public CU::Observer
	{
	public:

		void RenderScene(RenderTargetData& someRenderData, CU::SceneRenderCommands& someSceneData);

		Renderer();
		~Renderer();

		void OnResize(Vector2<int> aResolution);
		void Init(DirectXEngine* anEngine);

		ShadowManager& GetShadowManager();

		//TEST
		DirectXEngine* GetEngine();
	private:

		void PreSceneSetup(RenderTargetData& someData);
		void DrawShadowMaps(CU::SceneRenderCommands& someData);
		void DrawInstances(CU::SceneRenderCommands& someSceneData);
		void DrawAlphaInstances(CU::SceneRenderCommands& someSceneData);
		void DrawEmitters(CU::SceneRenderCommands& someSceneData);
		void DrawStreaks(CU::SceneRenderCommands& someSceneData);
		
		bool HandleToggleShadowEvent(const CU::PoolPointer<CU::Event>& anEvent);
		
		LineRenderer* myLineRenderer;

		DirectXEngine* myEngine;

		Texture* myPostProcessScreenTarget;
		Texture* mySecondPostProcessScreenTarget;
		Texture* myHDRScreenTarget;
		Texture* myObjectVelocityData;

		ShadowManager myShadowManager;
		FullScreenHelper myFullScreenHelper;
		DispatchHelper myDispatcher;
		BloomManager myBloomManager;
		HDRManager myHDRManager;
		MotionBlurManager myMotionBlurManager;
		FaderHelper myFaderHelper;

		bool myShadowsIsEnabled;

		CU::GrowingArray<unsigned short> myAlphaIndexes;


	};
}

namespace GE = GraphicsEngine;
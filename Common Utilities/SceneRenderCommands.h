#pragma once
#include "RenderCommandFont.h"
#include "RenderCommandInstance.h"
#include "RenderCommandLine.h"
#include "SharedPointer.h"


namespace GraphicsEngine
{
	struct SceneRenderData;
}

namespace GE = GraphicsEngine;

namespace CommonUtilities
{
	struct ScenePrioID
	{
		CU::ID myID;
		unsigned int myPriority; //Higher renders first
	};

	struct SceneRenderCommands
	{
	public:
		SceneRenderCommands();
		~SceneRenderCommands();

		void AddSceneRenderCommands(const SceneRenderCommands& someCommands);

		ScenePrioID myPrioID;
		CU::GrowingArray<RenderCommandInstance> myRenderCommandInstances;
		CU::GrowingArray<RenderCommandLine, unsigned int> myLineRenderCommands;
		CU::GrowingArray<RenderCommandFont> myFontRenderCommands;
		GE::SceneRenderData* mySceneData;

	};
}

namespace CU = CommonUtilities;
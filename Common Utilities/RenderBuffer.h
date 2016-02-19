#pragma once

#include "StaticArray.h"
#include "DebugLines.h"
#include "GrowingArray.h"
#include "SceneRenderCommands.h"

namespace CommonUtilities
{
	class RenderBuffer
	{
	public:
		RenderBuffer();
		~RenderBuffer();

		void AddRenderCommands(const SceneRenderCommands& someRenderCommands);
		
		void ClearBuffer();

		SceneRenderCommands& GetRenderCommands(const CU::ID& aSceneID);

		CU::GrowingArray<ScenePrioID> GetSceneIDInRenderOrder();

	private:

		CU::GrowingArray<SceneRenderCommands> myCommands;

	};

}
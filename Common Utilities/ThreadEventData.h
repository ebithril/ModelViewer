#pragma once
#include <thread>
#include "GrowingArray.h"
#include "DetachObserverData.h"
#include "AttachObserverData.h"
#include "EventBuffer.h"
#include "RenderBuffer.h"
#include "SceneRenderCommands.h"

namespace GraphicsEngine
{
	struct SceneRenderData;
}

namespace GE = GraphicsEngine;

namespace CommonUtilities
{
	class ThreadEventData
	{
	public:
		ThreadEventData();
		~ThreadEventData();

		void Init();

		void Clear();

		const bool operator==(const std::thread::id& anID);

		void CheckIfObserversExist(ThreadEventData& someData);
		void ExecuteObserverAttachFunctions();
		void ExecuteObserverDetachFunctions();

		EventBuffer& GetEventBuffer();
		CU::GrowingArray<SceneRenderCommands>& GetSceneRenderBuffer();

		void SetCurrentScene(const ScenePrioID& aPrioID);
		const ScenePrioID& GetCurrentScene();

		void AddDetachObserver(const DetachObserverData& someData);
		void AddAttachObserver(const AttachObserverData& someData);

		void AddSceneData(const GE::SceneRenderData& someSceneData);

		void AddRenderCommand(const RenderCommandInstance& aRenderCommandInstance);
		void AddRenderCommand(const CU::GrowingArray<RenderCommandInstance>& aRenderCommandInstance);

		void AddRenderCommand(const RenderCommandFont& aCommandFont);
		void AddRenderCommand(const GrowingArray<RenderCommandFont>& someCommandFonts);

		void AddRenderCommand(const RenderCommandLine& aLineCommand);
		void AddRenderCommand(const GrowingArray<RenderCommandLine>& someLineCommands);

	private:
		
		std::thread::id myThreadID;
		ScenePrioID myCurrentPrioID;
		EventBuffer myEventBuffer;
		CU::GrowingArray<SceneRenderCommands> mySceneRenderCommands;
		CU::GrowingArray<DetachObserverData> myDetachObserverData;
		CU::GrowingArray<AttachObserverData> myAttachObserverData;

	};
}

namespace CU = CommonUtilities;
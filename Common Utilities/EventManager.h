#pragma once

#include "Event.h"
#include "GrowingArray.h"
#include "EventBuffer.h"
#include "Flag.h"
#include "ObserverEvent.h"
#include "RenderBuffer.h"
#include "../Engine/StreakEmitter.h"
#include <string>
#include "Events.h"
#include "ThreadEventData.h"

class Observer;

namespace GraphicsEngine
{
	struct SceneRenderData;
}

namespace CommonUtilities
{
	struct ScenePrioID;
	class ThreadEventData;
	struct DetachObserverData;
	struct AttachObserverData;

	class EventManager
	{
		friend ThreadEventData;

	public:

		static CU::EventManager* GetInstance();
		static void Create();
		static void Create(CU::EventManager* someEventManager);
		static void Destroy();
		
		void Update(const float aDeltaTime);

		const RenderBuffer& GetRenderBuffer()const;
		const GrowingArray<CU::PoolPointer<GE::StreakEmitter>>& GetStreaksToRender();

		template <typename Type>
		CU::PoolPointer<Event> CreateInternalEvent(const Type&);

		
		void AddPoolPointerEvent(const PoolPointer<Event>& anEvent);
		void AddPoolPointerEvent(const GrowingArray<PoolPointer<Event>>& someEvents);

		template <typename Type>
		void AddEvent(const Type& anEvent);


		void AttachObserver(const AttachObserverData& someData);
		void DetachObserver(Observer& anObserver);

		float GetDeltaTime();
		const double& GetTimeSinceSceneStart();
		void ResetTimeSinceSceneStart();

		void RegisterThread();

		void AddRenderCommand(const RenderCommandInstance& aRenderCommandInstance);
		void AddRenderCommand(const CU::GrowingArray<RenderCommandInstance>& aRenderCommandInstance);

		void AddRenderCommand(const RenderCommandFont& aCommandFont);
		void AddRenderCommand(const GrowingArray<RenderCommandFont>& someCommandFonts);

		void AddRenderCommand(const RenderCommandLine& aLineCommand);
		void AddRenderCommand(const GrowingArray<RenderCommandLine>& someLineCommands);

		void AddSceneData(const GE::SceneRenderData& someSceneData);

		void SetCurrentScene(const ScenePrioID& aPrioID);

		bool GetFlag(const std::string& aFlagName);
		void SetFlag(const Flag& aFlag);
		void ClearFlags();

		void ClearTimedEvents();

		RenderBuffer& GetRenderBuffer();

		void ClearRenderData();

	private:

		template <typename Type>
		CU::PoolPointer<Event> CreatePoolEvent(const Type&);

		void ClearThreadData();
		void UpdateRenderBuffer();

		void NotifyObservers();

		void UpdateTimedEvents(float aDeltaTime);
		void CreateObserverEvents();

		void ExecuteObserverFunctions();
		void ExecuteEventFunctions();
		void ExecuteObserverDetach(const DetachObserverData& someData);
		void ExecuteObserverAttach(const AttachObserverData& someData);

		unsigned short GetEnumNumber(const eEvent& anEnumNumber);

		EventManager();
		~EventManager();

		static EventManager* ourEventManager;

		RenderBuffer myRenderBuffer;

		float ourDeltaTime;
		double ourTimeSinceSceneStart;
		volatile bool myIsNotifing;

		GrowingArray<ThreadEventData*> myThreadsEventData;
		GrowingArray<PoolPointer<Event>> myTimedEvents;
		GrowingArray < PoolPointer<Event>> myCurrentNotifyEvents;
		GrowingArray<PoolPointer<Event>> myNotifyingEvents;
		GrowingArray<AttachObserverData> myNotifyingAttachData;
		GrowingArray<DetachObserverData> myNotifyingDetachData;
		GrowingArray<ObserverEvent> myObserverEvents;
		GrowingArray<unsigned short> myEventWithObserverNullPointers;
		ThreadEventData* GetThreadEventData();
		Map<std::string, Flag,1024> myFlags;
	};

	template <typename Type>
	void EventManager::AddEvent(const Type& anEvent)
	{
		AddPoolPointerEvent(CreatePoolEvent(anEvent));
	}

	template <typename Type>
	CU::PoolPointer<Event> EventManager::CreatePoolEvent(const Type& anEvent)
	{
		return GetThreadEventData()->GetEventBuffer().CreatePoolEvent(anEvent);
	}
	
	template <typename Type>
	CU::PoolPointer<Event> EventManager::CreateInternalEvent(const Type& anEvent)
	{
		return GetThreadEventData()->GetEventBuffer().CreateInternalEvent(anEvent);
	}

}

namespace CU = CommonUtilities;
#include "Common Utilities_Precompiled.h"
#include "EventManager.h"
#include "Observer.h"
#include "EventEnums.h"

#include "Macros.h"
#include "DL_Assert.h"
#include "ThreadEventData.h"
#include "../Engine/SceneRenderData.h"
#include "Events.h"


namespace CommonUtilities
{
	EventManager* EventManager::ourEventManager = nullptr;

	void EventManager::AddPoolPointerEvent(const PoolPointer<Event>& anEvent)
	{
		if (anEvent->GetMyType() == eEvent::MULTI_EVENT)
		{
			GET_EVENT_DATA(anEvent, MultiEvent, multiEvent);
			AddEvent(multiEvent->myPoolpointers);
		}
		else
		{
			if (myIsNotifing == false)
			{
				GetThreadEventData()->GetEventBuffer().AddEvent(anEvent);
			}
			else
			{
				myNotifyingEvents.Add(anEvent);
			}
		}
	}

	void EventManager::AddPoolPointerEvent(const GrowingArray<PoolPointer<Event>>& someEvents)
	{
		for (unsigned short i = 0; i < someEvents.Size(); i++)
		{
			AddPoolPointerEvent(someEvents[i]);
		}
	}

	void EventManager::ClearFlags()
	{
		myFlags.Clear();
	}

	bool EventManager::GetFlag(const std::string& aFlagName)
	{
		if (myFlags.KeyExists(aFlagName) == true)
		{
			return myFlags.Get(aFlagName).myFlag;
		}
		else
		{
			return false;
		}
	}

	void EventManager::SetFlag(const Flag& aFlag)
	{
		myFlags[aFlag.myID.GetMyTextID()] = aFlag;
	}

	float EventManager::GetDeltaTime()
	{
		return ourDeltaTime;
	}

	const double& EventManager::GetTimeSinceSceneStart()
	{
		return ourTimeSinceSceneStart;
	}

	void EventManager::ResetTimeSinceSceneStart()
	{
		ourTimeSinceSceneStart = 0;
	}

	void EventManager::AttachObserver(const AttachObserverData& someData)
	{
		if (myIsNotifing == false)
		{
			GetThreadEventData()->AddAttachObserver(someData);
		}
		else
		{
			myNotifyingAttachData.Add(someData);
		}
	}

	void EventManager::DetachObserver(Observer& anObserver)
	{
		if (myIsNotifing == false)
		{
			ThreadEventData* threadData = GetThreadEventData();
			for (unsigned short i = 0; i < anObserver.GetMyAttachEvents().Size(); i++)
			{
				DetachObserverData data;
				data.myAttachEvents = anObserver.GetMyAttachEvents()[i];
				data.myObserver = &anObserver;

				threadData->AddDetachObserver(data);

				bool found = false;
				unsigned short currentAttachEvent = GetEnumNumber(data.myAttachEvents.myEventType);

				for (unsigned short observerLoop = myObserverEvents[currentAttachEvent].myObservers.Size() - 1; observerLoop < USHRT_MAX - 1; observerLoop--)
				{
					if (myObserverEvents[currentAttachEvent].myObservers[observerLoop].myObserver == data.myObserver)
					{
						myObserverEvents[currentAttachEvent].myObservers[observerLoop].myIsActive = false;
						found = true;
						break;
					}
				}
			}
		}
		else
		{
			for (unsigned short detachLoop = anObserver.GetMyAttachEvents().Size() - 1; detachLoop < USHRT_MAX - 1; detachLoop--)
			{
				DetachObserverData data;
				data.myAttachEvents = anObserver.GetMyAttachEvents()[detachLoop];
				data.myObserver = &anObserver;

				unsigned short currentAttachEvent = GetEnumNumber(data.myAttachEvents.myEventType);

				bool found = false;

				for (unsigned short observerLoop = myNotifyingAttachData.Size() - 1; observerLoop < USHRT_MAX - 1; observerLoop--)
				{
					if (myNotifyingAttachData[observerLoop].myObserver == data.myObserver && myNotifyingAttachData[observerLoop].myEventType == data.myAttachEvents.myEventType)
					{
						myNotifyingAttachData.RemoveCyclicAtIndex(observerLoop);
						found = true;
						break;
					}
				}

				for (unsigned short observerLoop = myObserverEvents[currentAttachEvent].myObservers.Size() - 1; observerLoop < USHRT_MAX - 1; observerLoop--)
				{
					if (myObserverEvents[currentAttachEvent].myObservers[observerLoop].myObserver == data.myObserver)
					{
						myObserverEvents[currentAttachEvent].myObservers[observerLoop].myIsActive = false;
						found = true;
						break;
					}
				}

				if (found == false)
				{
					myNotifyingDetachData.Add(data);
				}
			}

		}
	}

	void EventManager::ExecuteObserverDetach(const DetachObserverData& someData)
	{

		bool found = false;
		unsigned short currentAttachEvent = GetEnumNumber(someData.myAttachEvents.myEventType);
		for (unsigned short observerLoop = myObserverEvents[currentAttachEvent].myObservers.Size() - 1; observerLoop < USHRT_MAX - 1; observerLoop--)
		{
			if (&*(myObserverEvents[currentAttachEvent].myObservers[observerLoop].myObserver) == &*someData.myObserver)
			{
				myObserverEvents[currentAttachEvent].myObservers.RemoveAtIndex(observerLoop);
				found = true;
			}
		}
		if (found == false)
		{
			//DL_ASSERT("Observer Not Found!");
		}
	}

	void EventManager::ExecuteObserverAttach(const AttachObserverData& someData)
	{
		if (someData.myHandleEventFunction._Empty() == false)
		{
			CU::GrowingArray<ObserverPrio>* observers = &(myObserverEvents[GetEnumNumber(someData.myEventType)].myObservers);

			ObserverPrio observerPrio;
			observerPrio.myObserver = someData.myObserver;
			observerPrio.myPrio = someData.aPriority;
			observerPrio.myIsActive = true;
			observerPrio.myHandleEventFunction = someData.myHandleEventFunction;
			for (unsigned short i = observers->Size() - 1; i < USHRT_MAX - 1; --i)
			{
				if ((*observers)[i].myObserver == someData.myObserver)
				{
					(*observers).RemoveCyclicAtIndex(i);
				}
			}
			for (unsigned short i = 0; i < observers->Size(); i++)
			{
				if ((*observers)[i].myPrio >= someData.aPriority)
				{
					observers->Insert(i, observerPrio);
					return;
				}
			}

			observers->Add(observerPrio);
		}
	}

	void EventManager::Create()
	{
		assert(ourEventManager == nullptr && "Eventmanager already initialized.");
		ourEventManager = new EventManager();
		ourEventManager->CreateObserverEvents();

		ourEventManager->RegisterThread();
		ourEventManager->myIsNotifing = false;
	}

	void EventManager::Create(CU::EventManager* someEventManager)
	{
		assert(ourEventManager == nullptr && "Eventmanager already initialized.");
		ourEventManager = someEventManager;
	}

	void EventManager::CreateObserverEvents()
	{
		for (unsigned short i = 0; i < static_cast<unsigned short>(eEvent::MY_SIZE); i++)
		{
			ObserverEvent newObserverEvent;
			newObserverEvent.myEventType = static_cast<eEvent>(i);
			newObserverEvent.myObservers.Init(4);
			myObserverEvents.Add(newObserverEvent);
		}
	}

	void EventManager::Update(const float aDeltaTime)
	{
		ourTimeSinceSceneStart += static_cast<double>(aDeltaTime);

		ourDeltaTime = aDeltaTime;
		ExecuteObserverFunctions();

		UpdateRenderBuffer();

		UpdateTimedEvents(aDeltaTime);
		NotifyObservers();
		ClearThreadData();
	}

	void EventManager::UpdateRenderBuffer()
	{
		myRenderBuffer.ClearBuffer();

		for (unsigned short i = 0; i < myThreadsEventData.Size(); i++)
		{
			CU::GrowingArray<SceneRenderCommands>& scenes = myThreadsEventData[i]->GetSceneRenderBuffer();

			for (unsigned short j = 0; j < scenes.Size(); j++)
			{
				myRenderBuffer.AddRenderCommands(scenes[j]);
			}
		}
	}

	void EventManager::ClearThreadData()
	{
		for (unsigned short i = 0; i < myThreadsEventData.Size(); i++)
		{
			myThreadsEventData[i]->Clear();
		}
	}

	void EventManager::ExecuteObserverFunctions()
	{
		for (unsigned short i = 0; i < myThreadsEventData.Size(); i++)
		{
			for (unsigned short j = 0; j < myThreadsEventData.Size(); j++)
			{
				myThreadsEventData[i]->CheckIfObserversExist(*myThreadsEventData[j]);
			}
		}

		for (unsigned short i = 0; i < myThreadsEventData.Size(); i++)
		{
			myThreadsEventData[i]->ExecuteObserverAttachFunctions();
		}

		for (unsigned short i = 0; i < myThreadsEventData.Size(); i++)
		{
			myThreadsEventData[i]->ExecuteObserverDetachFunctions();
		}
	}

	void EventManager::NotifyObservers()
	{
		bool startSceneOnce = false;

		myIsNotifing = true;

		myCurrentNotifyEvents.RemoveAll();


		for (unsigned short i = 0; i < myThreadsEventData.Size(); i++)
		{
			myCurrentNotifyEvents.Add(myThreadsEventData[i]->GetEventBuffer().GetMyEvents());
			myThreadsEventData[i]->GetEventBuffer().ClearBuffer();
		}

		do
		{
			myNotifyingEvents.RemoveAll();
			myNotifyingAttachData.RemoveAll();
			myNotifyingDetachData.RemoveAll();

			for (unsigned short eventLoop = 0; eventLoop < myCurrentNotifyEvents.Size(); eventLoop++)
			{
				if (myCurrentNotifyEvents[eventLoop] == nullptr)
				{
					continue;
				}
				eEvent currentType = myCurrentNotifyEvents[eventLoop]->GetMyType();

				switch (currentType)
				{
				case eEvent::TIMED_EVENT:
					myTimedEvents.Add(myCurrentNotifyEvents[eventLoop]);
					continue;
					break;

				case eEvent::SET_FLAG:

				{
					GET_EVENT_DATA(myCurrentNotifyEvents[eventLoop], SetFlagEvent, flagEvent);
					myFlags[flagEvent->myFlagName.c_str()].myFlag = flagEvent->mySetFlagTo;
				}

					continue;
					break;

				case eEvent::FUNCTION:
				{
					ExecuteObserverFunctions();

					GET_EVENT_DATA(myCurrentNotifyEvents[eventLoop], FunctionEvent, functionEvent);
					functionEvent->myFunction();
				}
					continue;
					break;

				case eEvent::START_SCENE:
					if (startSceneOnce == true)
					{
						continue;
					}
					startSceneOnce = true;

					break;
				default:
					break;
				}

				unsigned short currentEventNumber = GetEnumNumber(currentType);

				for (unsigned short observerLoop = 0; observerLoop < myObserverEvents[currentEventNumber].myObservers.Size(); observerLoop++)
				{
					if (myObserverEvents[currentEventNumber].myObservers[observerLoop].myIsActive == true)
					{
						ExecuteObserverFunctions();
						if (myObserverEvents[currentEventNumber].myObservers[observerLoop].myHandleEventFunction._Empty() == false)
						{
							if (myObserverEvents[currentEventNumber].myObservers[observerLoop].myHandleEventFunction(myCurrentNotifyEvents[eventLoop]) == false)
							{
								break;
							}
						}
					}
				}
			}
			myCurrentNotifyEvents = myNotifyingEvents;

			for (unsigned short i = 0; i < myNotifyingAttachData.Size(); i++)
			{
				ExecuteObserverAttach(myNotifyingAttachData[i]);
			}

			for (unsigned short i = 0; i < myNotifyingDetachData.Size(); i++)
			{
				ExecuteObserverDetach(myNotifyingDetachData[i]);
			}

		} while (myCurrentNotifyEvents.Size() > 0);


		myIsNotifing = false;
		ExecuteObserverFunctions();

	}

	void EventManager::UpdateTimedEvents(float aDeltaTime)
	{
		for (unsigned short i = myTimedEvents.Size() - 1; i != USHRT_MAX; i--)
		{
			GET_NON_CONST_EVENT_DATA(myTimedEvents[i], TimedEvent, timedEvent)

				float timeLeft = timedEvent->myTimeLeft - aDeltaTime;

			if (timeLeft < 0)
			{
				AddPoolPointerEvent(timedEvent->myPoolpointers);
				timedEvent->myFunction();
				myTimedEvents.RemoveCyclicAtIndex(i);
			}
			else
			{
				timedEvent->myTimeLeft = timeLeft;
			}
		}
	}

	void EventManager::Destroy()
	{
		if (ourEventManager != nullptr)
		{
			ourEventManager->myTimedEvents.RemoveAll();
			ourEventManager->myCurrentNotifyEvents.RemoveAll();
			ourEventManager->myNotifyingEvents.RemoveAll();

			ourEventManager->myThreadsEventData.DeleteAll();
			SAFE_DELETE(ourEventManager);
		}
	}

	CU::EventManager* EventManager::GetInstance()
	{
		return ourEventManager;
	}

	unsigned short EventManager::GetEnumNumber(const eEvent& anEnumNumber)
	{
		return static_cast<unsigned short>(anEnumNumber);
	}

	void EventManager::AddRenderCommand(const RenderCommandLine& aLineCommand)
	{
		GetThreadEventData()->AddRenderCommand(aLineCommand);
	}

	void EventManager::AddRenderCommand(const  GrowingArray<RenderCommandLine>& someLineCommands)
	{
		GetThreadEventData()->AddRenderCommand(someLineCommands);
	}

	void EventManager::AddRenderCommand(const RenderCommandFont& aCommandFont)
	{
		GetThreadEventData()->AddRenderCommand(aCommandFont);
	}

	void EventManager::AddRenderCommand(const CU::GrowingArray<RenderCommandFont>& someCommandFonts)
	{
		GetThreadEventData()->AddRenderCommand(someCommandFonts);
	}

	void EventManager::AddRenderCommand(const RenderCommandInstance& aRenderCommandInstance)
	{
		GetThreadEventData()->AddRenderCommand(aRenderCommandInstance);
	}

	void EventManager::AddRenderCommand(const CU::GrowingArray<RenderCommandInstance>& aRenderCommandInstance)
	{
		GetThreadEventData()->AddRenderCommand(aRenderCommandInstance);
	}

	void EventManager::SetCurrentScene(const ScenePrioID& aPrioID)
	{
		GetThreadEventData()->SetCurrentScene(aPrioID);
	}

	void EventManager::AddSceneData(const GE::SceneRenderData& someSceneData)
	{
		GetThreadEventData()->AddSceneData(someSceneData);
	}

	EventManager::EventManager()
	{
		ourDeltaTime = 0;
		ourTimeSinceSceneStart = 0;
		myTimedEvents.Init(4);
		myObserverEvents.Init(4);
		myThreadsEventData.Init(4);
		myEventWithObserverNullPointers.Init(4);
		myNotifyingEvents.Init(4);
		myCurrentNotifyEvents.Init(4);
		myNotifyingAttachData.Init(4);
		myNotifyingDetachData.Init(4);
	}

	EventManager::~EventManager()
	{
	}

	ThreadEventData* EventManager::GetThreadEventData()
	{
		std::thread::id currentThreadID = std::this_thread::get_id();

		for (unsigned short i = 0; i < myThreadsEventData.Size(); i++)
		{
			if (*myThreadsEventData[i] == currentThreadID)
			{
				return myThreadsEventData[i];
			}
		}

		DL_ASSERT("No threadEventData created for this thread.");

		return myThreadsEventData[0];
	}

	void EventManager::RegisterThread()
	{
		ThreadEventData* newData = new ThreadEventData();
		newData->Init();
		myThreadsEventData.Add(newData);
	}

	RenderBuffer& EventManager::GetRenderBuffer()
	{
		return myRenderBuffer;
	}

	void EventManager::ClearRenderData()
	{
		myRenderBuffer.ClearBuffer();
		for (unsigned short i = 0; i < myThreadsEventData.Size(); i++)
		{
			CU::GrowingArray<CU::SceneRenderCommands> &commands = myThreadsEventData[i]->GetSceneRenderBuffer();

			for (unsigned short j = 0; j < commands.Size(); j++)
			{
				commands[j].myRenderCommandInstances.RemoveAll();
			}
		}
	}

	void EventManager::ClearTimedEvents()
	{
	}

}

namespace CU = CommonUtilities;
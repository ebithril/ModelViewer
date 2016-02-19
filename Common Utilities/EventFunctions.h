#pragma once
#include "Event.h"
#include "EventEnums.h"
#include "PoolPointer.h"
#include "Events.h"
#include "EventManager.h"


namespace CommonUtilities
{
	CU::PoolPointer<Event> ToolEventToEvent(const EventStruct& anEventStruct);

	CU::PoolPointer<Event> ToolEventToEvent(const EventStruct& anEventStruct)
	{
		TimedEvent timedEvent;
		timedEvent.myTimeLeft = anEventStruct.myTimeDelay;
		timedEvent.myPoolpointers.Init(1);

		switch (anEventStruct.myEventType)
		{
			/*case eToolEvents::SPAWN_ENEMY_WITHIN_AREA:


				return eEvent::SPAWN_ENEMY_WITHIN_AREA;*/

		case eToolEvents::SET_FLAG:
		{
			SetFlagEvent setFlag;
			setFlag.SetMyType(eEvent::SET_FLAG);
			setFlag.myFlagName = anEventStruct.myFirstStringArgument.c_str();
			setFlag.mySetFlagTo = anEventStruct.myFlag;

			timedEvent.myPoolpointers.Add(CU::EventManager::GetInstance()->CreateInternalEvent(setFlag));

		}
			break;

		case eToolEvents::START_SCENE:
		{
			StartSceneEvent startEvent;
			startEvent.SetMyType(eEvent::START_SCENE);
			startEvent.mySceneName = anEventStruct.myFirstStringArgument.c_str();

			timedEvent.myPoolpointers.Add(CU::EventManager::GetInstance()->CreateInternalEvent(startEvent));
		}
			break;

		case eToolEvents::QUIT:
		{
			Event newEvent;
			newEvent.SetMyType(eEvent::QUIT);
			timedEvent.myPoolpointers.Add(CU::EventManager::GetInstance()->CreateInternalEvent(newEvent));
		}
			break;

		case eToolEvents::PLAY_SUBTITLE:
		{
			PlaySubtitleEvent subtitleEvent;
			subtitleEvent.SetMyType(eEvent::PLAY_SUBTITLE);
			subtitleEvent.mySubtitle = anEventStruct.myFirstStringArgument.c_str();

			timedEvent.myPoolpointers.Add(CU::EventManager::GetInstance()->CreateInternalEvent(subtitleEvent));

		}
			break;

		case eToolEvents::REMOVE_CHILD_STATE:
		{
			Event newEvent;
			newEvent.SetMyType(eEvent::REMOVE_CHILD_STATE);
			timedEvent.myPoolpointers.Add(CU::EventManager::GetInstance()->CreateInternalEvent(newEvent));
		}
			break;

		case eToolEvents::REMOVE_MAIN_STATE:
		{
			Event newEvent;
			newEvent.SetMyType(eEvent::REMOVE_MAIN_STATE);
			timedEvent.myPoolpointers.Add(CU::EventManager::GetInstance()->CreateInternalEvent(newEvent));
		}
			break;



		default:
			DL_ASSERT("ToolEvent not implemented in converter.");
			break;
		}

		return CU::EventManager::GetInstance()->CreateInternalEvent(timedEvent);
	}
}

namespace CU = CommonUtilities;
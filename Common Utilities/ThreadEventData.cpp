#include "Common Utilities_Precompiled.h"
#include "ThreadEventData.h"
#include "EventManager.h"
#include "../Engine/SceneRenderData.h"
#include "../Engine/ParticleEmitter.h"
#include "../Engine/StreakEmitter.h"

namespace CommonUtilities
{
	ThreadEventData::ThreadEventData()
	{
	}

	ThreadEventData::~ThreadEventData()
	{

	}

	void ThreadEventData::Init()
	{
		myThreadID = std::this_thread::get_id();
		mySceneRenderCommands.Init(4);
		myDetachObserverData.Init(32);
		myAttachObserverData.Init(32);
		myEventBuffer.Init();
	}

	void ThreadEventData::Clear()
	{
		mySceneRenderCommands.RemoveAll();
		myEventBuffer.ClearBuffer();
	}

	const bool ThreadEventData::operator == (const std::thread::id& anID)
	{
		if (anID == myThreadID)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void ThreadEventData::AddDetachObserver(const DetachObserverData& someData)
	{
		myDetachObserverData.Add(someData);
	}

	void ThreadEventData::AddAttachObserver(const AttachObserverData& someData)
	{
		myAttachObserverData.Add(someData);
	}

	void ThreadEventData::CheckIfObserversExist(ThreadEventData& someData)
	{
		for (unsigned short i = 0; i < someData.myAttachObserverData.Size(); i++)
		{
			for (unsigned short j = 0; j < myDetachObserverData.Size(); j++)
			{
				if (&*(someData.myAttachObserverData[i].myObserver) == &*(myDetachObserverData[j].myObserver))
				{
					someData.myAttachObserverData.RemoveAtIndex(i);
					myDetachObserverData.RemoveAtIndex(j);
					i--;
					j--;
					break;
				}
			}
		}
	}

	void ThreadEventData::ExecuteObserverAttachFunctions()
	{
		for (unsigned short i = 0; i < myAttachObserverData.Size(); i++)
		{
			CU::EventManager::GetInstance()->ExecuteObserverAttach(myAttachObserverData[i]);
		}
		myAttachObserverData.RemoveAll();
	}

	void ThreadEventData::ExecuteObserverDetachFunctions()
	{
		for (unsigned short i = 0; i < myDetachObserverData.Size(); i++)
		{
			CU::EventManager::GetInstance()->ExecuteObserverDetach(myDetachObserverData[i]);
		}
		myDetachObserverData.RemoveAll();

	}

	void ThreadEventData::AddSceneData(const GE::SceneRenderData& someSceneData)
	{
		for (unsigned short i = 0; i < mySceneRenderCommands.Size(); i++)
		{
			if (mySceneRenderCommands[i].myPrioID.myID == myCurrentPrioID.myID)
			{
				if (mySceneRenderCommands[i].mySceneData != nullptr)
				{
					SAFE_DELETE(mySceneRenderCommands[i].mySceneData);
				}
				mySceneRenderCommands[i].mySceneData = new GE::SceneRenderData(someSceneData);
				return;
			}
		}
		SceneRenderCommands commands;
		commands.myPrioID.myID = myCurrentPrioID.myID;
		commands.mySceneData = new GE::SceneRenderData(someSceneData);

		mySceneRenderCommands.Add(commands);


	}
	void ThreadEventData::AddRenderCommand(const RenderCommandInstance& aRenderCommandInstance)
	{
		for (unsigned short i = 0; i < mySceneRenderCommands.Size(); i++)
		{
			if (myCurrentPrioID.myID == mySceneRenderCommands[i].myPrioID.myID)
			{
				mySceneRenderCommands[i].myRenderCommandInstances.Add(aRenderCommandInstance);
				return;
			}
		}

		SceneRenderCommands commands;
		commands.myPrioID = myCurrentPrioID;
		commands.myRenderCommandInstances.Add(aRenderCommandInstance);

		mySceneRenderCommands.Add(commands);
	}

	void ThreadEventData::AddRenderCommand(const CU::GrowingArray<RenderCommandInstance>& aRenderCommandInstance)
	{
		for (unsigned short i = 0; i < aRenderCommandInstance.Size(); i++)
		{
			AddRenderCommand(aRenderCommandInstance[i]);
		}
	}

	void ThreadEventData::AddRenderCommand(const RenderCommandFont& aCommandFont)
	{
		for (unsigned short i = 0; i < mySceneRenderCommands.Size(); i++)
		{
			if (myCurrentPrioID.myID == mySceneRenderCommands[i].myPrioID.myID)
			{
				mySceneRenderCommands[i].myFontRenderCommands.Add(aCommandFont);
				return;
			}
		}

		SceneRenderCommands commands;
		commands.myPrioID = myCurrentPrioID;
		commands.myFontRenderCommands.Add(aCommandFont);

		mySceneRenderCommands.Add(commands);
	}

	void ThreadEventData::AddRenderCommand(const GrowingArray<RenderCommandFont>& someCommandFonts)
	{
		for (unsigned short i = 0; i < someCommandFonts.Size(); i++)
		{
			AddRenderCommand(someCommandFonts[i]);
		}
	}

	void ThreadEventData::AddRenderCommand(const RenderCommandLine& aLineCommand)
	{
		for (unsigned short i = 0; i < mySceneRenderCommands.Size(); i++)
		{
			if (myCurrentPrioID.myID == mySceneRenderCommands[i].myPrioID.myID)
			{
				mySceneRenderCommands[i].myLineRenderCommands.Add(aLineCommand);
				return;
			}
		}

		SceneRenderCommands commands;
		commands.myPrioID.myID = myCurrentPrioID.myID;
		commands.myLineRenderCommands.Add(aLineCommand);

		mySceneRenderCommands.Add(commands);
	}

	void ThreadEventData::AddRenderCommand(const GrowingArray<RenderCommandLine>& someLineCommands)
	{
		for (unsigned short i = 0; i < someLineCommands.Size(); i++)
		{
			AddRenderCommand(someLineCommands[i]);
		}
	}

	EventBuffer& ThreadEventData::GetEventBuffer()
	{
		return myEventBuffer;
	}

	CU::GrowingArray<SceneRenderCommands>& ThreadEventData::GetSceneRenderBuffer()
	{
		return mySceneRenderCommands;
	}

	void ThreadEventData::SetCurrentScene(const ScenePrioID& aPrioID)

	{
		myCurrentPrioID = aPrioID;
	}

	const ScenePrioID& ThreadEventData::GetCurrentScene()
	{
		return myCurrentPrioID;
	}

}
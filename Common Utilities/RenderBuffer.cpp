#include "Common Utilities_Precompiled.h"
#include "RenderBuffer.h"
#include "../Engine/SceneRenderData.h"

namespace CommonUtilities
{

	RenderBuffer::RenderBuffer()
	{
		myCommands.Init(4);
	}

	RenderBuffer::~RenderBuffer()
	{

	}

	void RenderBuffer::AddRenderCommands(const SceneRenderCommands& someRenderCommands)
	{

		for (unsigned short i = 0; i < myCommands.Size(); i++)
		{
			if (myCommands[i].myPrioID.myID == someRenderCommands.myPrioID.myID)
			{
				myCommands[i].AddSceneRenderCommands(someRenderCommands);
				return;
			}
		}

		myCommands.Add(someRenderCommands);
	}

	void RenderBuffer::ClearBuffer()
	{
		for (unsigned short i = 0; i < myCommands.Size(); i++)
		{
			SAFE_DELETE(myCommands[i].mySceneData);
		}

		myCommands.RemoveAll();
	}

	SceneRenderCommands& RenderBuffer::GetRenderCommands(const CU::ID& aSceneID)
	{
		for (unsigned short i = 0; i < myCommands.Size(); i++)
		{

			if (myCommands[i].myPrioID.myID == aSceneID)
			{
				return myCommands[i];
			}
		}
		DL_ASSERT("Couldn't find scene while rendering.");

		return myCommands[0];
	}

	CU::GrowingArray<ScenePrioID> RenderBuffer::GetSceneIDInRenderOrder()
	{
		if (myCommands.Size() != 0)
		{
			CU::GrowingArray<ScenePrioID> myScenes(myCommands.Size());
			myScenes.Add(myCommands[0].myPrioID);
			
			for (unsigned short i = 1; i < myCommands.Size(); i++)
			{
				for (unsigned short j = 0; j < myScenes.Size(); j++)
				{
					if (myScenes[j].myPriority > myCommands[i].myPrioID.myPriority)
					{
						myScenes.Insert(j, myCommands[i].myPrioID);
					}
				}
				myScenes.Add(myCommands[i].myPrioID);
			}
			return myScenes;
		}


		return CU::GrowingArray<ScenePrioID>(2);
	}

}
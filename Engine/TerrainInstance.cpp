#include "Engine_Precompiled.h"
#include "TerrainInstance.h"
#include "SceneRenderData.h"
#include "LightBuffer.h"
#include "Model.h"

namespace GraphicsEngine
{
	TerrainInstance::TerrainInstance() : Instance(CU::PoolPointer<Model>())
	{
		myModel = nullptr;
	}

	TerrainInstance::~TerrainInstance()
	{
	}

	void TerrainInstance::SetModel(SharedPointer<Model> aModel)
	{
		myModel = aModel;
	}

	void TerrainInstance::Render(SceneRenderData& someScenerRenderData, CU::RenderCommandInstance& aRenderCommand)
	{
		if (myModel != nullptr)
		{
			for (unsigned short i = 0; i < someScenerRenderData.myLightBuffer->GetReadLightDataBuffer().Size() && i < 10; i++)
			{
				aRenderCommand.myLightIndexes.Add(i);
			}
			myModel->Render(someScenerRenderData, aRenderCommand,aRenderCommand.myOrientation, true);
		}
	}


	void TerrainInstance::RenderShadowData(Effect* anEffect, Matrix44f anOrientation)
	{
		if (myModel != nullptr)
		{
			myModel->RenderShadowShaderData(anEffect, anOrientation);
		}
	}

	Effect* TerrainInstance::GetEffect()
	{
		return myModel->GetEffect();
	}

	bool TerrainInstance::ModelIsNullPointer()
	{
		if (myModel == nullptr)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool TerrainInstance::IsLoaded() const
	{
		if (myModel != nullptr)
		{
			return true;
		}
		return false;
	}


}
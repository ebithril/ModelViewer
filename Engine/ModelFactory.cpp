#include "Engine_Precompiled.h"
#include "ModelFactory.h"
#include "Model.h"
#include "Effect.h"
#include "DirectXEngine.h"
#include "FBXFactory.h"
#include <string>
#include "../Common Utilities/TimerManager.h"
#include "../Common Utilities/CUString.h"
#include "../Common Utilities/ThreadPool.h"
#include "../Common Utilities/EventManager.h"
#include "TextureFactory.h"
#include "GfxFactoryWrapper.h"
#include "EffectFactory.h"
#include "WindowsFunctions.h"

namespace GraphicsEngine
{
	ModelFactory::ModelFactory()
	{

	}

	ModelFactory::~ModelFactory()
	{
		DeleteAll();
		SAFE_DELETE(myFBXFactory);
	}

	void ModelFactory::Init(COMObjectPointer<ID3D11Device>& aDevice)
	{
		myObjectPool.Init();
		myLoadingState = eLoaderState::IN_GAME;
		myModelNames.Init(16);
		myModelCount.Init(16);
		myModelsToNoUnload.Init(16);
		myTextureFactory = GfxFactoryWrapper::GetInstance()->GetTextureFactory();
		myDevice = aDevice;
		myFBXFactory = new FBXFactory(myTextureFactory);
	}

	CU::PoolPointer<Model> ModelFactory::AllocateEmptyChild()
	{
		return myObjectPool.Allocate<Model, Model>();
	}

	void ModelFactory::CreateModel(const std::string& aModelName, Effect* anEffect, const eModelType aModelType, const float aSize, Texture* aTexture)
	{
		START_LOG_TIMER();

		CU::PoolPointer<Model> newModel = myObjectPool.Allocate<Model, Model>();

		switch (aModelType)
		{

		case POLYGON:
			newModel->InitPolygon(myDevice, anEffect, aSize);
			break;

		case CUBE:
			newModel->InitCube(myDevice, anEffect, aSize);
			break;

		case CUBE_NORM_TEX:
			newModel->InitNormCube(myDevice, anEffect, aSize, aTexture);
			break;
		case SKYBOX:
			newModel->InitSkyBox(myDevice, anEffect, aSize, aTexture);
			break;
		case CUBEVIDEO:
			newModel->InitNormCubeVideo(myDevice, anEffect, aSize, aTexture);

			break;
		default:
			DL_ASSERT("Failed at model creation.");
			break;
		}

		myModelNames.Add(aModelName);
		myModels.Insert(aModelName, newModel);

		float timeSpent = END_LOG_TIMER();
		CU::String<200> message = "Loaded model with name: ";
		message += aModelName.c_str();
		message += ".\nTime spent: ";
		message += timeSpent;
		message += " milliseconds.";
		RESOURCE_LOG(message.c_str());
	}

	CU::PoolPointer<Model> ModelFactory::CreateUnsearchableModelFromBufferData(GE::VertexDataWrapper& someVertexData, GE::VertexIndexWrapper& someIndexData)
	{
		CU::PoolPointer<Model> newModel = myObjectPool.Allocate<Model, Model>();
		newModel->InitFromBufferData(someVertexData, someIndexData);
		return newModel;
	}

	void ModelFactory::CreateModel(const std::string& aFileName, Effect* anEffect)
	{
		START_LOG_TIMER();

		CU::PoolPointer<Model> currentModel = myObjectPool.Allocate<Model, Model>();;
		myFBXFactory->LoadModel(myDevice, aFileName.c_str(), currentModel);

		currentModel->SetEffect(anEffect);

		std::string modelID = WF::GetFileNameWithoutExtension(aFileName);

		myModelNames.Add(modelID);
		myModels.Insert(modelID, currentModel);

		float timeSpent = END_LOG_TIMER();
		CU::String<200> message = "Loaded model with name: ";
		message += modelID.c_str();
		message += ".\nTime spent: ";
		message += timeSpent;
		message += " milliseconds.";
		RESOURCE_LOG(message.c_str());
	}

	void ModelFactory::CreateModelFromEmptyModel(const std::string& aFileName, Effect* anEffect)
	{
		START_LOG_TIMER();
		std::string modelID = WF::GetFileNameWithoutExtension(aFileName);


		CU::PoolPointer<Model> currentModel = myModels.Get(modelID);
		myFBXFactory->LoadModel(myDevice, aFileName.c_str(), currentModel);


		if (currentModel->GetEffect() != nullptr)
		{
			currentModel->SetEffect(currentModel->GetEffect());
		}
		else
		{
			currentModel->SetEffect(anEffect);
		}

		float timeSpent = END_LOG_TIMER();
		CU::String<200> message = "Loaded model from empty model with name: ";
		message += modelID.c_str();
		message += ".\nTime spent: ";
		message += timeSpent;


		message += " milliseconds.";
		RESOURCE_LOG(message.c_str());
	}

	CU::PoolPointer<Model> ModelFactory::GetModel(const std::string& aModelName)
	{
		if (myModels.KeyExists(aModelName))
		{
			CU::PoolPointer<Model> model = myModels.Get(aModelName);
			if (myLoadingState == eLoaderState::IN_GAME)
			{
				if (model->IsLoaded() == false)
				{
					CU::String<128> errortext = "Needed to Load Model while in game with name ";
					errortext += aModelName.c_str();

					DL_PRINT(errortext.c_str());
					CreateModelFromEmptyModel(model->GetFileName(), GfxFactoryWrapper::GetInstance()->GetEffectFactory()->GetEffect("VertexPosNormUVBiTan"));
				}
			}
			return model;
		}
		else
		{
			std::string errorText = "Model with name " + aModelName + " doesn't exist in model factory.";
			DL_PRINT(errorText.c_str());
			return myModels.Get("sphere");
			//return nullptr;
		}
	}

	void ModelFactory::LoadAnimation(CU::PoolPointer<Model> aModel, const std::string& aAnimationName, const std::string& aFileName)
	{
		if (myModels.KeyExists(aFileName))
		{
			myFBXFactory->LoadAnimation(myDevice, myModels[aFileName]->GetFileName().c_str(), aModel, aAnimationName);
		}
	}

	void ModelFactory::DeleteAll()
	{
		for (unsigned short i = 0; i < myModelNames.Size(); i++)
		{
			myModels[myModelNames[i]] = nullptr;
		}

		for (unsigned short i = 0; i < myModelCount.Size(); i++)
		{
			myModelCount[i] = nullptr;
		}

		for (unsigned short i = 0; i < myModelsToNoUnload.Size(); i++)
		{
			myModelsToNoUnload[i] = nullptr;
		}
	}

	void ModelFactory::CreateAllEmptyModels()
	{
		CU::GrowingArray<std::string> files = WF::GetFilesWithExtension("Data\\Assets\\Models\\", ".fbx");

		for (unsigned short i = 0; i < files.Size(); i++)
		{
			CreateEmptyModel(files[i], GfxFactoryWrapper::GetInstance()->GetEffectFactory()->GetEffect("VertexPosNormUVBiTan"));
		}
	}

	void ModelFactory::CreateEmptyModel(const std::string& aModelName, Effect* anEffect)
	{
		anEffect;
		CU::PoolPointer<Model> newModel = myObjectPool.Allocate<Model, Model>(aModelName);

		std::string modelID = WF::GetFileNameWithoutExtension(aModelName);
		if (modelID == "uiHealth" || modelID == "uiShield")
		{
			newModel->SetUpdateDynamic(true);
		}
		myModelCount.Add(newModel);
		myModelNames.Add(modelID);
		myModels.Insert(modelID, newModel);
	}

	void ModelFactory::DoNotUnloadModel(const std::string& aModelName)
	{
		myModelsToNoUnload.Add(GetModel(aModelName));
	}

	void ModelFactory::LoadRequestedModels()
	{
		for (unsigned short i = 0; i < myModelCount.Size(); i++)
		{
			if (myModelCount[i]->IsLoaded() == false)
			{
				if (myModelCount[i].GetReferenceCount() > 2)
				{
					//CU::Work loadWork([this, i]{CreateModelFromEmptyModel(myModelCount[i]->GetFileName(), GfxFactoryWrapper::GetInstance()->GetEffectFactory()->GetEffect("VertexPosNormUVBiTan")); });
					//CU::ThreadPool::GetInstance()->AddWork(loadWork);

					CreateModelFromEmptyModel(myModelCount[i]->GetFileName(), GfxFactoryWrapper::GetInstance()->GetEffectFactory()->GetEffect("VertexPosNormUVBiTan"));
				}
			}
		}
	}

	void ModelFactory::UnloadUnusedModels()
	{
		for (unsigned short i = 0; i < myModelCount.Size(); i++)
		{
			if (myModelCount[i]->IsLoaded() == true)
			{
				if (myModelCount[i].GetReferenceCount() <= 2)
				{
					myModelCount[i]->UnLoad();
				}
			}
		}
	}

	void ModelFactory::SetLoaderState(const eLoaderState aState)
	{
		myLoadingState = aState;
	}
}
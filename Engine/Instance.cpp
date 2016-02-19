#include "Engine_Precompiled.h"
#include "Instance.h"

#include "Model.h"
#include "Animation.h"
#include "Effect.h"
#include "TransformationNode.h"
#include "..\Common Utilities\EventManager.h"
#include "d3dx11effect.h"
#include "AnimationInstance.h"
#include "GfxFactoryWrapper.h"
#include "ModelFactory.h"
#include "SceneRenderData.h"

#include "../Common Utilities/VTuneAPI.h"

VTUNE_API_CREATE_DOMAIN(locUpdateAnimationDomain, "AnimationUpdateDomain");
VTUNE_API_CREATE_HANDLE("UpdateAnimation", locUpdateAnimationTimeStamp);

VTUNE_API_CREATE_DOMAIN(locRenderDomain, "RenderInstanceDomain");
VTUNE_API_CREATE_HANDLE("RenderInstance", locRenderTimeStamp);

namespace GraphicsEngine
{
	Instance::Instance(CU::PoolPointer<Model> aModel)
	{
		myIsInited = false;
		myCurrentAnimationTime = 0;
		myModel = aModel;
		myAnimation = nullptr;
	}

	Instance::Instance(const Instance& aInstance)
	{
		myIsInited = aInstance.myIsInited;
		myModel = aInstance.myModel;
		myHierarchy = aInstance.myHierarchy;
		myAnimation = aInstance.myAnimation;
		myBones = aInstance.myBones;
		myAnimationsToLoad = aInstance.myAnimationsToLoad;
		myCurrentAnimationTime = 0;
	}

	Instance::~Instance()
	{
		delete myAnimation;
	}

	void Instance::Init()
	{
		if (myModel != nullptr)
		{
			myAnimation = new AnimationInstance();

			myIsInited = true;
			if (myAnimationsToLoad.IsInited())
			{
				for each (AnimationToLoad data in myAnimationsToLoad)
				{
					LoadAnimation(data.myAnimationName, data.myFileName);
				}
			}

			myAnimation->SetAnimations(myModel->myAnimation);
			myAnimation->PlayAnimation("Idle", true);

			BuildHierarchy();

		}
	}

	void Instance::Render(SceneRenderData& someScenerRenderData, CU::RenderCommandInstance& aRenderCommand)
	{
		if (myModel != nullptr && myModel->IsLoaded() == true && myIsInited == true)
		{
			if (aRenderCommand.myDiffuseTexture != nullptr)
			{
				myModel->SetDiffuseTexture(aRenderCommand.myDiffuseTexture);
			}
			else
			{
				myModel->SetDiffuseTexture(myModel->GetOriginalDiffuseTexture());
			}

			if (aRenderCommand.myEmmisiveTexture != nullptr)
			{
				myModel->SetEmissiveTexture(aRenderCommand.myEmmisiveTexture);
			}
			else
			{
				myModel->SetEmissiveTexture(myModel->GetOriginalEmissiveTexture());
			}


			if (myAnimation != nullptr)
			{
				VTUNE_API_TASK_BEGIN(locUpdateAnimationDomain, locUpdateAnimationTimeStamp);
				myAnimation->GetBoneMatrices(myBones);
				VTUNE_API_TASK_END(locUpdateAnimationDomain);
			}

			VTUNE_API_TASK_BEGIN(locRenderDomain, locRenderTimeStamp);
			GE::Effect* engineEffect = myModel->GetEffect();
			if (engineEffect != nullptr)
			{
				GE::COMObjectPointer<ID3DX11Effect> dxEffect = engineEffect->GetEffect();
				if (dxEffect->IsValid())
				{
					ID3DX11EffectMatrixVariable* bonesVar = dxEffect->GetVariableByName("myBones")->AsMatrix();
					if (bonesVar->IsValid())
					{
						bonesVar->SetMatrixArray((float*)&myBones[0], 0, 32);
					}

					if (aRenderCommand.myDebugColor != Vector4f(-1, -1, -1, -1))
					{
						ID3DX11EffectVectorVariable* colorVar = dxEffect->GetVariableByName("Color")->AsVector();
						if (colorVar->IsValid())
						{
							colorVar->SetFloatVector(&aRenderCommand.myDebugColor.r);
						}
					}
				}
			}
			RenderModel(someScenerRenderData, aRenderCommand, &*myModel, aRenderCommand.myOrientation, myHierarchy, true);
			VTUNE_API_TASK_END(locRenderDomain);
		}
	}

	void Instance::RenderShadowData(Effect* anEffect, Matrix44f anOrientation)
	{
		ID3DX11EffectMatrixVariable* bonesVar = anEffect->GetEffect()->GetVariableByName("myBones")->AsMatrix();
		if (bonesVar->IsValid())
		{
			bonesVar->SetMatrixArray((float*)&myBones[0], 0, 32);
		}
		RenderShadowData(anEffect, anOrientation, &*myModel, myHierarchy);
	}

	void Instance::RenderShadowData(Effect* anEffect, Matrix44f aParentSpace, Model* aModel, TransformationNodeInstance& aHierarchy)
	{
		aModel->RenderShadowShaderData(anEffect, aParentSpace);
		for (unsigned short i = 0; i < aModel->myChilds.Size(); i++)
		{
			if (aHierarchy.GetChildren().IsInited() == true && aHierarchy.GetChildren().Size() == aModel->myChilds.Size())
			{
				Matrix44<float> worldMatrix = aHierarchy.GetChildren()[i].GetTransformation() * aParentSpace;

				RenderShadowData(anEffect, worldMatrix, &*aModel->myChilds[i], aHierarchy.GetChildren()[i]);
			}
		}
	}

	void Instance::Update()
	{
		if (myIsInited == false && myModel != nullptr && myModel->IsLoaded() == true)
		{
			Init();
		}

		if (myIsInited == true)
		{
			myAnimation->Update(CU::EventManager::GetInstance()->GetDeltaTime());

			myHierarchy.Update();
		}
	}

	const bool Instance::AnimationExists(const std::string& aAnimationName) const
	{
		return myModel->AnimationIsLoaded(aAnimationName);
	}

	void Instance::BuildHierarchy(TransformationNodeInstance& aHierarchy, Model* aModel)
	{
		if (aHierarchy.GetChildren().IsInited() == false)
		{
			aHierarchy.Init();
		}

		for (unsigned short i = 0; i < aModel->myChilds.Size(); i++)
		{
			if (aModel->myChilds[i]->myTransform == nullptr)
			{
				aModel->myChilds[i]->myTransform = new TransformationNode;
			}

			aModel->myChildTransforms[i] = aModel->myChilds[i]->myTransform;

			TransformationNodeInstance instance;
			instance.SetTransformationNode(aModel->myChildTransforms[i]);
			aHierarchy.AddChildNode(instance);

			BuildHierarchy(aHierarchy.GetChildren()[i], &*aModel->myChilds[i]);
		}
	}

	void Instance::BuildHierarchy()
	{
		BuildHierarchy(myHierarchy, &*myModel);
	}

	Effect* Instance::GetEffect()
	{
		if (myModel != nullptr)
		{
			return myModel->GetEffect();
		}
		else
		{
			return nullptr;
		}
	}

	void Instance::LoadAnimation(const std::string& aAnimationName, const std::string& aFileName)
	{
		if (myIsInited == false)
		{
			if (myAnimationsToLoad.IsInited() == false)
			{
				myAnimationsToLoad.Init(8);
			}
			AnimationToLoad data;
			data.myAnimationName = aAnimationName;
			data.myFileName = aFileName;

			myAnimationsToLoad.Add(data);
			return;
		}

		if (AnimationExists(aAnimationName) == false)
		{
			GfxFactoryWrapper::GetInstance()->GetModelFactory()->LoadAnimation(myModel, aAnimationName, aFileName);
		}
	}

	void Instance::StartAnimation(const std::string aName, bool aShouldLoop)
	{
		if (myIsInited == true)
		{
			myAnimation->PlayAnimation(aName, aShouldLoop);
		}
	}

	void Instance::StopAnimation(const std::string aName)
	{
		if (myIsInited == true)
		{
			myAnimation->StopAnimation(aName);
		}
	}

	bool Instance::ModelIsNullPointer()
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

	bool Instance::IsLoaded()const
	{
		if (myModel != nullptr)
		{
			return myModel->IsLoaded();
		}
		return false;
	}

	Texture* Instance::GetOriginalDiffuse()
	{
		return myModel->GetOriginalDiffuseTexture();
	}

	void Instance::RenderModel(SceneRenderData& someScenerRenderData, CU::RenderCommandInstance& aRenderCommand, Model* aModel, const Matrix44<float>& aParentSpace,
		TransformationNodeInstance& aHierarchy, const bool anObjectFirstTimeRender)
	{
		aModel->Render(someScenerRenderData, aRenderCommand,aParentSpace, anObjectFirstTimeRender);
		for (unsigned short i = 0; i < aModel->myChilds.Size(); i++)
		{
			if (aHierarchy.GetChildren().IsInited() == true)
			{
				Matrix44<float> worldMatrix = aHierarchy.GetChildren()[i].GetTransformation() * aParentSpace;

				RenderModel(someScenerRenderData, aRenderCommand, &*aModel->myChilds[i], worldMatrix, aHierarchy.GetChildren()[i], false);
			}		
		}
	}
}
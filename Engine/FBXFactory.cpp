#include "Engine_Precompiled.h"
#include "../FBXLoader/FbxLoader.h"
#include "FBXFactory.h"
#include "VertexIndexWrapper.h"
#include "VertexDataWrapper.h"
#include "Model.h"
#include "Surface.h"
#include "ModelDataStruct.h"
#include <d3d11_2.h>
#include "TextureFactory.h"
#include "TextureType.h"
#include "ModelFactory.h"
#include "GfxFactoryWrapper.h"

#include "KeyFrame.h"
#include "TransformationNode.h"
#include "TransformationNodeCurves.h"
#include "TransformationNodeHierarchy.h"
#include "AnimationCurve.h"
#include "Animation.h"
#include "AnimationNode.h"
#include "AnimationContainer.h"

namespace GraphicsEngine
{
	FBXFactory::FBXFactory(TextureFactory* aTextureFactory)
	{
		myLoader = new FBXLoader();
		myTextureFactory = aTextureFactory;
	}

	FBXFactory::~FBXFactory()
	{
		SAFE_DELETE(myLoader);
	}

	void FBXFactory::FillData(COMObjectPointer<ID3D11Device>& aDevice, ModelData* someData, CU::PoolPointer<Model>& outData)
	{
		outData->myOriginalDiffuse = myTextureFactory->GetTexture("Black"), eTextureType::DIFFUSE;
		outData->myOriginalEmissive = myTextureFactory->GetTexture("Black"), eTextureType::EMISSIVE;

		ModelDataStruct data;
		GE::VertexIndexWrapper* indexWrapper = new GE::VertexIndexWrapper();
		indexWrapper->myFormat = DXGI_FORMAT_R32_UINT;
		unsigned int* indexData = new unsigned int[someData->myIndexCount];
		memcpy(indexData, someData->myIndicies, someData->myIndexCount*sizeof(unsigned int));
		indexWrapper->myIndexData = (char*)indexData;
		indexWrapper->mySize = someData->myIndexCount*sizeof(unsigned int);
		indexWrapper->myNumberOfIndexes = someData->myIndexCount;
		data.myIndexData = indexWrapper;

		GE::VertexDataWrapper* vertexData = new GE::VertexDataWrapper();
		someData->myVertexBuffer;
		int sizeOfBuffer = someData->myVertexCount*someData->myVertexStride*sizeof(float);
		char* vertexRawData = new char[sizeOfBuffer];
		memcpy(vertexRawData, someData->myVertexBuffer, sizeOfBuffer);
		vertexData->myVertexData = vertexRawData;
		vertexData->myNumberOfVertexes = someData->myVertexCount;
		vertexData->mySize = sizeOfBuffer;
		vertexData->myStride = someData->myVertexStride*sizeof(float);

		data.myVertexData = vertexData;

		data.myInputDescription.Init(4);

		char vertexType = 0;

		for (unsigned short i = 0; i < someData->myLayout.Size(); ++i)
		{
			auto currentLayout = someData->myLayout[i];
			D3D11_INPUT_ELEMENT_DESC* desc = new D3D11_INPUT_ELEMENT_DESC();
			desc->SemanticIndex = 0;
			desc->AlignedByteOffset = currentLayout.myOffset;
			desc->InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			desc->InputSlot = 0;
			desc->InstanceDataStepRate = 0;

			if (currentLayout.myType == ModelData::VERTEX_POS)
			{
				desc->SemanticName = "POSITION";
				desc->Format = DXGI_FORMAT_R32G32B32_FLOAT;
				vertexType |= eVertexVariables::POSITION;
			}
			else if (currentLayout.myType == ModelData::VERTEX_NORMAL)
			{
				desc->SemanticName = "NORMAL";
				desc->Format = DXGI_FORMAT_R32G32B32_FLOAT;
				vertexType |= eVertexVariables::NORMAL;
			}
			else if (currentLayout.myType == ModelData::VERTEX_UV)
			{
				desc->SemanticName = "TEXCOORD";
				desc->Format = DXGI_FORMAT_R32G32_FLOAT;
				vertexType |= eVertexVariables::TEXCOORD;
			}
			else if (currentLayout.myType == ModelData::VERTEX_BINORMAL)
			{
				desc->SemanticName = "BINORMAL";
				desc->Format = DXGI_FORMAT_R32G32B32_FLOAT;
				vertexType |= eVertexVariables::BI_NORMAL;
			}
			else if (currentLayout.myType == ModelData::VERTEX_TANGENT)
			{
				desc->SemanticName = "TANGENT";
				desc->Format = DXGI_FORMAT_R32G32B32_FLOAT;
				vertexType |= eVertexVariables::TANGENT;
			}
			else if (currentLayout.myType == ModelData::VERTEX_SKINWEIGHTS)
			{
				desc->SemanticName = "WEIGHTS";
				desc->Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			}
			else if (currentLayout.myType == ModelData::VERTEX_BONEID)
			{
				desc->SemanticName = "BONES";
				desc->Format = DXGI_FORMAT_R32G32B32A32_SINT;
			}
			data.myInputDescription.Add(desc);
			//delete desc;
		}

		data.myVertexData->SetVertexType(vertexType);

		data.mySurfaces.Init(2);

		Surface surface;
		surface.SetPrimologyType(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		surface.SetIndexCount(someData->myIndexCount);
		surface.SetVertexStart(0);
		surface.SetIndexStart(0);
		surface.SetVertexCount(someData->myVertexCount);

		std::bitset<8> textureBitset = 0;

		for (unsigned int i = 0; i < someData->myTextures.size(); ++i)
		{
			auto& currentTexture = someData->myTextures[i];

			eTextureType textureType = eTextureType::NR_OF_TEXTURETYPES;

			switch (currentTexture.myType)
			{
			case FBXTextureType::DIFFUSE:
				textureType = eTextureType::DIFFUSE;
				break;
			case FBXTextureType::NORMALMAP:
				textureType = eTextureType::NORMALMAP;
				break;
			case FBXTextureType::ROUGHNESS:
				textureType = eTextureType::ROUGHNESS;
				break;
			case FBXTextureType::SUBSTANCE:
				textureType = eTextureType::SUBSTANCE;
				break;
			case FBXTextureType::AO:
				textureType = eTextureType::AO;
				break;
			case FBXTextureType::EMISSIVE:
				textureType = eTextureType::EMISSIVE;
				break;
			}

			if (currentTexture.myFileName.size() != 0)
			{
				surface.SetTexture(myTextureFactory->LoadTexture(currentTexture.myFileName, textureType), textureType);
				if (textureType == eTextureType::DIFFUSE)
				{
					outData->myOriginalDiffuse = myTextureFactory->LoadTexture(currentTexture.myFileName, textureType);
				}
				if (textureType == eTextureType::EMISSIVE)
				{
					outData->myOriginalEmissive = myTextureFactory->LoadTexture(currentTexture.myFileName, textureType);
				}
			}
			else
			{
				std::string printText = "FbxFactory is trying to load a texture without a name from a model.";
				DL_PRINT(printText.c_str());
			}

			textureBitset.at(static_cast<int>(textureType)).flip();
		}

		AddMissingTextures(surface, textureBitset);

		data.mySurfaces.Add(surface);

		outData->InitModel(aDevice, data);

		/*SAFE_DELETE(data.myIndexData);
		SAFE_DELETE(data.myVertexData);*/
	}

	void FBXFactory::CreateModel(COMObjectPointer<ID3D11Device>& aDevice, FbxModelData* someModelData, CU::PoolPointer<Model>& aModel)
	{
		if (someModelData->myData)
		{
			FillData(aDevice, someModelData->myData, aModel);
		}

		FillAnimationData(someModelData, aModel, "Idle");
		
		if (someModelData->myLodGroup)
		{
			aModel->myIsUsingLOD = true;
			aModel->myLodDisplayLevel = someModelData->myLodGroup->myDisplayLevels;
			aModel->myLodThreashholds = someModelData->myLodGroup->myThreashHolds;
		}
		for (unsigned short i = 0; i < someModelData->myChilds.Size(); ++i)
		{
			auto currentChild = someModelData->myChilds[i];
			if (currentChild != nullptr)
			{
				CU::PoolPointer<Model> newModel = GfxFactoryWrapper::GetInstance()->GetModelFactory()->AllocateEmptyChild();
				CreateModel(aDevice, currentChild, newModel);

				aModel->AddChild(newModel);
			}
		}
	}

	void FBXFactory::FillAnimationData(FbxModelData* someModelData, CU::PoolPointer<Model>& aModel, const std::string& aName)
	{
		//FillHierarchyAnimationData(someModelData, aModel);
		FillBoneAnimationData(someModelData, aModel, aName);
	}

	void FBXFactory::FillHierarchyAnimationData(FbxModelData* someModelData, CU::PoolPointer<Model>& aModel)
	{
		if (someModelData->myAnimationCurves != nullptr && someModelData->myAnimation == nullptr)
		{
			TransformationNodeCurves* nodeCurves = new TransformationNodeCurves();

			float startTime = FLT_MAX;
			float stopTime = FLT_MIN;

			for (int i = 0; i < 3; ++i)
			{
				FbxAnimCurve* fbxCurve = someModelData->myAnimationCurves->myRotationCurve[i];

				FbxTimeSpan span;
				fbxCurve->GetTimeInterval(span);

				startTime = fminf(startTime, float(span.GetStart().GetSecondDouble()));
				stopTime = fmaxf(stopTime, float(span.GetStop().GetSecondDouble()));

				int count = fbxCurve->KeyGetCount();

				AnimationCurve* curve = new AnimationCurve();


				for (int j = 0; j < count; ++j)
				{
					GE::KeyFrame frame;
					frame.myTime = float(fbxCurve->KeyGetTime(j).GetSecondDouble());
					frame.myValue = float(fbxCurve->KeyGetValue(j));
					curve->AddKeyFrame(frame);
				}

				curve->FinalizeCurve();
				nodeCurves->SetRotationCurve(i, curve);
			}

			for (int i = 0; i < 3; ++i)
			{
				FbxAnimCurve* fbxCurve = someModelData->myAnimationCurves->myTtranslationCurve[i];

				FbxTimeSpan span;
				fbxCurve->GetTimeInterval(span);

				startTime = fminf(startTime, float(span.GetStart().GetSecondDouble()));
				stopTime = fmaxf(stopTime, float(span.GetStop().GetSecondDouble()));

				int count = fbxCurve->KeyGetCount();

				AnimationCurve* curve = new AnimationCurve();

				for (int j = 0; j < count; ++j)
				{
					GE::KeyFrame frame;
					frame.myTime = float(fbxCurve->KeyGetTime(j).GetSecondDouble());
					frame.myValue = fbxCurve->KeyGetValue(j);
					curve->AddKeyFrame(frame);
				}
				curve->FinalizeCurve();
				nodeCurves->SetTranslationCurve(i, curve);
			}

			//note that number of keyframes is -1, how do we calc/read this value correctly?
			nodeCurves->Init(startTime, stopTime);

			aModel->myTransform = nodeCurves;
		}
	}

	void FBXFactory::FillBoneAnimationData(FbxModelData* someModelData, CU::PoolPointer<Model>& aModel, const std::string& aName)
	{
		if (someModelData->myAnimation != nullptr && someModelData->myAnimation->myBones.size() > 0)
		{
			auto loadedAnimation = someModelData->myAnimation;
			float animationLenght = 0.f;

			Animation* newAnimation = new Animation();

			HierarchyBone rootBone;
			BuildBoneHierarchy(loadedAnimation->myBones[loadedAnimation->myRootBone], loadedAnimation, rootBone);

			int nrOfbones = static_cast<int>(someModelData->myAnimation->myBones.size());
			for (int i = 0; i < nrOfbones; ++i)
			{
				Bone& currentBone = someModelData->myAnimation->myBones[i];

				AnimationNode* newNode = new AnimationNode(currentBone.myFrames.size());

				int nrOfFrames = currentBone.myFrames.size();
				for (int j = 0; j < nrOfFrames; ++j)
				{
					AnimationNodeValue newValue;
					auto currentFrame = currentBone.myFrames[j];

					newValue.myTime = currentFrame.myTime;
					newValue.myMatrix = currentFrame.myMatrix;

					newNode->AddValue(newValue);
				}

				newNode->myBoneName = currentBone.myName;
				newAnimation->AddAnimation(newNode);

				newAnimation->SetBoneMatrix(currentBone.myBaseOrientation, i);
				newAnimation->SetBoneBindPose(currentBone.myBindMatrix, i);
				newAnimation->AddBoneName(currentBone.myName);

				newAnimation->SetBindMatrix(loadedAnimation->myBindMatrix);
			}

			newAnimation->SetHierarchy(rootBone);
			animationLenght = someModelData->myAnimation->myBones[0].myAnimationTime;
			newAnimation->SetAnimationLenght(animationLenght);

			if (aModel->myAnimation == nullptr)
			{
				aModel->myAnimation = new AnimationContainer();
			}

			aModel->myAnimation->AddAnimation(aName, newAnimation);
		}
	}

	void FBXFactory::BuildBoneHierarchy(Bone& aBone, AnimationData* aAnimationData, HierarchyBone& aOutBone)
	{
		aOutBone.myBoneID = aBone.myId;
		aOutBone.myBoneName = aBone.myName;
		const unsigned short nrOfChildren = static_cast<unsigned short>(aBone.myChilds.size());

		if (nrOfChildren > 0)
		{
			aOutBone.myChildren.Init(nrOfChildren);
			for (int i = 0; i < nrOfChildren; ++i)
			{
				HierarchyBone child;
				BuildBoneHierarchy(aAnimationData->myBones[aBone.myChilds[i]], aAnimationData, child);
				aOutBone.myChildren.Add(child);
			}
		}
	}

	void FBXFactory::AddMissingTextures(Surface& aSurface, const std::bitset<8>& aTextureBitset)
	{
		if (aTextureBitset.test(static_cast<int>(eTextureType::DIFFUSE)) == false)
		{
			aSurface.SetTexture(myTextureFactory->GetTexture("White"), eTextureType::DIFFUSE);
		}
		if (aTextureBitset.test(static_cast<int>(eTextureType::AO)) == false)
		{
			aSurface.SetTexture(myTextureFactory->GetTexture("White"), eTextureType::AO);
		}
		if (aTextureBitset.test(static_cast<int>(eTextureType::NORMALMAP)) == false)
		{
			aSurface.SetTexture(myTextureFactory->GetTexture("defaultNormalMap"), eTextureType::NORMALMAP);
		}
		if (aTextureBitset.test(static_cast<int>(eTextureType::SUBSTANCE)) == false)
		{
			aSurface.SetTexture(myTextureFactory->GetTexture("Black"), eTextureType::SUBSTANCE);
		}
		if (aTextureBitset.test(static_cast<int>(eTextureType::ROUGHNESS)) == false)
		{
			aSurface.SetTexture(myTextureFactory->GetTexture("White"), eTextureType::ROUGHNESS);
		}
		if (aTextureBitset.test(static_cast<int>(eTextureType::EMISSIVE)) == false)
		{
			aSurface.SetTexture(myTextureFactory->GetTexture("Black"), eTextureType::EMISSIVE);
		}
	}

	void FBXFactory::LoadModel(COMObjectPointer<ID3D11Device>& aDevice, const char* aFilePath, CU::PoolPointer<Model>& aModel)
	{
		myLoadMutex.lock();
  		FbxModelData* modelData = myLoader->loadModel(aFilePath);
		
		FBXData* data = new FBXData();

		data->myData = modelData;
		data->myPath = aFilePath;
		modelData = data->myData;

		CreateModel(aDevice, modelData, aModel);

		SAFE_DELETE(modelData);
		myLoadMutex.unlock();
	}

	void FBXFactory::LoadAnimation(COMObjectPointer<ID3D11Device>& /*aDevice*/, const char* aFilePath, CU::PoolPointer<Model>& aModel, const std::string& aName)
	{
		myLoadMutex.lock();
		FbxModelData* modelData = myLoader->loadModel(aFilePath);
		
		FBXData* data = new FBXData();

		data->myData = modelData;
		data->myPath = aFilePath;
		modelData = data->myData;

		FillAnimationData(modelData, aModel, aName);
		
		SAFE_DELETE(modelData);
		delete data;
		myLoadMutex.unlock();
	}
}

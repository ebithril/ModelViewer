#include "Engine_Precompiled.h"
#include "Model.h"
#include "DirectXEngine.h"
#include "Effect.h"
#include "Engine.h"
#include "d3dx11effect.h"
#include "Camera.h"
#include "VertexDataWrapper.h"
#include "VertexIndexWrapper.h"
#include "ConstantBuffers.h"
#include "Surface.h"
#include "ModelDataStruct.h"
#include "GfxFactoryWrapper.h"
#include "TextureFactory.h"
#include "ModelFactory.h"
#include "EffectFactory.h"
#include "AnimationContainer.h"
#include "LightBuffer.h"

#include "../Common Utilities/EventManager.h"
#include "../Common Utilities/RenderCommandInstance.h"

#define SKYBOXSIZE 50

namespace GraphicsEngine
{
	Model::Model()
	{
		myTotalTime = 0;
		myChilds.Init(4);
		mySurfaces.Init(2);
		myLodThreashholds.Init(4);
		myChildTransforms.Init(4);
		myIsLoaded = false;
		myIsUsingLOD = false;
		myLodDisplayLevel = 0;
		myEffect = nullptr;
		myUpdateDynamic = false;
		myOriginalDiffuse = nullptr;
		myOriginalEmissive = nullptr;
		myTransform = nullptr;
		myAnimation = nullptr;
		myIsLoading = false;
	}

	Model::Model(const std::string& aModelFilename) : Model()
	{
		myModelFile = aModelFilename;
		myAnimation = nullptr;
		myIsLoading = false;
	}

	Model::~Model()
	{
		UnLoad();
	}

	Texture* Model::GetOriginalDiffuseTexture()
	{
		if (myOriginalDiffuse != nullptr)
		{
			return myOriginalDiffuse;
		}

		for (unsigned short i = 0; i < myChilds.Size(); i++)
		{
			Texture* texture = myChilds[i]->GetOriginalDiffuseTexture();

			if (texture != nullptr)
			{
				return texture;
			}
		}

		return nullptr;
	}

	Texture* Model::GetOriginalEmissiveTexture()
	{
		if (myOriginalEmissive != nullptr)
		{
			return myOriginalEmissive;
		}

		for (unsigned short i = 0; i < myChilds.Size(); i++)
		{
			Texture* texture = myChilds[i]->GetOriginalEmissiveTexture();

			if (texture != nullptr)
			{
				return texture;
			}
		}

		return nullptr;
	}

	void Model::UnLoad()
	{
		if (myIsLoaded == true)
		{
			for (unsigned short i = 0; i < myChilds.Size(); i++)
			{
				myChilds[i]->UnLoad();
				myChilds[i] = nullptr;
			}
			myChilds.RemoveAll();
			myLodThreashholds.RemoveAll();
			mySurfaces.RemoveAll();

			myVertexBuffer.UnLoad();
			myIndexBuffer.UnLoad();

			myIsLoaded = false;
		}
	}

	Effect* Model::GetEffect()
	{
		return myEffect;
	}

	void Model::SetEffect(Effect* anEffect)
	{
		myEffect = anEffect;

		if (myIsLoaded == true)
		{
			for (unsigned short i = 0; i < mySurfaces.Size(); i++)
			{
				mySurfaces[i].SetEffect(anEffect);
			}

			for (unsigned short i = 0; i < myChilds.Size(); i++)
			{
				myChilds[i]->SetEffect(anEffect);
			}
		}
	}

	bool Model::IsLoaded() const
	{
		return myIsLoaded;
	}

	bool Model::AnimationIsLoaded(const std::string& anAnimationName) const
	{
		if (myAnimation == nullptr)
		{
			return false;
		}
		else
		{
			return myAnimation->CheckIfExist(anAnimationName);
		}
	}

	const std::string& Model::GetFileName()
	{
		return myModelFile;
	}

	void Model::InitFromBufferData(VertexDataWrapper& someVertexData, VertexIndexWrapper& someIndexData, const D3D_PRIMITIVE_TOPOLOGY& aTopology)
	{
		myChilds.RemoveAll();
		mySurfaces.RemoveAll();

		Surface surface;
		surface.SetIndexCount(someIndexData.myNumberOfIndexes);
		surface.SetIndexStart(0);
		surface.SetVertexCount(someVertexData.myNumberOfVertexes);
		surface.SetVertexStart(0);
		surface.SetPrimologyType(aTopology);
		mySurfaces.Add(surface);

		myVertexBuffer.Create(Engine::GetInstance()->GetEngine()->GetDevice(), &someVertexData);
		myIndexBuffer.Create(Engine::GetInstance()->GetEngine()->GetDevice(), &someIndexData);

		myIsLoaded = true;
	}

	void Model::InitSkybox(const std::string& aSkyboxName)
	{
		myIsLoaded = true;
		myChilds.RemoveAll();
		mySurfaces.RemoveAll();

		CU::PoolPointer<Model> newModel = GfxFactoryWrapper::GetInstance()->GetModelFactory()->AllocateEmptyChild();
		newModel->InitSkyBoxChild(aSkyboxName, eSkyboxSide::eBOTTOM);
		myChilds.Add(newModel);

		newModel = GfxFactoryWrapper::GetInstance()->GetModelFactory()->AllocateEmptyChild();
		newModel->InitSkyBoxChild(aSkyboxName, eSkyboxSide::eTOP);
		myChilds.Add(newModel);

		newModel = GfxFactoryWrapper::GetInstance()->GetModelFactory()->AllocateEmptyChild();
		newModel->InitSkyBoxChild(aSkyboxName, eSkyboxSide::eRIGHT);
		myChilds.Add(newModel);

		newModel = GfxFactoryWrapper::GetInstance()->GetModelFactory()->AllocateEmptyChild();
		newModel->InitSkyBoxChild(aSkyboxName, eSkyboxSide::eLEFT);
		myChilds.Add(newModel);

		newModel = GfxFactoryWrapper::GetInstance()->GetModelFactory()->AllocateEmptyChild();
		newModel->InitSkyBoxChild(aSkyboxName, eSkyboxSide::eBACK);
		myChilds.Add(newModel);

		newModel = GfxFactoryWrapper::GetInstance()->GetModelFactory()->AllocateEmptyChild();
		newModel->InitSkyBoxChild(aSkyboxName, eSkyboxSide::eFORWARD);
		myChilds.Add(newModel);
	}

	void Model::InitSkyBoxChild(const std::string& aTextureName, eSkyboxSide aSkyBoxSide)
	{
		myIsLoaded = true;

		VertexPosNormalUVBiNormalTan* data = new VertexPosNormalUVBiNormalTan[4];
		VertexDataWrapper* vertexData = new VertexDataWrapper();
		vertexData->myNumberOfVertexes = 4;
		vertexData->mySize = sizeof(VertexPosNormalUVBiNormalTan) * vertexData->myNumberOfVertexes;
		vertexData->myVertexData = reinterpret_cast<char*>(data);
		vertexData->myStride = sizeof(VertexPosNormalUVBiNormalTan);
		vertexData->myType = eVertexType::VERTEX_POS_NORM_UV_BI_NORMAL_TAN;

		for (int i = 0; i < 4; i++)
		{
			data[i].myBinormal = { 0, 0, 0 };
			data[i].myTangent = { 0, 0, 0 };
			data[i].myNormal = { 0, 0, 0 };
		}

		data[0].myUV = { 0, 0 };
		data[1].myUV = { 1, 0 };
		data[2].myUV = { 0, 1 };
		data[3].myUV = { 1, 1 };

		std::string textureName = "";

		switch (aSkyBoxSide)
		{
		case eSkyboxSide::eBOTTOM:
			data[0].myPosition = { -SKYBOXSIZE, -SKYBOXSIZE, SKYBOXSIZE };
			data[1].myPosition = { SKYBOXSIZE, -SKYBOXSIZE, SKYBOXSIZE };
			data[2].myPosition = { -SKYBOXSIZE, -SKYBOXSIZE, -SKYBOXSIZE };
			data[3].myPosition = { SKYBOXSIZE, -SKYBOXSIZE, -SKYBOXSIZE };

			textureName = aTextureName + "_bottom";
			break;
		case eSkyboxSide::eTOP:
			data[0].myPosition = { -SKYBOXSIZE, SKYBOXSIZE, -SKYBOXSIZE };
			data[1].myPosition = { SKYBOXSIZE, SKYBOXSIZE, -SKYBOXSIZE };
			data[2].myPosition = { -SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE };
			data[3].myPosition = { SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE };

			textureName = aTextureName + "_top";
			break;
		case eSkyboxSide::eRIGHT:
			data[0].myPosition = { SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE };
			data[1].myPosition = { SKYBOXSIZE, SKYBOXSIZE, -SKYBOXSIZE };
			data[2].myPosition = { SKYBOXSIZE, -SKYBOXSIZE, SKYBOXSIZE };
			data[3].myPosition = { SKYBOXSIZE, -SKYBOXSIZE, -SKYBOXSIZE };

			textureName = aTextureName + "_right";
			break;
		case eSkyboxSide::eLEFT:
			data[0].myPosition = { -SKYBOXSIZE, SKYBOXSIZE, -SKYBOXSIZE };
			data[1].myPosition = { -SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE };
			data[2].myPosition = { -SKYBOXSIZE, -SKYBOXSIZE, -SKYBOXSIZE };
			data[3].myPosition = { -SKYBOXSIZE, -SKYBOXSIZE, SKYBOXSIZE };

			textureName = aTextureName + "_left";
			break;
		case eSkyboxSide::eFORWARD:
			data[0].myPosition = { -SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE };
			data[1].myPosition = { SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE };
			data[2].myPosition = { -SKYBOXSIZE, -SKYBOXSIZE, SKYBOXSIZE };
			data[3].myPosition = { SKYBOXSIZE, -SKYBOXSIZE, SKYBOXSIZE };

			textureName = aTextureName + "_front";
			break;
		case eSkyboxSide::eBACK:
			data[0].myPosition = { SKYBOXSIZE, SKYBOXSIZE, -SKYBOXSIZE };
			data[1].myPosition = { -SKYBOXSIZE, SKYBOXSIZE, -SKYBOXSIZE };
			data[2].myPosition = { SKYBOXSIZE, -SKYBOXSIZE, -SKYBOXSIZE };
			data[3].myPosition = { -SKYBOXSIZE, -SKYBOXSIZE, -SKYBOXSIZE };

			textureName = aTextureName + "_back";
			break;
		default:
			assert(false && "Something broke in InitSkyBoxChild");
			break;
		}

		Surface surface;
		surface.SetIndexCount(6);
		surface.SetIndexStart(0);
		surface.SetVertexCount(4);
		surface.SetVertexStart(0);
		surface.SetPrimologyType(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		surface.SetTexture(GfxFactoryWrapper::GetInstance()->GetTextureFactory()->GetTexture(textureName), eTextureType::DIFFUSE);
		surface.SetEffect(GfxFactoryWrapper::GetInstance()->GetEffectFactory()->GetEffect("SkyBoxShader"));
		mySurfaces.Add(surface);

		VertexIndexWrapper* indexData = new VertexIndexWrapper;
		indexData->myFormat = DXGI_FORMAT_R32_UINT;
		indexData->myNumberOfIndexes = 6;
		indexData->mySize = sizeof(unsigned int) * indexData->myNumberOfIndexes;
		unsigned int* indexies = new unsigned int[6];
		indexData->myIndexData = reinterpret_cast<char*>(indexies);

		indexies[0] = 0;
		indexies[1] = 1;
		indexies[2] = 2;
		indexies[3] = 1;
		indexies[4] = 3;
		indexies[5] = 2;

		myVertexBuffer.Create(Engine::GetInstance()->GetEngine()->GetDevice(), vertexData);
		myIndexBuffer.Create(Engine::GetInstance()->GetEngine()->GetDevice(), indexData);

		delete vertexData;
		delete indexData;
	}

	void Model::SetDiffuseTexture(Texture* aTexture)
	{
		for (unsigned short i = 0; i < mySurfaces.Size(); i++)
		{
			mySurfaces[i].SetTexture(aTexture, eTextureType::DIFFUSE);
		}

		for (unsigned short i = 0; i < myChilds.Size(); i++)
		{
			myChilds[i]->SetDiffuseTexture(aTexture);
		}
	}

	void Model::SetEmissiveTexture(Texture* aTexture)
	{
		for (unsigned short i = 0; i < mySurfaces.Size(); i++)
		{
			mySurfaces[i].SetTexture(aTexture, eTextureType::EMISSIVE);
		}

		for (unsigned short i = 0; i < myChilds.Size(); i++)
		{
			myChilds[i]->SetEmissiveTexture(aTexture);
		}
	}

	void Model::InitModel(COMObjectPointer<ID3D11Device>& aDevice, ModelDataStruct& aModelData)
	{
		mySurfaces = aModelData.mySurfaces;
		InitBuffers(aDevice, *aModelData.myVertexData, *aModelData.myIndexData);
		myIsLoaded = true;
	}

	void Model::InitPolygon(COMObjectPointer<ID3D11Device>& aDevice, Effect* anEffect, const float aSize)
	{
		myEffect = anEffect;

		const float advancedNumber = 0.25f;
		const float ySize = aSize * advancedNumber;
		const float xSize = aSize * 0.5f;
		VertexPosCol vertices[] =
		{
			{ { 0.f, ySize, 0.f }, { 0.0f, 1.0f, 0.0f, 0.2f } },
			{ { xSize, -ySize, 0.f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
			{ { -xSize, -ySize, 0.f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
		};

		WORD indicies[] =
		{
			// front face
			0, 1, 2,
			// back face
			0, 2, 1,
		};

		VertexDataWrapper vertexData;
		vertexData.myType = eVertexType::VERTEX_POS_COL;
		vertexData.myNumberOfVertexes = ARRAYSIZE(vertices);
		vertexData.mySize = vertexData.myNumberOfVertexes * sizeof(VertexPosCol);
		vertexData.myStride = sizeof(VertexPosCol);
		vertexData.myVertexData = new char[vertexData.mySize];
		memcpy(vertexData.myVertexData.get(), vertices, vertexData.mySize);


		VertexIndexWrapper indexData;
		indexData.myNumberOfIndexes = ARRAYSIZE(indicies);
		indexData.mySize = sizeof(WORD) * indexData.myNumberOfIndexes;
		indexData.myFormat = DXGI_FORMAT_R16_UINT;
		indexData.myIndexData = new char[indexData.mySize];
		memcpy(indexData.myIndexData.get(), indicies, indexData.mySize);


		InitBuffers(aDevice, vertexData, indexData);
		myIsLoaded = true;
	}

	void Model::InitCube(COMObjectPointer<ID3D11Device>& aDevice, Effect* anEffect, const float aLength)
	{
		myEffect = anEffect;

		VertexPosCol vertices[] =
		{
			// Front Face
			{ { -aLength, -aLength, -aLength }, { 0.0f, 1.0f, 0.0f, 1.0f } },
			{ { -aLength, aLength, -aLength }, { 0.0f, 0.0f, 1.0f, 1.0f } },
			{ { aLength, aLength, -aLength }, { 1.0f, 0.0f, 0.0f, 1.0f } },
			{ { aLength, -aLength, -aLength }, { 1.0f, 1.0f, 0.0f, 1.0f } },

			// Back Face
			{ { -aLength, -aLength, aLength }, { 1.0f, 1.0f, 0.0f, 1.0f } },
			{ { -aLength, aLength, aLength }, { 0.0f, 1.0f, 0.0f, 1.0f } },
			{ { aLength, aLength, aLength }, { 0.0f, 0.0f, 1.0f, 1.0f } },
			{ { aLength, -aLength, aLength }, { 1.0f, 0.0f, 0.0f, 1.0f } },

		};

		WORD indicies[] =
		{
			// front face
			0, 1, 2,
			0, 2, 3,

			// back face
			4, 6, 5,
			4, 7, 6,

			// left face
			4, 5, 1,
			4, 1, 0,

			// right face
			3, 2, 6,
			3, 6, 7,

			// top face
			1, 5, 6,
			1, 6, 2,

			// bottom face
			4, 0, 3,
			4, 3, 7

		};

		VertexDataWrapper vertexData;
		vertexData.myType = eVertexType::VERTEX_POS_COL;
		vertexData.myNumberOfVertexes = ARRAYSIZE(vertices);
		vertexData.mySize = vertexData.myNumberOfVertexes * sizeof(VertexPosCol);
		vertexData.myStride = sizeof(VertexPosCol);
		vertexData.myVertexData = new char[vertexData.mySize];
		memcpy(vertexData.myVertexData.get(), vertices, vertexData.mySize);

		VertexIndexWrapper indexData;
		indexData.myNumberOfIndexes = ARRAYSIZE(indicies);
		indexData.mySize = sizeof(WORD) * indexData.myNumberOfIndexes;
		indexData.myFormat = DXGI_FORMAT_R16_UINT;
		indexData.myIndexData = new char[indexData.mySize];
		memcpy(indexData.myIndexData.get(), indicies, indexData.mySize);

		InitBuffers(aDevice, vertexData, indexData);
		myIsLoaded = true;
	}

	void Model::InitNormCube(COMObjectPointer<ID3D11Device>& aDevice, Effect* anEffect, const float aSize, Texture* aTexture)
	{
		myEffect = anEffect;

		VertexPosNormalUVBiNormalTan vertices[] =
		{

			// Front Face
			{ { -aSize, -aSize, -aSize }, { 0.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f } },
			{ { -aSize, aSize, -aSize }, { 0.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f } },
			{ { aSize, aSize, -aSize }, { 0.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f } },
			{ { aSize, -aSize, -aSize }, { 0.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f } },

			// Back Face
			{ { -aSize, -aSize, aSize }, { 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f } },
			{ { aSize, -aSize, aSize }, { 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f } },
			{ { aSize, aSize, aSize }, { 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f } },
			{ { -aSize, aSize, aSize }, { 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f } },

			// Top Face
			{ { -aSize, aSize, -aSize }, { 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f } },
			{ { -aSize, aSize, aSize }, { 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f } },
			{ { aSize, aSize, aSize }, { 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f } },
			{ { aSize, aSize, -aSize }, { 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f } },

			// Bottom Face
			{ { -aSize, -aSize, -aSize }, { 0.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f } },
			{ { aSize, -aSize, -aSize }, { 0.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f } },
			{ { aSize, -aSize, aSize }, { 0.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f } },
			{ { -aSize, -aSize, aSize }, { 0.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f } },

			// Left Face
			{ { -aSize, -aSize, aSize }, { 0.0f }, { -1.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f } },
			{ { -aSize, aSize, aSize }, { 0.0f }, { -1.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f } },
			{ { -aSize, aSize, -aSize }, { 0.0f }, { -1.0f, 0.0f, 0.0f }, { 0.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f } },
			{ { -aSize, -aSize, -aSize }, { 0.0f }, { -1.0f, 0.0f, 0.0f }, { 0.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f } },


			// Right Face
			{ { aSize, -aSize, -aSize }, { 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f } },
			{ { aSize, aSize, -aSize }, { 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f } },
			{ { aSize, aSize, aSize }, { 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f } },
			{ { aSize, -aSize, aSize }, { 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f } },

		};

		WORD indicies[] =
		{
			// Front Face
			0, 1, 2,
			0, 2, 3,

			// Back Face
			4, 5, 6,
			4, 6, 7,

			// Top Face
			8, 9, 10,
			8, 10, 11,

			// Bottom Face
			12, 13, 14,
			12, 14, 15,

			// Left Face
			16, 17, 18,
			16, 18, 19,

			// Right Face
			20, 21, 22,
			20, 22, 23
		};

		for (unsigned int i = 0; i < ARRAYSIZE(indicies); i += 3)
		{
			Vector3<float> normal = GetPlaneNormal(vertices[indicies[i]].myPosition, vertices[indicies[i + 1]].myPosition, vertices[indicies[i + 2]].myPosition);

			for (unsigned int j = 0; j < 3; j++)
			{
				vertices[indicies[i + j]].myNormal = normal;
			}
		}

		VertexDataWrapper vertexData;
		vertexData.myType = eVertexType::VERTEX_POS_NORM_UV_BI_NORMAL_TAN;
		vertexData.myNumberOfVertexes = ARRAYSIZE(vertices);
		vertexData.mySize = vertexData.myNumberOfVertexes * sizeof(VertexPosNormalUVBiNormalTan);
		vertexData.myStride = sizeof(VertexPosNormalUVBiNormalTan);
		vertexData.myVertexData = new char[vertexData.mySize];
		memcpy(vertexData.myVertexData.get(), vertices, vertexData.mySize);

		VertexIndexWrapper indexData;
		indexData.myNumberOfIndexes = ARRAYSIZE(indicies);
		indexData.mySize = sizeof(WORD) * indexData.myNumberOfIndexes;
		indexData.myFormat = DXGI_FORMAT_R16_UINT;
		indexData.myIndexData = new char[indexData.mySize];
		memcpy(indexData.myIndexData.get(), indicies, indexData.mySize);

		InitBuffers(aDevice, vertexData, indexData);

		Surface newSurface;

		newSurface.SetPrimologyType(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		newSurface.SetIndexCount(indexData.myNumberOfIndexes);
		newSurface.SetVertexStart(0);
		newSurface.SetIndexStart(0);
		newSurface.SetVertexCount(vertexData.myNumberOfVertexes);

		newSurface.SetTexture(aTexture, eTextureType::DIFFUSE);
		newSurface.SetTexture(aTexture, eTextureType::AO);
		newSurface.SetTexture(aTexture, eTextureType::NORMALMAP);
		newSurface.SetTexture(aTexture, eTextureType::ROUGHNESS);
		newSurface.SetTexture(aTexture, eTextureType::SUBSTANCE);

		newSurface.SetEffect(anEffect);

		mySurfaces.Init(2);
		mySurfaces.Add(newSurface);
		myIsLoaded = true;

	}

	void Model::InitNormCubeVideo(COMObjectPointer<ID3D11Device>& aDevice, Effect* anEffect, const float aSize, Texture* aTexture)
	{
		myEffect = anEffect;

		VertexPosNormalUVBiNormalTan vertices[] =
		{

			// Front Face
			{ { -aSize, -aSize, -aSize }, { 0.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f } },
			{ { -aSize, aSize, -aSize }, { 0.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f } },
			{ { aSize, aSize, -aSize }, { 0.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f } },
			{ { aSize, -aSize, -aSize }, { 0.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f } },

			// Back Face
			{ { -aSize, -aSize, aSize }, { 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f } },
			{ { aSize, -aSize, aSize }, { 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f } },
			{ { aSize, aSize, aSize }, { 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f } },
			{ { -aSize, aSize, aSize }, { 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f } },

			// Top Face
			{ { -aSize, aSize, -aSize }, { 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f } },
			{ { -aSize, aSize, aSize }, { 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f } },
			{ { aSize, aSize, aSize }, { 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f } },
			{ { aSize, aSize, -aSize }, { 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f } },

			// Bottom Face
			{ { -aSize, -aSize, -aSize }, { 0.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f } },
			{ { aSize, -aSize, -aSize }, { 0.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f } },
			{ { aSize, -aSize, aSize }, { 0.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f } },
			{ { -aSize, -aSize, aSize }, { 0.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f } },

			// Left Face
			{ { -aSize, -aSize, aSize }, { 0.0f }, { -1.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f } },
			{ { -aSize, aSize, aSize }, { 0.0f }, { -1.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f } },
			{ { -aSize, aSize, -aSize }, { 0.0f }, { -1.0f, 0.0f, 0.0f }, { 0.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f } },
			{ { -aSize, -aSize, -aSize }, { 0.0f }, { -1.0f, 0.0f, 0.0f }, { 0.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f } },


			// Right Face
			{ { aSize, -aSize, -aSize }, { 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f } },
			{ { aSize, aSize, -aSize }, { 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f } },
			{ { aSize, aSize, aSize }, { 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f } },
			{ { aSize, -aSize, aSize }, { 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f } },

		};

		WORD indicies[] =
		{
			// Front Face
			0, 1, 2,
			0, 2, 3,

			// Back Face
			4, 5, 6,
			4, 6, 7,

			// Top Face
			8, 9, 10,
			8, 10, 11,

			// Bottom Face
			12, 13, 14,
			12, 14, 15,

			// Left Face
			16, 17, 18,
			16, 18, 19,

			// Right Face
			20, 21, 22,
			20, 22, 23
		};

		for (unsigned int i = 0; i < ARRAYSIZE(indicies); i += 3)
		{
			Vector3<float> normal = GetPlaneNormal(vertices[indicies[i]].myPosition, vertices[indicies[i + 1]].myPosition, vertices[indicies[i + 2]].myPosition);

			for (unsigned int j = 0; j < 3; j++)
			{
				vertices[indicies[i + j]].myNormal = normal;
			}
		}

		VertexDataWrapper vertexData;
		vertexData.myType = eVertexType::VERTEX_POS_NORM_UV_BI_NORMAL_TAN;
		vertexData.myNumberOfVertexes = ARRAYSIZE(vertices);
		vertexData.mySize = vertexData.myNumberOfVertexes * sizeof(VertexPosNormalUVBiNormalTan);
		vertexData.myStride = sizeof(VertexPosNormalUVBiNormalTan);
		vertexData.myVertexData = new char[vertexData.mySize];
		memcpy(vertexData.myVertexData.get(), vertices, vertexData.mySize);

		VertexIndexWrapper indexData;
		indexData.myNumberOfIndexes = ARRAYSIZE(indicies);
		indexData.mySize = sizeof(WORD) * indexData.myNumberOfIndexes;
		indexData.myFormat = DXGI_FORMAT_R16_UINT;
		indexData.myIndexData = new char[indexData.mySize];
		memcpy(indexData.myIndexData.get(), indicies, indexData.mySize);

		InitBuffers(aDevice, vertexData, indexData);

		Surface newSurface;

		newSurface.SetPrimologyType(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		newSurface.SetIndexCount(indexData.myNumberOfIndexes);
		newSurface.SetVertexStart(0);
		newSurface.SetIndexStart(0);
		newSurface.SetVertexCount(vertexData.myNumberOfVertexes);

		newSurface.SetTexture(aTexture, eTextureType::DIFFUSE);
		newSurface.SetTexture(aTexture, eTextureType::AO);
		newSurface.SetTexture(aTexture, eTextureType::NORMALMAP);
		newSurface.SetTexture(aTexture, eTextureType::ROUGHNESS);
		newSurface.SetTexture(aTexture, eTextureType::SUBSTANCE);

		newSurface.SetEffect(anEffect);

		mySurfaces.Init(2);
		mySurfaces.Add(newSurface);
		myIsLoaded = true;
	}

	void Model::InitSkyBox(COMObjectPointer<ID3D11Device>& aDevice, Effect* anEffect, const float aSize, Texture* aTexture)
	{
		myEffect = anEffect;

		VertexPosNormUV vertices[] =
		{

			// Front Face
			{ { -aSize, -aSize, -aSize }, { 0.0f, 1.0f }, { 0.0f, 0.0f, -1.0f } },
			{ { -aSize, aSize, -aSize }, { 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f } },
			{ { aSize, aSize, -aSize }, { 1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f } },
			{ { aSize, -aSize, -aSize }, { 1.0f, 1.0f }, { 0.0f, 0.0f, -1.0f } },

			// Back Face
			{ { -aSize, -aSize, aSize }, { 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f } },
			{ { aSize, -aSize, aSize }, { 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f } },
			{ { aSize, aSize, aSize }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
			{ { -aSize, aSize, aSize }, { 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } },

			// Top Face
			{ { -aSize, aSize, -aSize }, { 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f } },
			{ { -aSize, aSize, aSize }, { 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
			{ { aSize, aSize, aSize }, { 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
			{ { aSize, aSize, -aSize }, { 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f } },

			// Bottom Face
			{ { -aSize, -aSize, -aSize }, { 1.0f, 1.0f }, { 0.0f, -1.0f, 0.0f } },
			{ { aSize, -aSize, -aSize }, { 0.0f, 1.0f }, { 0.0f, -1.0f, 0.0f } },
			{ { aSize, -aSize, aSize }, { 0.0f, 0.0f }, { 0.0f, -1.0f, 0.0f } },
			{ { -aSize, -aSize, aSize }, { 1.0f, 0.0f }, { 0.0f, -1.0f, 0.0f } },

			// Left Face
			{ { -aSize, -aSize, aSize }, { 0.0f, 1.0f }, { -1.0f, 0.0f, 0.0f } },
			{ { -aSize, aSize, aSize }, { 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f } },
			{ { -aSize, aSize, -aSize }, { 1.0f, 0.0f }, { -1.0f, 0.0f, 0.0f } },
			{ { -aSize, -aSize, -aSize }, { 1.0f, 1.0f }, { -1.0f, 0.0f, 0.0f } },

			// Right Face
			{ { aSize, -aSize, -aSize }, { 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f } },
			{ { aSize, aSize, -aSize }, { 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } },
			{ { aSize, aSize, aSize }, { 1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } },
			{ { aSize, -aSize, aSize }, { 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f } },

		};

		WORD indicies[] =
		{
			// Front Face
			2, 1, 0,
			3, 2, 0,

			// Back Face
			6, 5, 4,
			7, 6, 4,

			// Top Face
			10, 9, 8,
			11, 10, 8,

			// Bottom Face
			14, 13, 12,
			15, 14, 12,

			// Left Face
			18, 17, 16,
			19, 18, 16,

			// Right Face
			22, 21, 20,
			23, 22, 20
		};

		for (unsigned int i = 0; i < ARRAYSIZE(indicies); i += 3)
		{
			Vector3<float> normal = GetPlaneNormal(vertices[indicies[i]].myPosition, vertices[indicies[i + 1]].myPosition, vertices[indicies[i + 2]].myPosition);
			for (unsigned int j = 0; j < 3; j++)
			{
				vertices[indicies[i + j]].myNormal = normal;
			}
		}

		VertexDataWrapper vertexData;
		vertexData.myType = eVertexType::VERTEX_POS_COL;
		vertexData.myNumberOfVertexes = ARRAYSIZE(vertices);
		vertexData.mySize = vertexData.myNumberOfVertexes * sizeof(VertexPosNormUV);
		vertexData.myStride = sizeof(VertexPosNormUV);
		vertexData.myVertexData = new char[vertexData.mySize];
		memcpy(vertexData.myVertexData.get(), vertices, vertexData.mySize);

		VertexIndexWrapper indexData;
		indexData.myNumberOfIndexes = ARRAYSIZE(indicies);
		indexData.mySize = sizeof(WORD) * indexData.myNumberOfIndexes;
		indexData.myFormat = DXGI_FORMAT_R16_UINT;
		indexData.myIndexData = new char[indexData.mySize];
		memcpy(indexData.myIndexData.get(), indicies, indexData.mySize);

		InitBuffers(aDevice, vertexData, indexData);

		Surface newSurface;

		newSurface.SetPrimologyType(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		newSurface.SetIndexCount(indexData.myNumberOfIndexes);
		newSurface.SetVertexStart(0);
		newSurface.SetIndexStart(0);
		newSurface.SetVertexCount(vertexData.myNumberOfVertexes);

		TextureFactory* aTextureFactory = GfxFactoryWrapper::GetInstance()->GetTextureFactory();

		newSurface.SetTexture(aTexture, eTextureType::DIFFUSE);
		newSurface.SetTexture(aTextureFactory->GetTexture("White"), eTextureType::AO);
		newSurface.SetTexture(aTextureFactory->GetTexture("White"), eTextureType::NORMALMAP);
		newSurface.SetTexture(aTextureFactory->GetTexture("White"), eTextureType::ROUGHNESS);
		newSurface.SetTexture(aTextureFactory->GetTexture("White"), eTextureType::SUBSTANCE);

		newSurface.SetEffect(anEffect);

		mySurfaces.Init(2);
		mySurfaces.Add(newSurface);
		myIsLoaded = true;

	}

	void Model::SetUpdateDynamic(const bool aFlag)
	{
		myUpdateDynamic = aFlag;
		for (unsigned short i = 0; i < myChilds.Size(); i++)
		{
			myChilds[i]->SetUpdateDynamic(aFlag);
		}
	}

	void Model::SetShaderVariable(const std::string& aName, float aValue){

		ID3DX11EffectScalarVariable* var = myEffect->GetEffect()->GetVariableByName(aName.c_str())->AsScalar();
		if (var->IsValid() == false)
		{
			DL_ASSERT("Effect Scalar variable not valid");
		}
		else
		{
			var->SetFloat(aValue);
		}
		var->Release();
	}

	void Model::AddChild(CU::PoolPointer<Model>& aModel)
	{
		myIsLoaded = true;
		myChilds.Add(aModel);
		myChildTransforms.Add(nullptr);
	}

	void Model::Render(SceneRenderData& someData, CU::RenderCommandInstance& aRenderCommand, const Matrix44f& anInstance, const bool anObjectsFirstRender)
	{
		anObjectsFirstRender;
		if (myIsLoaded == true)
		{
			if (mySurfaces.Size() != 0 && myEffect != nullptr)
			{
				COMObjectPointer<ID3D11DeviceContext> context = Engine::GetInstance()->GetEngine()->GetContext();

				context->IASetInputLayout(myEffect->GetInputLayout().Get());
				context->IASetVertexBuffers(0, 1, &myVertexBuffer.myVertexBuffer, &myVertexBuffer.myStride, &myVertexBuffer.myByteOffset);
				context->IASetIndexBuffer(myIndexBuffer.myIndexBuffer.Get(), myIndexBuffer.myIndexBufferFormat, myIndexBuffer.myByteOffset);

				if (myEffect->NeedNewFrameUpdate() == true)
				{

					myTotalTime += CU::EventManager::GetInstance()->GetDeltaTime();

					PerFrameCameraBuffer cameraBuffer;
					cameraBuffer.myCameraPosition = someData.myCamera.GetPosition();
					cameraBuffer.myTime = myTotalTime;
					cameraBuffer.myViewMatrix = someData.myCamera.GetInverse();
					cameraBuffer.myViewDirection = someData.myCamera.GetOrientation().GetForwardVector();

					const LightShaderData& shaderData = someData.myLightBuffer->GetDirectionalLight();

					PerFrameLightBuffer frameLightBuffer;
					frameLightBuffer.myLightColor = { shaderData.myColor.myX, shaderData.myColor.myY, shaderData.myColor.myZ, 1 };
					frameLightBuffer.myLightDirection = shaderData.myDirection;
					frameLightBuffer.myLightDirectionShadowViewProjection = someData.myLightBuffer->GetReadDirectionalLightShadowBuffer().GetViewProjection();
					
					myEffect->PerFrameUpdate(cameraBuffer, frameLightBuffer);

					if (myEffect->NeedNewOnResizeUpdate() == true)
					{
						OnResizeBuffer resizeBuffer;
						resizeBuffer.myProjectionMatrix = someData.myCamera.GetProjection();
						myEffect->OnResizeUpdate(resizeBuffer);
					}

				}

				if (/*anObjectsFirstRender == false*/true)
				{
					PerObjectLightBuffer lightData;
					ZeroMemory(&lightData, sizeof(lightData));

					for (unsigned short i = 0; i < aRenderCommand.myLightIndexes.Size() && i < GRAPHICS_ENGINE_NUMBER_OF_SPOT_AND_POINT_LIGHTS; i++)
					{
						lightData.myData[i] = someData.myLightBuffer->GetLightWithIndex(aRenderCommand.myLightIndexes[i]);
					}
					myEffect->PerObjectLightBufferUpdate(lightData);
				}

				PerObjectDataBuffer objectBuffer;
				Matrix44f viewProjection = someData.myCamera.GetInverse() * someData.myCamera.GetProjection();
				objectBuffer.myDebugColor = { 1, 1, 1, 1 };

				objectBuffer.myToWorldMatrix = anInstance;
				objectBuffer.myWorldViewProjectionMatrix = anInstance * viewProjection;
				objectBuffer.myLastFrameToWorldMatrix = anInstance;
				objectBuffer.myLastFrameWorldViewProjectionMatrix = anInstance* viewProjection;
				objectBuffer.myCutOffValue = Vector4<float>(aRenderCommand.myCutOffValue.x, aRenderCommand.myCutOffValue.y, aRenderCommand.myCutOffValue.x, aRenderCommand.myCutOffValue.y);
				myEffect->PerObjectUpdate(objectBuffer);


				for (unsigned short i = 0; i < mySurfaces.Size(); i++)
				{
					mySurfaces[i].Activate();
					Engine::GetInstance()->GetEngine()->GetContext()->IASetPrimitiveTopology(mySurfaces[i].GetPrimologyType());

					D3DX11_TECHNIQUE_DESC techDesc;

					COMObjectPointer<ID3DX11EffectTechnique> tech = myEffect->GetTechnique();
					tech->GetDesc(&techDesc);

					for (UINT p = 0; p < techDesc.Passes; ++p)
					{
						COMObjectPointer<ID3DX11EffectPass> pass = myEffect->GetTechnique()->GetPassByIndex(p);

						if (pass->IsValid() == false)
						{
							DL_ASSERT("Pass invalid");
						}

						HRESULT passResult = pass->Apply(NULL, context.Get());
						if (FAILED(passResult))
						{
							DL_ASSERT("Pass failed.");
						}

						context->DrawIndexed(myIndexBuffer.myIndexCount, 0, 0);
					}
				}
			}
		}
	}

	void Model::RenderShadowShaderData(Effect* aShadowEffect, const Matrix44f& anInstance)
	{
		COMObjectPointer<ID3D11DeviceContext> aContext = Engine::GetInstance()->GetEngine()->GetContext();

		if (myIsLoaded == true)
		{
			if (mySurfaces.Size() != 0)
			{
				ID3DX11EffectMatrixVariable* toWorld = aShadowEffect->GetEffect()->GetVariableByName("myToWorldShadowMatrix")->AsMatrix();
				if (toWorld->IsValid() == false)
				{
					DL_ASSERT("Couldn't find myToWorldMatrix variable in shadowEffect");
				}
				toWorld->SetMatrix(&anInstance.myMatrix[0]);

				aContext->IASetVertexBuffers(0, 1, &myVertexBuffer.myVertexBuffer, &myVertexBuffer.myStride, &myVertexBuffer.myByteOffset);
				aContext->IASetIndexBuffer(myIndexBuffer.myIndexBuffer.Get(), myIndexBuffer.myIndexBufferFormat, myIndexBuffer.myByteOffset);

				D3DX11_TECHNIQUE_DESC techDesc;

				COMObjectPointer<ID3DX11EffectTechnique> tech = aShadowEffect->GetTechnique();
				tech->GetDesc(&techDesc);

				for (UINT p = 0; p < techDesc.Passes; ++p)
				{
					COMObjectPointer<ID3DX11EffectPass> pass = aShadowEffect->GetTechnique()->GetPassByIndex(p);

					if (pass->IsValid() == false)
					{
						DL_ASSERT("Pass invalid");
					}

					HRESULT passResult = pass->Apply(NULL, aContext.Get());
					if (FAILED(passResult))
					{
						DL_ASSERT("Pass failed.");
					}

					aContext->DrawIndexed(myIndexBuffer.myIndexCount, 0, 0);
				}
			}
		}
	}

	void Model::SetIsLoading(bool aIsLoading)
	{
		myIsLoading = aIsLoading;
	}

	bool Model::GetIsLoading()
	{
		return myIsLoading;
	}

	void Model::InitBuffers(COMObjectPointer<ID3D11Device>& aDevice, VertexDataWrapper& someVertexData, VertexIndexWrapper& someIndexData)
	{
		myVertexBuffer.Create(aDevice, &someVertexData);
		myIndexBuffer.Create(aDevice, &someIndexData);
	}

	Vector3<float> Model::GetPlaneNormal(const Vector3<float>& aFirstVertice, const Vector3<float>& aSecondVertice, const Vector3<float>& aThirdVertice) const
	{
		Vector3<float> a;
		Vector3<float> b;

		a = aFirstVertice - aSecondVertice;
		b = aFirstVertice - aThirdVertice;
		return GetNormalized(Cross(a, b));
	}

	const CU::GrowingArray<CU::PoolPointer<Model>> Model::GetChildren() const
	{
		return myChilds;
	}

	const CU::GrowingArray<Surface>& Model::GetSurfaces() const
	{
		return mySurfaces;
	}
}
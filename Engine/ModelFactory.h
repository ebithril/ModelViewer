#pragma once

#include "Model.h"
#include "../Common Utilities/Map.h"
#include "COMObjectPointer.h"
#include "../Common Utilities/ObjectPool.h"
#include "../Common Utilities/PoolPointer.h"

struct ID3D11Device;

namespace GraphicsEngine
{
	class FBXFactory;
	class Effect;
	class TextureFactory;

	enum eModelType
	{
		POLYGON,
		CUBE,
		CUBE_NORM_TEX,
		SKYBOX,
		CUBEVIDEO
	};

	enum class eLoaderState
	{
		IN_GAME,
		LOADING_SCREEN
	};

	class Instance;
	class Texture;

	class ModelFactory
	{
		friend class FBXFactory;

	public:
		ModelFactory();
		~ModelFactory();

		void Init(COMObjectPointer<ID3D11Device>& aDevice);
		void CreateModel(const std::string& aModelName, Effect* anEffect, const eModelType aModelType, const float aSize, Texture* aTexture = nullptr);
		CU::PoolPointer<Model> GetModel(const std::string& aModelName);

		CU::PoolPointer<Model> CreateUnsearchableModelFromBufferData(GE::VertexDataWrapper& someVertexData, GE::VertexIndexWrapper& someIndexData);

		void LoadAnimation(CU::PoolPointer<Model> aModel, const std::string& aAnimationName, const std::string& aFileName);

		void DoNotUnloadModel(const std::string& aModelName);
		void LoadRequestedModels();
		void UnloadUnusedModels();

		void CreateAllEmptyModels();


		void SetLoaderState(const eLoaderState aState);

		CU::PoolPointer<Model> AllocateEmptyChild();
	private:

		void CreateModel(const std::string& aModelName, Effect* anEffect);
		void CreateModelFromEmptyModel(const std::string& aFileName, Effect* anEffect);
		void CreateEmptyModel(const std::string& aModelName, Effect* anEffect);

		eLoaderState myLoadingState;

		void DeleteAll();
		CU::Map<std::string, CU::PoolPointer<Model>> myModels;
		CU::GrowingArray<std::string> myModelNames;
		CU::GrowingArray < CU::PoolPointer<Model>> myModelCount;
		CU::GrowingArray<CU::PoolPointer<Model>> myModelsToNoUnload;
		COMObjectPointer<ID3D11Device> myDevice;
		FBXFactory* myFBXFactory;
		TextureFactory* myTextureFactory;

		CU::ObjectPool<67108864> myObjectPool;
	};
}

namespace GE = GraphicsEngine;

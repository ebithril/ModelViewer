#pragma once
#include <vector>
#include <bitset>
#include "../Common Utilities/PoolPointer.h"

#include <mutex>

struct ID3D11Device;

class FBXLoader;
struct ModelData;
class FbxModelData;
struct Bone;
struct AnimationData;

namespace GraphicsEngine
{
	class Model;
	class TextureFactory;
	class Surface;
	class HierarchyBone;

	class FBXFactory
	{
	public:
		FBXFactory(TextureFactory* aTextureFactory);
		~FBXFactory();

		void LoadModel(COMObjectPointer<ID3D11Device>& aDevice, const char* aFilePath, CU::PoolPointer<Model>& aModel);
		void LoadAnimation(COMObjectPointer<ID3D11Device>& aDevice, const char* aFilePath, CU::PoolPointer<Model>& aModel, const std::string& aName);
	private:
		void FillData(COMObjectPointer<ID3D11Device>& aDevice, ModelData* someData, CU::PoolPointer<Model>& outData);
		void CreateModel(COMObjectPointer<ID3D11Device>& aDevice, FbxModelData* someModelData, CU::PoolPointer<Model>& aModel);
		void FillAnimationData(FbxModelData* someModelData, CU::PoolPointer<Model>& aModel, const std::string& aName);
		void FillHierarchyAnimationData(FbxModelData* someModelData, CU::PoolPointer<Model>& aModel);
		void FillBoneAnimationData(FbxModelData* someModelData, CU::PoolPointer<Model>& aModel, const std::string& aName);
		void BuildBoneHierarchy(Bone& aBone, AnimationData* aAnimationData, HierarchyBone& aOutBone);

		void AddMissingTextures(Surface& aSurface, const std::bitset<8>& aTextureBitset);

		FBXLoader *myLoader;
		TextureFactory* myTextureFactory;

		std::mutex myLoadMutex;

		struct FBXData
		{
			FbxModelData* myData;
			std::string myPath;
		};
	};
}

namespace GE = GraphicsEngine;
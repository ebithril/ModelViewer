#pragma once

#include "../Common Utilities/GrowingArray.h"
#include "../Common Utilities/Matrix.h"
#include "VertexBufferWrapper.h"
#include "IndexBufferWrapper.h"
#include "Surface.h"
#include "../Common Utilities/PoolPointer.h"

#include <d3d11.h>

struct ID3D11Buffer;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct D3D11_INPUT_ELEMENT_DESC;

enum eSkyboxSide
{
	eBOTTOM,
	eTOP,
	eRIGHT,
	eLEFT,
	eFORWARD,
	eBACK,
};

namespace CommonUtilities
{
	struct RenderCommandInstance;
}

namespace GraphicsEngine
{
	class Effect;
	class Camera;
	class Texture;
	class Light;
	struct VertexDataWrapper;
	struct VertexIndexWrapper;
	class FBXFactory;
	class Surface;
	struct ModelDataStruct;
	class Instance;
	class Animation;
	class TransformationNode;
	class AnimationContainer;
	struct SceneRenderData;

	class Model
	{
		friend FBXFactory;
		friend Instance;
	public:
		
		Model();
		Model(const std::string& aModelFilename);
		~Model();

		Texture* GetOriginalDiffuseTexture();
		Texture* GetOriginalEmissiveTexture();

		void SetDiffuseTexture(Texture* aTexture);
		void SetEmissiveTexture(Texture* aTexture);

		Effect* GetEffect();
		void SetEffect(Effect* anEffect);
		
		void InitFromBufferData(VertexDataWrapper& someVertexData, VertexIndexWrapper& someIndexData, const D3D_PRIMITIVE_TOPOLOGY& aTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		void InitModel(COMObjectPointer<ID3D11Device>& aDevice, ModelDataStruct& aModelData);
		void InitPolygon(COMObjectPointer<ID3D11Device>& aDevice, Effect* anEffect, const float aSize);
		void InitCube(COMObjectPointer<ID3D11Device>& aDevice, Effect* anEffect, const float aSize);
		void InitNormCube(COMObjectPointer<ID3D11Device>& aDevice, Effect* anEffect, const float aSize, Texture* aTexture);
		void InitNormCubeVideo(COMObjectPointer<ID3D11Device>& aDevice, Effect* anEffect, const float aSize, Texture* aTexture);
		void InitSkyBox(COMObjectPointer<ID3D11Device>& aDevice, Effect* anEffect, const float aSize, Texture* aTexture);
		void Render(SceneRenderData& someData, CU::RenderCommandInstance& aRenderCommand, const Matrix44f& anInstance, const bool anObjectsFirstRender);

		void UnLoad();

		void InitSkybox(const std::string& aSkyboxName);

		bool IsLoaded() const;
		bool AnimationIsLoaded(const std::string& anAnimationName) const;
		const std::string& GetFileName();

		void SetShaderVariable(const std::string& aName, float aValue);
		void SetUpdateDynamic(const bool aFlag);

		void AddChild(CU::PoolPointer<Model>& aModel);

		const CU::GrowingArray<CU::PoolPointer<Model>> GetChildren() const;
		const CU::GrowingArray<Surface>& GetSurfaces() const;


		void RenderShadowShaderData(Effect* aShadowEffect, const Matrix44f& anInstance);

		void SetIsLoading(bool aIsLoading);
		bool GetIsLoading();
	private:
		void InitSkyBoxChild(const std::string& aTextureName, eSkyboxSide aSkyBoxSide);
		void InitBuffers(COMObjectPointer<ID3D11Device>& aDevice, VertexDataWrapper& someVertexData, VertexIndexWrapper& someIndexData);
		Vector3<float> Model::GetPlaneNormal(const Vector3<float>& aFirstVertice, const Vector3<float>& aSecondVertice, const Vector3<float>& aThirdVertice) const;

		Effect* myEffect;
		std::string myEffectFile;
		std::string myModelFile;
		VertexBufferWrapper myVertexBuffer;
		IndexBufferWrapper myIndexBuffer;

		CU::GrowingArray<Surface> mySurfaces;
		CU::GrowingArray<CU::PoolPointer<Model>> myChilds;

		float myTotalTime;
		int myLodDisplayLevel;
		CU::GrowingArray<float> myLodThreashholds;
		bool myIsUsingLOD;
		bool addTime;
		volatile bool myIsLoaded;
		bool myUpdateDynamic;
		volatile bool myIsLoading;

		TransformationNode* myTransform;
		CU::GrowingArray<TransformationNode*> myChildTransforms;

		Texture* myOriginalDiffuse;
		Texture* myOriginalEmissive;
		AnimationContainer* myAnimation;
	};
}
namespace GE = GraphicsEngine;


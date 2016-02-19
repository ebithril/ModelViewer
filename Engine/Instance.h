#pragma once

#include "AnimationDefines.h"
#include "..\Common Utilities\Matrix44.h"
#include "../Common Utilities/StaticArray.h"
#include "TransformationNodeInstance.h"
#include "../Common Utilities/PoolPointer.h"
#include "../Common Utilities/RenderCommandInstance.h"

namespace GraphicsEngine
{
	struct AnimationToLoad
	{
		std::string myAnimationName;
		std::string myFileName;
	};

	class AnimationInstance;
	class Camera;
	class Light;
	class ModelProxy;
	class RenderProcessTarget;
	class Model;
	class Effect;
	
	struct SceneRenderData;

	class Instance
	{
	public:
		Instance(CU::PoolPointer<Model> aModel);
		Instance(const Instance& aInstance);
		~Instance();

		void Init();

		virtual void Render(SceneRenderData& someScenerRenderData, CU::RenderCommandInstance& aRenderCommand);
		virtual void RenderShadowData(Effect* anEffect, Matrix44f anOrientation);
		void Update();
		void BuildHierarchy();
		virtual Effect* GetEffect();

		void LoadAnimation(const std::string& aAnimationName, const std::string& aFileName);

		void StartAnimation(const std::string aName, bool aShouldLoop);
		void StopAnimation(const std::string aName);

		virtual bool ModelIsNullPointer();

		virtual bool IsLoaded()const;

		Texture* GetOriginalDiffuse();

	private:
		const bool AnimationExists(const std::string& aAnimationName) const;
		void BuildHierarchy(TransformationNodeInstance& aHierarchy, Model* aModel);
		void RenderModel(SceneRenderData& someScenerRenderData, CU::RenderCommandInstance& aRenderCommand, Model* aModel,
			const Matrix44<float>& aParentSpace, TransformationNodeInstance& aHierarchy, const bool anObjectFirstTimeRender);

		virtual void RenderShadowData(Effect* anEffect, Matrix44f aParentSpace, Model* aModel, TransformationNodeInstance& aHierarchy);

		CU::PoolPointer<Model> myModel;
		TransformationNodeInstance myHierarchy;

		AnimationInstance* myAnimation;
		CU::StaticArray<Matrix44<float>, MAX_NR_OF_BONES> myBones;
		CU::GrowingArray<AnimationToLoad> myAnimationsToLoad;

		float myCurrentAnimationTime;
		bool myIsInited;
	};
}

namespace GE = GraphicsEngine;
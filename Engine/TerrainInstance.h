#pragma once
#include "Instance.h"

#include "../Common Utilities/SharedPointer.h"

namespace GraphicsEngine
{
	class TerrainInstance : public Instance
	{
	public:
		TerrainInstance();
		~TerrainInstance();

		void SetModel(SharedPointer<Model> aModel);

		virtual void Render(SceneRenderData& someScenerRenderData, CU::RenderCommandInstance& aRenderCommand) override;
		virtual void RenderShadowData(Effect* anEffect, Matrix44f anOrientation) override;

		Effect* GetEffect();

		virtual bool ModelIsNullPointer() override;
		virtual bool IsLoaded() const override;


	private:
		SharedPointer<Model> myModel;
	};
}
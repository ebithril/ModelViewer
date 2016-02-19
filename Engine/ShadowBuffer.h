#pragma once

#include "Camera.h"
#include "ShadowRenderData.h"


namespace CommonUtilities
{
	struct CircleStruct;
}

namespace GraphicsEngine
{
	class Frustum;

	class ShadowBuffer
	{
	public:

		ShadowBuffer();
		~ShadowBuffer();

		void Render();
		void Clear();
		
		void InitDirectionalLightShadow(const Vector3f& aLightDirection , const Camera& aCameraFrustumToCover);

		void AddShadowData(const ShadowRenderData& someData);

		bool Collision(const Vector3f& aPosition, const float aRadius);

		const CU::CircleStruct& GetCullingSphere();

		const Matrix44f GetViewProjection() const;

		const CU::GrowingArray<ShadowRenderData>& GetShadowRenderData()const;

	private:

		Camera myCamera;
		CU::GrowingArray<ShadowRenderData> myShadowRenderData;
		Matrix44f myViewProjection;
	};
}
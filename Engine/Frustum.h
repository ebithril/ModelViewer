#pragma once

#include "../Common Utilities/Plane.h"
#include "../Common Utilities/StaticArray.h"
#include "../Common Utilities/Matrix44.h"
#include "../Common Utilities/RenderCommandLine.h"

namespace GraphicsEngine
{
	class Frustum
	{
	public:
		Frustum();
		~Frustum();

		void Init(float aFar, float aNear);
		void Update(const Matrix44<float>& aCameraOrientation, float aAspectRatio, float aFov);

		bool CheckIfSphereInside(const Vector3<float>& aPosition, float aRadius) const;
		bool CheckPointInsideFrustum(const Vector3<float>& aPosition) const;
		bool CheckQuadInsideFrustum(const Matrix44<float>& aOrientation, const Vector2<float>& aDimensions);
		bool CheckAABBInsideFrustum(const Vector3<float>& someMinValues, const Vector3<float>& someMaxValues);

		float GetRadius() const;
		const CU::StaticArray<Vector3<float>, 8>& GetPointPositions() const;
		const Vector3f& GetPosition() const;
		void RenderFrustum() const;

	private:
		CU::GrowingArray<CU::RenderCommandLine> myLines;
		CU::StaticArray<Vector3<float>, 8> myPointPositions;
		CU::StaticArray<Plane<float>, 6> myPlanes;

		float myFar;
		float myNear;
		float myRadius;

		Vector3<float> myPosition;
	};
}

namespace GE = GraphicsEngine;
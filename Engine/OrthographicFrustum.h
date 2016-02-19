#pragma once
#include "../Common Utilities/PlaneVolume.h"
#include "../Common Utilities/Matrix.h"
#include "../Common Utilities/RenderCommandLine.h"
#include "../Common Utilities/ColliderCircle.h"

namespace GraphicsEngine
{
	class OrthographicFrustum
	{
	public:
		OrthographicFrustum();
		~OrthographicFrustum();

		OrthographicFrustum(const Matrix44f& aCameraOrientation, const float aNear, const float aFar, const float aWidth, const float aHeight);
		void Render();

		bool CheckCollision(const Position& aPosition, const float aRadius) const;

		const CU::CircleStruct& GetCullingSphere() const;

	private:

		PlaneVolume<float> myPlaneVolume;
		CU::StaticArray<CU::RenderCommandLine, 18> myLines;
		CU::CircleStruct myCullingSphere;
	};
}
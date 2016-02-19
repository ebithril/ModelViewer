#include "Engine_Precompiled.h"
#include "OrthographicFrustum.h"
#include "../Common Utilities/EventManager.h"



namespace GraphicsEngine
{
	OrthographicFrustum::OrthographicFrustum()
	{
	}

	OrthographicFrustum::~OrthographicFrustum()
	{
	}

	GraphicsEngine::OrthographicFrustum::OrthographicFrustum(const Matrix44f& aCameraOrientation, const float aNear, const float aFar, const float aWidth, const float aHeight)
	{
		float halfWidth = aWidth * 0.5f;
		float halfHeight = aHeight * 0.5f;

		Position nearUpperLeft(-halfWidth, halfHeight, aNear);
		Position nearUpperRight(halfWidth, halfHeight, aNear);
		Position nearBottomRight(halfWidth, -halfHeight, aNear);
		Position nearBottomLeft(-halfWidth, -halfHeight, aNear);

		Position farUpperLeft(-halfWidth, halfHeight, aFar);
		Position farUpperRight(halfWidth, halfHeight, aFar);
		Position farBottomRight(halfWidth, -halfHeight, aFar);
		Position farBottomLeft(-halfWidth, -halfHeight, aFar);

		nearUpperLeft = ToVector3(ToVector4(nearUpperLeft, 1.f) * aCameraOrientation);
		nearUpperRight = ToVector3(ToVector4(nearUpperRight, 1.f) * aCameraOrientation);
		nearBottomRight = ToVector3(ToVector4(nearBottomRight, 1.f) * aCameraOrientation);
		nearBottomLeft = ToVector3(ToVector4(nearBottomLeft, 1.f) * aCameraOrientation);

		farUpperLeft = ToVector3(ToVector4(farUpperLeft, 1.f) * aCameraOrientation);
		farUpperRight = ToVector3(ToVector4(farUpperRight, 1.f) * aCameraOrientation);
		farBottomRight = ToVector3(ToVector4(farBottomRight, 1.f) * aCameraOrientation);
		farBottomLeft = ToVector3(ToVector4(farBottomLeft, 1.f) * aCameraOrientation);

		myPlaneVolume.AddPlane(Plane<float>(nearUpperLeft, nearUpperRight, nearBottomLeft));
		myPlaneVolume.AddPlane(Plane<float>(farUpperLeft, farBottomLeft, farBottomRight));
		myPlaneVolume.AddPlane(Plane<float>(nearUpperLeft, nearBottomLeft, farUpperLeft));
		myPlaneVolume.AddPlane(Plane<float>(farUpperLeft, nearUpperRight, nearUpperLeft));
		myPlaneVolume.AddPlane(Plane<float>(nearBottomRight, nearUpperRight, farBottomRight));
		myPlaneVolume.AddPlane(Plane<float>(nearBottomLeft, nearBottomRight, farBottomRight));

		myLines[0] = CU::RenderCommandLine(nearUpperLeft, nearUpperRight);
		myLines[1] = CU::RenderCommandLine(nearBottomRight, nearUpperRight);
		myLines[2] = CU::RenderCommandLine(nearBottomRight, nearBottomLeft);
		myLines[3] = CU::RenderCommandLine(nearUpperLeft, nearBottomLeft);

		myLines[4] = CU::RenderCommandLine(nearUpperLeft, farUpperLeft);
		myLines[5] = CU::RenderCommandLine(nearUpperRight, farUpperRight);
		myLines[6] = CU::RenderCommandLine(nearBottomRight, farBottomRight);
		myLines[7] = CU::RenderCommandLine(nearBottomLeft, farBottomLeft);

		myLines[8] = CU::RenderCommandLine(farUpperLeft, farUpperRight);
		myLines[9] = CU::RenderCommandLine(farBottomRight, farUpperRight);
		myLines[10] = CU::RenderCommandLine(farBottomRight, farBottomLeft);
		myLines[11] = CU::RenderCommandLine(farUpperLeft, farBottomLeft);

		myLines[12] = CU::RenderCommandLine(aCameraOrientation.GetTranslation(), aCameraOrientation.GetTranslation() + myPlaneVolume.GetPlane(0).GetNormal() * aFar * 0.5f);
		myLines[13] = CU::RenderCommandLine(ToVector3(Vector4f(0, 0, aFar, 1) * aCameraOrientation), ToVector3(Vector4f(0, 0, aFar, 1) * aCameraOrientation) + myPlaneVolume.GetPlane(1).GetNormal() * aFar * 0.5f);
		myLines[14] = CU::RenderCommandLine(nearUpperLeft, nearUpperLeft + myPlaneVolume.GetPlane(2).GetNormal() * 100.f);
		myLines[15] = CU::RenderCommandLine(farUpperLeft, farUpperLeft + myPlaneVolume.GetPlane(3).GetNormal() * 100.f); 
		myLines[16] = CU::RenderCommandLine(nearBottomRight, nearBottomRight + myPlaneVolume.GetPlane(4).GetNormal() * 100.f);
		myLines[17] = CU::RenderCommandLine(nearBottomLeft, nearBottomLeft + myPlaneVolume.GetPlane(5).GetNormal() * 100.f);

		myCullingSphere.myPosition = ToVector3(Vector4f(0, 0, aFar * 0.5f, 1) * aCameraOrientation);
		myCullingSphere.myRadius = Length(nearUpperLeft - farBottomRight) * 0.5f;
	}

	void OrthographicFrustum::Render()
	{
		for (unsigned short i = 0; i < 18; i++)
		{
			CU::EventManager::GetInstance()->AddRenderCommand(myLines[i]);
		}
	}

	bool OrthographicFrustum::CheckCollision(const Position& aPosition, const float aRadius) const
	{
		return myPlaneVolume.Inside(aPosition, aRadius);
	}

	const CU::CircleStruct& OrthographicFrustum::GetCullingSphere() const
	{
		return myCullingSphere;
	}

}

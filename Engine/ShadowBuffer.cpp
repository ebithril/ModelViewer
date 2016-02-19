#include "Engine_Precompiled.h"
#include "ShadowBuffer.h"
#include "../Common Utilities/EventManager.h"

namespace GraphicsEngine
{

	void ShadowBuffer::InitDirectionalLightShadow(const Vector3f& aLightDirection, const Camera& aCameraToCover)
	{
		myShadowRenderData.Init(32);
		const CU::StaticArray<Vector3f, 8>& cameraPoints = aCameraToCover.GetFrustum().GetPointPositions();

		Matrix44f lightSpace;
		lightSpace.LookAt(aLightDirection * -1.f);

		CU::StaticArray<Vector3f, 8> positionInLightSpace;

		for (unsigned short i = 0; i < 8; i++)
		{
			Vector4f position = Vector4f(cameraPoints[i].x, cameraPoints[i].y, cameraPoints[i].z, 1) * lightSpace;
			positionInLightSpace[i] = { position.myX, position.myY, position.myZ };
		}

		Vector3f maxPosition = positionInLightSpace[0];
		Vector3f minPosition = positionInLightSpace[0];

		for (unsigned short i = 1; i < 8; i++)
		{
			maxPosition.myX = MAX(maxPosition.myX, positionInLightSpace[i].myX);
			maxPosition.myY = MAX(maxPosition.myY, positionInLightSpace[i].myY);
			maxPosition.myZ = MAX(maxPosition.myZ, positionInLightSpace[i].myZ);

			minPosition.myX = MIN(minPosition.myX, positionInLightSpace[i].myX);
			minPosition.myY = MIN(minPosition.myY, positionInLightSpace[i].myY);
			minPosition.myZ = MIN(minPosition.myZ, positionInLightSpace[i].myZ);
		}

		Vector3f depth = maxPosition - minPosition;

		Position lightPosition = aCameraToCover.GetPosition() + aCameraToCover.GetOrientation().GetForwardVector() * aCameraToCover.GetFarPlaneDistance() * 0.5f;
		lightPosition = lightPosition + (aLightDirection * -1.f) * aCameraToCover.GetFarPlaneDistance() * 0.5f;


		lightSpace.SetTranslation(lightPosition);

		myCamera.InitOrthographicProjection({ depth.myX, depth.myY }, Vector3f(), 0.2f, depth.myZ * 1.f);
		myCamera.SetOrientation(lightSpace);

		myViewProjection = myCamera.GetInverse() * myCamera.GetProjection();

	}

	void ShadowBuffer::AddShadowData(const GE::ShadowRenderData& someData)
	{
		myShadowRenderData.Add(someData);
	}

	bool ShadowBuffer::Collision(const Vector3f& aPosition, const float aRadius)
	{
		return myCamera.Collision(aPosition, aRadius);
	}

	const CU::CircleStruct& ShadowBuffer::GetCullingSphere()
	{
		return myCamera.GetCullingSphere();
	}

	const Matrix44f ShadowBuffer::GetViewProjection() const
	{
		return myViewProjection;
	}

	const CU::GrowingArray<ShadowRenderData>& ShadowBuffer::GetShadowRenderData() const
	{
		return myShadowRenderData;
	}

	void ShadowBuffer::Render()
	{
		if (myCamera.IsInited() == true)
		{
			myCamera.Update();
			myCamera.Render();
		}
	}

	void ShadowBuffer::Clear()
	{
		if (myShadowRenderData.IsInited() == true)
		{
			myShadowRenderData.RemoveAll();
		}
	}

	ShadowBuffer::ShadowBuffer()
	{
	}

	ShadowBuffer::~ShadowBuffer()
	{
	}
}


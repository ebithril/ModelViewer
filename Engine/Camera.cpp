#include "Engine_Precompiled.h"
#include "Camera.h"
#include "Engine.h"
#include "SetupInfo.h"
#include "../Common Utilities/EventManager.h"
#include "../Common Utilities/Lerp.h"
#include "../Common Utilities/Macros.h"
#include "../Common Utilities/Events.h"
#include <DirectXMath.h>
#include "../Common Utilities/ColliderCircle.h"

namespace GraphicsEngine
{
	Camera::Camera()
	{
	}

	Camera::~Camera()
	{
	}

	void Camera::Init(const Vector2<float>& aProjectionWidthAndHeight, const Position& aPosition, const float& aNear, const float& aFar, const float aFieldOfViewInRadians, bool anEnableResize)
	{
		if (anEnableResize == true)
		{
			ObserveEvent(CU::eEvent::RESIZE, HANDLE_EVENT_FUNCTION(HandleOnResizeEvent));
		}
		myNear = aNear;
		myFar = aFar;
		myScale = 1.0f;
		myOrientation.SetTranslation(aPosition);
		myFov = aFieldOfViewInRadians;
		ResizeProjection(aProjectionWidthAndHeight, aNear, aFar);
		myFrustum.Init(aFar, aNear);
		myFrustum.Update(myOrientation, aProjectionWidthAndHeight.myY / aProjectionWidthAndHeight.myX, myFov);
		myIsInited = true;
		myIsOrthogonal = false;
	}

	void Camera::ResizeProjection(const Vector2<float>& aProjectionWidthAndHeight, const float& aNear, const float& aFar)
	{
		myWidthAndHeight = aProjectionWidthAndHeight;
		if (myIsOrthogonal == false)
		{
			myProjection = Matrix44f::CreateProjectionMatrixLH(aNear, aFar, aProjectionWidthAndHeight.myY / aProjectionWidthAndHeight.myX, myFov, myScale);
		}
		else
		{
			DirectX::XMMATRIX orthoMatrix = DirectX::XMMatrixOrthographicLH(aProjectionWidthAndHeight.myX, aProjectionWidthAndHeight.myY, aNear, aFar);

			myProjection.myMatrix[0] = orthoMatrix.r[0].m128_f32[0];
			myProjection.myMatrix[1] = orthoMatrix.r[0].m128_f32[1];
			myProjection.myMatrix[2] = orthoMatrix.r[0].m128_f32[2];
			myProjection.myMatrix[3] = orthoMatrix.r[0].m128_f32[3];
			myProjection.myMatrix[4] = orthoMatrix.r[1].m128_f32[0];
			myProjection.myMatrix[5] = orthoMatrix.r[1].m128_f32[1];
			myProjection.myMatrix[6] = orthoMatrix.r[1].m128_f32[2];
			myProjection.myMatrix[7] = orthoMatrix.r[1].m128_f32[3];
			myProjection.myMatrix[8] = orthoMatrix.r[2].m128_f32[0];
			myProjection.myMatrix[9] = orthoMatrix.r[2].m128_f32[1];
			myProjection.myMatrix[10] = orthoMatrix.r[2].m128_f32[2];
			myProjection.myMatrix[11] = orthoMatrix.r[2].m128_f32[3];
			myProjection.myMatrix[12] = orthoMatrix.r[3].m128_f32[0];
			myProjection.myMatrix[13] = orthoMatrix.r[3].m128_f32[1];
			myProjection.myMatrix[14] = orthoMatrix.r[3].m128_f32[2];
			myProjection.myMatrix[15] = orthoMatrix.r[3].m128_f32[3];
		}
	}

	const Matrix44f Camera::GetInverse() const
	{
		return myOrientation.GetInverse();
	}

	const Matrix44f& Camera::GetProjection() const
	{
		return myProjection;
	}

	void Camera::Move(const Position& aRelativePosition)
	{
		myOrientation.Move(aRelativePosition);
	}

	void Camera::SetPosition(const Position& aPosition)
	{
		myOrientation.SetTranslation(aPosition);
	}

	const Position Camera::GetPosition()const
	{
		return myOrientation.GetTranslation();
	}

	const Matrix44f& Camera::GetOrientation() const
	{
		return myOrientation;
	}

	const float Camera::GetMyZoom() const
	{
		return myScale;
	}

	void Camera::Zoom(const float aZoom)
	{
		myScale = aZoom;
		ResizeProjection(GetWidthAndHeight(), myNear, myFar);
	}

	Vector2<float> Camera::GetWidthAndHeight() const
	{
		return myWidthAndHeight;
	}

	void Camera::FocusOnPoint(Position aFocusPoint)
	{
		Vector3<float> forwardVector = aFocusPoint - myOrientation.GetTranslation();
		Normalize(forwardVector);

		Position position = myOrientation.GetTranslation();

		myOrientation = Matrix44f();

		myOrientation = myOrientation.CreateRotateAroundX(forwardVector.myX * PI) * myOrientation;
		myOrientation = myOrientation.CreateRotateAroundY(forwardVector.myY * PI) * myOrientation;
		myOrientation = myOrientation.CreateRotateAroundZ(forwardVector.myZ * PI) * myOrientation;

		myOrientation.SetTranslation(position);
	}

	void Camera::SetOrientation(const Matrix44<float>& anOrientation)
	{
		myOrientation = anOrientation;
	}

	void Camera::SetPlayerOrientation(const Matrix44<float>& anOrientation)
	{
		myOldPlayerOrientation = myPlayerOrientation;
		myPlayerOrientation = anOrientation;
	}

	void Camera::SetDirection(const Vector3<float>& aDirection)
	{
		Matrix44<float> newOrientation = Matrix44<float>::CreateMatrixFromForwardVector(aDirection);
		newOrientation.SetTranslation(myOrientation.GetTranslation());
		myOrientation = newOrientation;
		myFrustum.Update(myOrientation, myWidthAndHeight.myY / myWidthAndHeight.myX, myFov);
	}

	bool Camera::HandleOnResizeEvent(const CU::PoolPointer<CU::Event>& anEvent)
	{
		anEvent;
		Vector2<int> resolution = Engine::GetInstance()->GetSetupInfo().myResolution;

		ResizeProjection(Vector2<float>(static_cast<float>(resolution.myX), static_cast<float>(resolution.myY)), myNear, myFar);
		return true;
	}

	void Camera::Rotate(const Vector3<float>& aRotationInRadians)
	{
		myXRotation *= Matrix44f::CreateRotateAroundX(aRotationInRadians.myX);
		myYRotation *= Matrix44f::CreateRotateAroundY(aRotationInRadians.myY);
		myZRotation *= Matrix44f::CreateRotateAroundZ(aRotationInRadians.myZ);

		Position position = myOrientation.GetTranslation();

		myOrientation.SetTranslation({ 0, 0, 0 });

		myOrientation = myYRotation * myXRotation * myZRotation;

		myOrientation.SetTranslation(position);
	}

	void Camera::TranslationRotation(const Vector3<float>& aRotationInRadians, const Position& aPosition)
	{
		myOrientation.TranslationRotation(aRotationInRadians, aPosition);
	}

	void Camera::Update()
	{
		if (myIsOrthogonal == false)
		{
			UpdateFrustum();
		}
		else
		{
			myOrthographicFrustum = OrthographicFrustum(myOrientation, myNear, myFar, myWidthAndHeight.myX, myWidthAndHeight.myY);
		}
	}

	const Frustum& Camera::GetFrustum() const
	{
		return myFrustum;
	}

	void Camera::UpdateFrustum()
	{
		myFrustum.Update(myOrientation, myWidthAndHeight.myX / myWidthAndHeight.myY, myFov);
	}

	const float Camera::GetFarPlaneDistance() const
	{
		return myFar;
	}

	const bool Camera::IsInited()
	{
		return myIsInited;
	}

	void Camera::InitOrthographicProjection(const Vector2<float>& aProjectionWidthAndHeight, const Position& aPosition, const float& aNear, const float& aFar)
	{
		myNear = aNear;
		myFar = aFar;
		myScale = 1.0f;
		myOrientation.SetTranslation(aPosition);
		myFov = 0;
		ResizeProjection(aProjectionWidthAndHeight, aNear, aFar);
		myOrthographicFrustum = OrthographicFrustum(myOrientation, aNear, aFar, aProjectionWidthAndHeight.myX, aProjectionWidthAndHeight.myY);
		myIsInited = true;
		myIsOrthogonal = true;
		myWidthAndHeight = aProjectionWidthAndHeight;
	}

	void Camera::Render()
	{
		if (myIsOrthogonal == false)
		{
			myFrustum.RenderFrustum();
		}
		else
		{
			myOrthographicFrustum.Render();
		}
	}

	bool Camera::Collision(const Vector3f& aPosition, const float aRadius) const
	{
		if (myIsOrthogonal == true)
		{
			return myOrthographicFrustum.CheckCollision(aPosition, aRadius);
		}
		else
		{
			return myFrustum.CheckIfSphereInside(aPosition, aRadius);
		}
	}

	const CU::CircleStruct Camera::GetCullingSphere() const
	{
		CU::CircleStruct data;
		if (myIsOrthogonal == true)
		{
			data = myOrthographicFrustum.GetCullingSphere();
		}
		else
		{
			data.myPosition = myFrustum.GetPosition();
			data.myRadius = myFrustum.GetRadius();
		}

		return data;
	}

	const float Camera::GetAspectRatio() const
	{
		return myWidthAndHeight.myY / myWidthAndHeight.myX;
	}

}
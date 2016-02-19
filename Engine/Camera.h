#pragma once
#include "OrthographicFrustum.h"
#include "../Common Utilities/Matrix.h"
#include "../Common Utilities/Vector.h"
#include "../Common Utilities/Observer.h"
#include "../Common Utilities/PoolPointer.h"
#include "Frustum.h"


namespace CommonUtilities
{
	class Event;
}

namespace GraphicsEngine
{
	class Camera : public CU::Observer
	{
	public:
		Camera();
		~Camera();
		const float GetAspectRatio() const;

		void Init(const Vector2<float>& aProjectionWidthAndHeight, const Position& aPosition, const float& aNear, const float& aFar, const float aFieldOfViewInRadians = DEGREE_TO_RADIAN(90.f), bool anEnableResize = true);
		void InitOrthographicProjection(const Vector2<float>& aProjectionWidthAndHeight, const Position& aPosition, const float& aNear, const float& aFar);
		void ResizeProjection(const Vector2<float>& aProjectionWidthAndHeight, const float& aNear, const float& aFar);

		const Matrix44f GetInverse() const;
		const Matrix44f& GetProjection() const;
		const Matrix44f& GetOrientation() const;

		void Move(const Position& aRelativePosition);
		void SetPosition(const Position& aPosition);

		const Position GetPosition()const;

		void Zoom(const float aZoom);
		const float GetMyZoom()const;

		void Rotate(const Vector3<float>& aRotationInRadians);
		void TranslationRotation(const Vector3<float>& aRotationInRadians, const Position& aPosition);

		void SetOrientation(const Matrix44<float>& anOrientation);
		void SetPlayerOrientation(const Matrix44<float>& anOrientation);
		void SetDirection(const Vector3<float>& aDirection);

		void FocusOnPoint(Position aFocusPoint);

		Vector2<float> GetWidthAndHeight() const;

		const Frustum& GetFrustum() const;

		void Update();
		void Render();

		const float GetFarPlaneDistance() const;

		const bool IsInited();

		bool Collision(const Vector3f& aPosition, const float aRadius)const;

		const CU::CircleStruct GetCullingSphere() const;

	private:

		void UpdateFrustum();
		bool HandleOnResizeEvent(const CU::PoolPointer<CU::Event>& anEvent);

		Frustum myFrustum;
		OrthographicFrustum myOrthographicFrustum;
		Matrix44f myOrientation;
		Matrix44f myProjection;
		Matrix44f myXRotation;
		Matrix44f myYRotation;
		Matrix44f myZRotation;
		Matrix44f myPlayerOrientation;
		Matrix44f myOldPlayerOrientation;
		Vector2<float> myWidthAndHeight;
		float myScale;
		float myNear;
		float myFar;
		float myFov;
		bool myIsOrthogonal;

		bool myIsInited;
	};
}

namespace GE = GraphicsEngine;
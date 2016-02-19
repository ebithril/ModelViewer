#pragma once
#include "Collider.h"

namespace CommonUtilities
{
	struct CircleStruct
	{
		float myRadius;
		Position myPosition;
	};

	class ColliderCircle : public Collider
	{
	public:
		ColliderCircle();
		ColliderCircle(const CircleStruct& someData);
		~ColliderCircle();

		virtual bool CheckCollision(const Collider& aCollider, const CollisionStruct& aCollisionStruct) const override;
		virtual bool CheckCollision(const ColliderCircle& aColliderCircle, const CollisionStruct& aCollisionStruct) const override;
		virtual bool CheckCollision(const Vector3<float>& aRayDirection, const Vector3<float>& aRayPosition, const Matrix44f& anObjectMatrix, const float anObjectScale, Vector3f& aTargetPosition, bool aCheckOnlyRoot = false) const override;
		virtual bool CheckCollision(const Vector3<float>& aRayDirection, const Vector3<float>& aRayPosition, const Matrix44f& anObjectMatrix, const float anObjectScale, bool aCheckOnlyRoot = false) const override;

		virtual void Render(const Matrix44f& aSpace) const override;
		virtual void RenderChildren(const Matrix44f& aSpace) const override;

		const float GetRadius() const;
		const float GetRadius(const float& aScale) const;
		const Position GetPosition() const;
	private:
		float myRadius;
		Position myPosition;
	};

}

namespace CU = CommonUtilities;
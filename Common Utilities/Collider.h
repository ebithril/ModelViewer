#pragma once
#include "Vector.h"
#include "GrowingArray.h"
#include "PoolPointer.h"
#include "Matrix.h"

namespace ClientGameSpace
{
	class CollisionFactory;
}

namespace CommonUtilities
{
	class ColliderCircle;
	struct CollisionStruct;
	class Collider
	{
	public:
		friend class ClientGameSpace::CollisionFactory;
		Collider();
		~Collider();

		virtual bool CheckCollision(const Collider& aCollider, const CollisionStruct& aCollisionStruct) const = 0;
		virtual bool CheckCollision(const ColliderCircle& aColliderCircle, const CollisionStruct& aCollisionStruct) const = 0;
		virtual bool CheckCollision(const Vector3<float>& aRayDirection, const Vector3<float>& aRayPosition, const Matrix44f& anObjectMatrix, const float anObjectScale, Vector3f& aTargetPosition, bool aCheckOnlyRoot = false) const = 0;
		virtual bool CheckCollision(const Vector3<float>& aRayDirection, const Vector3<float>& aRayPosition, const Matrix44f& anObjectMatrix, const float anObjectScale, bool aCheckOnlyRoot = false) const = 0;

		const ColliderCircle* GetAsColliderCircle() const;
		virtual void Render(const Matrix44f& aSpace) const = 0;
		virtual void RenderChildren(const Matrix44f& aSpace) const = 0;

	protected: 
		GrowingArray<PoolPointer<Collider>> myColliders;

	};
}

namespace CU = CommonUtilities;
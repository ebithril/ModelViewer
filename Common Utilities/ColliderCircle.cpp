#include "Common Utilities_Precompiled.h"
#include "ColliderCircle.h"
#include "Intersection.h"
#include "CollisionStructs.h"
#include "../Engine/GfxFactoryWrapper.h"
#include "../Engine/Model.h"
#include "../Engine/ModelFactory.h"
#include "EventManager.h"
#include "RenderCommandInstance.h"

namespace CommonUtilities
{
	ColliderCircle::ColliderCircle()
	{
		myPosition = Position(0.f, 0.f, 0.f);
		myRadius = 0;
	}


	ColliderCircle::~ColliderCircle()
	{
	}

	ColliderCircle::ColliderCircle(const CircleStruct& someData) : Collider()
	{
		myRadius = someData.myRadius;
		myPosition = someData.myPosition;
	}

	bool ColliderCircle::CheckCollision(const Collider& aCollider, const CollisionStruct& aCollisionStruct) const
	{
		CollisionStruct newStruct;
		newStruct.myObjectSpace = aCollisionStruct.myArgumentObjectSpace;
		newStruct.myObjectScale = aCollisionStruct.myArgumentObjectScale;
		newStruct.myArgumentObjectSpace = aCollisionStruct.myObjectSpace;
		newStruct.myArgumentObjectScale = aCollisionStruct.myObjectScale;

		return aCollider.CheckCollision(*this, newStruct);
	}

	bool ColliderCircle::CheckCollision(const ColliderCircle& aColliderCircle, const CollisionStruct& aCollisionStruct) const
	{
		Matrix44f thisCircleSpace;
		thisCircleSpace.SetTranslation(myPosition);
		thisCircleSpace = thisCircleSpace * aCollisionStruct.myObjectSpace;

		Matrix44f secondCircleSpace;
		secondCircleSpace.SetTranslation(aColliderCircle.myPosition);
		secondCircleSpace = secondCircleSpace * aCollisionStruct.myArgumentObjectSpace;



		//Vector3<float> thisCirclePosition = myPosition * aCollisionStruct.myObjectSpace;
		//Vector3<float> secondCirclePosition = { aColliderCircle.myPosition.x + aCollisionStruct.myArgumentObjectPosition.x, aColliderCircle.myPosition.y + aCollisionStruct.myArgumentObjectPosition.y, aColliderCircle.myPosition.z + aCollisionStruct.myArgumentObjectPosition.z };

		if (CU::Intersection::SphereVsSphere(thisCircleSpace.GetTranslation(), myRadius * aCollisionStruct.myObjectScale,
			secondCircleSpace.GetTranslation(), aColliderCircle.myRadius* aCollisionStruct.myArgumentObjectScale) == true)
		{

			bool toReturn = false;

			if (myColliders.Size() == 0 && aColliderCircle.myColliders.Size() > 0)
			{
				for (CU::PoolPointer<CU::Collider> collider : aColliderCircle.myColliders)
				{
					toReturn = CheckCollision(*collider, aCollisionStruct);

					if (toReturn == true)
					{
						return true;
					}
				}
			}
			else if (myColliders.Size() > 0 && aColliderCircle.myColliders.Size() > 0)
			{
				for (unsigned short i = 0; i < myColliders.Size(); i++)
				{
					for (unsigned short j = 0; j < aColliderCircle.myColliders.Size(); j++)
					{
						toReturn = myColliders[i]->CheckCollision(*aColliderCircle.myColliders[j], aCollisionStruct);
						if (toReturn == true)
						{
							return true;
						}
					}
				}
			}
			else if (myColliders.Size() > 0 && aColliderCircle.myColliders.Size() == 0)
			{
				for (CU::PoolPointer<CU::Collider> collider : myColliders)
				{
					toReturn = collider->CheckCollision(aColliderCircle, aCollisionStruct);

					if (toReturn == true)
					{
						return true;
					}
				}
			}
			else
			{
				toReturn = true;
			}

			return toReturn;
		}
		
		return false;
	}

	bool ColliderCircle::CheckCollision(const Vector3<float>& aRayDirection, const Vector3<float>& aRayPosition, const Matrix44f& anObjectMatrix, const float anObjectScale, bool aCheckOnlyRoot) const
	{
		Vector4f position = Vector4f(myPosition.myX, myPosition.myY, myPosition.myZ, 1) * anObjectMatrix;
		Vector3f pos = Vector3f(position.myX, position.myY, position.myZ);


		if (CU::Intersection::RayVsSphere(aRayDirection, aRayPosition, pos, myRadius * anObjectScale) == true)
		{
			if (aCheckOnlyRoot == true)
			{
				return true;
			}

			bool toReturn = false;
			for (unsigned short i = 0; i < myColliders.Size(); i++)
			{
				toReturn = myColliders[i]->CheckCollision(aRayDirection, aRayPosition, anObjectMatrix, anObjectScale);
				if (toReturn == true)
				{
					return true;
				}
			}

			if (myColliders.Size() == 0)
			{
				toReturn = true;
			}
			return toReturn;
		}
		return false;
	}

	bool ColliderCircle::CheckCollision(const Vector3<float>& aRayDirection, const Vector3<float>& aRayPosition, const Matrix44f& anObjectMatrix, const float anObjectScale, Vector3f& aTargetPosition, bool aCheckOnlyRoot) const
	{
		Vector4f position = Vector4f(myPosition.myX, myPosition.myY, myPosition.myZ, 1) * anObjectMatrix;
		Vector3f pos = Vector3f(position.myX, position.myY, position.myZ);

		if (CU::Intersection::RayVsSphere(aRayDirection, aRayPosition, pos, myRadius * anObjectScale) == true)
		{
			if (aCheckOnlyRoot == true)
			{
				return true;
			}

			bool toReturn = false;
			for (unsigned short i = 0; i < myColliders.Size(); i++)
			{
				toReturn = myColliders[i]->CheckCollision(aRayDirection, aRayPosition, anObjectMatrix, anObjectScale, aTargetPosition);
				if (toReturn == true)
				{
					return true;
				}
			}

			if (myColliders.Size() == 0)
			{
				aTargetPosition = pos;
				toReturn = true;
			}
			return toReturn;
		}
		return false;
	}

	const float ColliderCircle::GetRadius() const
	{
		return myRadius;
	}

	const float ColliderCircle::GetRadius(const float& aScale) const
	{
		return myRadius * aScale;
	}

	const Position ColliderCircle::GetPosition() const
	{
		return myPosition;
	}

	void ColliderCircle::Render(const Matrix44f& aSpace) const
	{
		CU::PoolPointer<GE::Model> model = GE::GfxFactoryWrapper::GetInstance()->GetModelFactory()->GetModel("Sphere");

		Matrix44f circleSpace;
		circleSpace.SetTranslation(myPosition);

		Matrix44f instance = circleSpace * aSpace;
		float radius = myRadius;
		instance.Scale({ radius, radius, radius });

		//CU::EventManager::GetInstance()->AddRenderCommand(CU::RenderCommandInstance(&*model, instance, instance));
	}

	void ColliderCircle::RenderChildren(const Matrix44f& aSpace) const
	{
		for (unsigned short i = 0; i < myColliders.Size(); i++)
		{
			myColliders[i]->Render(aSpace);
		}
	}

}
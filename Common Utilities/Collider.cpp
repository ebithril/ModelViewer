#include "Common Utilities_Precompiled.h"
#include "Collider.h"
#include "ColliderCircle.h"

namespace CommonUtilities
{
	Collider::Collider()
	{
		myColliders.Init(2);
	}


	Collider::~Collider()
	{
	}

	const ColliderCircle* Collider::GetAsColliderCircle() const
	{
		return reinterpret_cast<const ColliderCircle*>(this);
	}
}
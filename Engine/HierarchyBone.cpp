#include "Engine_Precompiled.h"
#include "HierarchyBone.h"

#include "AnimationNode.h"

namespace GraphicsEngine
{
	HierarchyBone::HierarchyBone()
	{
	}

	HierarchyBone::~HierarchyBone()
	{
	}

	void HierarchyBone::Update(const Matrix44<float>& aParentMatrix, float aCurrentTime)
	{
		Matrix44<float> calculatedMatrix = aParentMatrix;

		if (myResultMatrix)
		{
			if (myCurrentAnimation != nullptr && myCurrentAnimation->HasValues()
				&& myCurrentAnimation->GetAnimationLenght() > 0.f)
			{
				calculatedMatrix = myCurrentAnimation->GetCurrentMatrix(aCurrentTime) * aParentMatrix;
			}
			else
			{
				calculatedMatrix = (*myBoneMatrix) * aParentMatrix;
			}

			(*myResultMatrix) = calculatedMatrix;
		}

		for (unsigned short i = 0; i < myChildren.Size(); ++i)
		{
			myChildren[i].Update(calculatedMatrix, aCurrentTime);
		}
	}
}
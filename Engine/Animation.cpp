#include "Engine_Precompiled.h"
#include "Animation.h"

#include "AnimationNode.h"

namespace GraphicsEngine
{
	Animation::Animation() : myAnimationData(4), myBoneNames(4)
	{
		for (unsigned short i = 0; i < MAX_NR_OF_BONES; i++)
		{
			myCalculatedBones[i] = Matrix44<float>();
		}
	}

	Animation::~Animation()
	{
	}

	void Animation::Update(float aCurrentTime, CU::StaticArray<Matrix44<float>, MAX_NR_OF_BONES>& aApplyArray)
	{
		if (aCurrentTime > myAnimationLenght)
		{
			aCurrentTime -= myAnimationLenght * floor(aCurrentTime / myAnimationLenght);
		}

		myHierarchy.Update(Matrix44<float>(), aCurrentTime);

		for (unsigned short i = 0; i < myBoneNames.Size(); ++i)
		{
			myCalculatedBones[i] = myBindPoses[i] * myCalculatedBones[i];

			aApplyArray[i] = myCalculatedBones[i];
		}
	}

	void Animation::AddAnimation(AnimationNode* aNode)
	{
		myAnimationData.Add(aNode);
	}

	void Animation::SetBoneMatrix(const Matrix44<float>& aMatrix, int aIndex)
	{
		myBones[aIndex] = aMatrix;
	}

	void Animation::SetBoneBindPose(const Matrix44<float>& aMatrix, int aIndex)
	{
		myBindPoses[aIndex] = aMatrix;
	}

	void Animation::SetBindMatrix(const Matrix44<float>& aMatrix)
	{
		myBindMatrix = aMatrix;
	}

	void Animation::AddBoneName(const std::string& aName)
	{
		myBoneNames.Add(aName);
	}

	void Animation::SetHierarchy(const HierarchyBone& aBone)
	{
		myHierarchy = aBone;
		SetBoneData(myHierarchy);
	}

	void Animation::SetBoneData(HierarchyBone& aBone)
	{
		if (aBone.myBoneID == -1)
		{
			for (unsigned short i = 0; i < myBoneNames.Size(); ++i)
			{
				if (myBoneNames[i] == aBone.myBoneName)
				{
					aBone.myBoneID = i;
				}
			}
		}

		for (unsigned short i = 0; i < myAnimationData.Size(); ++i)
		{
			if (myAnimationData[i]->myBoneName == aBone.myBoneName)
			{
				aBone.myCurrentAnimation = myAnimationData[i];
			}
		}

		if (aBone.myBoneID != -1)
		{
			aBone.myBoneMatrix = &myBones[aBone.myBoneID];
			aBone.myResultMatrix = &myCalculatedBones[aBone.myBoneID];
		}

		for (unsigned short i = 0; i < aBone.myChildren.Size(); ++i)
		{
			SetBoneData(aBone.myChildren[i]);
		}
	}
}
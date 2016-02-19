#pragma once

#include "AnimationDefines.h"
#include "HierarchyBone.h"
#include "../Common Utilities/Matrix44.h"
#include "../Common Utilities/StaticArray.h"

namespace GraphicsEngine
{
	class AnimationNode;

	class Animation
	{
	public:
		Animation();
		~Animation();

		void Update(float aCurrentTime, CU::StaticArray<Matrix44<float>, MAX_NR_OF_BONES>& aApplyArray);

		void AddAnimation(AnimationNode* aNode);
		void SetBoneMatrix(const Matrix44<float>& aMatrix, int aIndex);
		void SetBoneBindPose(const Matrix44<float>& aMatrix, int aIndex);
		void SetBindMatrix(const Matrix44<float>& aMatrix);
		void AddBoneName(const std::string& aName);
		void SetHierarchy(const HierarchyBone& aBone);

		inline float GetAnimationLenght() const;
		inline void SetAnimationLenght(float aLenght);

	private:
		void SetBoneData(HierarchyBone& aBone);

		float myAnimationLenght;
		HierarchyBone myHierarchy;

		CU::GrowingArray<std::string> myBoneNames;
		Matrix44<float> myBindMatrix;
		CU::StaticArray<Matrix44<float>, MAX_NR_OF_BONES> myBones;
		CU::StaticArray<Matrix44<float>, MAX_NR_OF_BONES> myBindPoses;
		CU::StaticArray<Matrix44<float>, MAX_NR_OF_BONES> myCalculatedBones;
		CU::GrowingArray<AnimationNode*> myAnimationData;
	};

	float Animation::GetAnimationLenght() const
	{
		return myAnimationLenght;
	}

	void Animation::SetAnimationLenght(float aLenght)
	{
		myAnimationLenght = aLenght;
	}
}

namespace GE = GraphicsEngine;
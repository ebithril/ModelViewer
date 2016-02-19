#pragma once

#include "AnimationDefines.h"

namespace GraphicsEngine
{
	class AnimationNode;

	class HierarchyBone
	{
		friend class Animation;
		friend class FBXFactory;
	public:
		HierarchyBone();
		~HierarchyBone();

		void Update(const Matrix44<float>& aParentMatrix, float aCurrentTime);
	private:
		Matrix44<float>* myResultMatrix;
		Matrix44<float>* myBoneMatrix;
		AnimationNode* myCurrentAnimation;
		CU::GrowingArray<HierarchyBone> myChildren;
		int myBoneID;
		std::string myBoneName;
	};
}
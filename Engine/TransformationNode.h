#pragma once

#include "../Common Utilities/Matrix44.h"

namespace GraphicsEngine
{
	class TransformationNode
	{
	public:
		TransformationNode();
		TransformationNode(const Matrix44<float> aOrientation);
		~TransformationNode();

		virtual const Matrix44<float> GetTransformationForTime(float aTime);
		void SetOrientation(const Matrix44<float>& aOrientation);
		virtual float GetEndTime();
	protected:
		Matrix44<float> myOrientation;
	};
}
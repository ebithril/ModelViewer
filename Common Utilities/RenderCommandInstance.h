#pragma once
#include "Matrix.h"
#include "PoolPointer.h"
#include "VectorOnStack.h"
#include <memory>
#include <string>

namespace GraphicsEngine
{
	class Model;
	class Camera;
	class Light;
	class Texture;
	class Instance;
}

namespace CommonUtilities
{
	struct RenderCommandInstance
	{
	public:
		RenderCommandInstance(GraphicsEngine::Instance* anInstance, const Matrix44f& anOrientation, const Matrix44f& aLastFrameOrientation, 
			const VectorOnStack<unsigned short, 16>& someLightIndexes , const Vector4f& aDebugColor = { -1, -1, -1, -1 },
			GraphicsEngine::Texture* aDiffuseTexture = nullptr, GraphicsEngine::Texture* anEmmisiveTexture = nullptr, const bool anAffectedByFow = false, const Vector2<float>& aCutOffValue = { 1, 1 });
		RenderCommandInstance();
		~RenderCommandInstance();

		Matrix44f myOrientation;
		Matrix44f myLastFrameOrientation;
		Vector4f myDebugColor;
		GraphicsEngine::Texture* myDiffuseTexture;
		GraphicsEngine::Texture* myEmmisiveTexture;
		GraphicsEngine::Instance* myModel;
		VectorOnStack<unsigned short, 16> myLightIndexes;
		bool myAffectedByFow;
		Vector2<float> myCutOffValue;
	};

}

namespace CU = CommonUtilities;



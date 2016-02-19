#include "Common Utilities_Precompiled.h"
#include "RenderCommandInstance.h"

namespace CommonUtilities
{
	RenderCommandInstance::RenderCommandInstance()
	{
		myModel = nullptr;
		myDebugColor = {-1, -1, -1, -1};
	}

	RenderCommandInstance::RenderCommandInstance(GraphicsEngine::Instance* anInstance, const Matrix44f& anOrientation, const Matrix44f& aLastFrameOrientation,
		const VectorOnStack<unsigned short, 16>& someLightIndexes, const Vector4f& aDebugColor,
		GraphicsEngine::Texture* aDiffuseTexture, GraphicsEngine::Texture* anEmmisiveTexture , const bool anAffectedByFow, const Vector2<float>& aCutOffValue)
	{
		myDebugColor = aDebugColor;
		myModel = anInstance;
		myOrientation = anOrientation;
		myLastFrameOrientation = aLastFrameOrientation;
		myDiffuseTexture = aDiffuseTexture;
		myEmmisiveTexture = anEmmisiveTexture;
		myAffectedByFow = anAffectedByFow;
		myLightIndexes = someLightIndexes;
		myCutOffValue = aCutOffValue;
	}

	RenderCommandInstance::~RenderCommandInstance()
	{

	}
}
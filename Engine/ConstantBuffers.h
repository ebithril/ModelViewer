#pragma once
#include "../Common Utilities/Vector.h"
#include "../Common Utilities/Matrix.h"
#include <DirectXMath.h>
#include "TextureType.h"
#include "COMObjectPointer.h"
#include "LightShaderData.h"

class ID3D11ShadeResourceView;

#define GRAPHICS_ENGINE_NUMBER_OF_POINT_LIGHTS 6
#define GRAPHICS_ENGINE_NUMBER_OF_SPOT_LIGHTS 6
#define GRAPHICS_ENGINE_NUMBER_OF_SPOT_AND_POINT_LIGHTS 8


namespace GraphicsEngine
{
	struct PerObjectDataBuffer
	{
		Matrix44f myToWorldMatrix;
		Matrix44f myWorldViewProjectionMatrix;
		Matrix44f myLastFrameToWorldMatrix;
		Vector4f myDebugColor;
		Matrix44f myLastFrameWorldViewProjectionMatrix;
		Vector4f myCutOffValue;
	};

	struct PerFrameCameraBuffer
	{
		Matrix44f myViewMatrix;
		Vector3f myCameraPosition;
		float myTime;
		Vector3f myViewDirection;
		float myCameraPadding;
	};

	struct PerFrameLightBuffer
	{
		Vector4f myLightColor;
		Vector3f myLightDirection;
		float myTrash;
		Matrix44f myLightDirectionShadowViewProjection;
	};

	struct PerObjectLightBuffer
	{
		LightShaderData myData[GRAPHICS_ENGINE_NUMBER_OF_SPOT_AND_POINT_LIGHTS];
	};

	struct PerObjectPointLightBuffer
	{
		Vector4f myColor[GRAPHICS_ENGINE_NUMBER_OF_POINT_LIGHTS];
		Vector4f myPositionAndRange[GRAPHICS_ENGINE_NUMBER_OF_POINT_LIGHTS];
	};

	struct PerObjectSpotLightBuffer
	{
		Vector4f myColor[GRAPHICS_ENGINE_NUMBER_OF_SPOT_LIGHTS];
		Vector4f myPositionAndRange[GRAPHICS_ENGINE_NUMBER_OF_SPOT_LIGHTS];
		Vector4f myDirectionAndAngle[GRAPHICS_ENGINE_NUMBER_OF_SPOT_LIGHTS];
		Matrix44f myDepthBiasViewProjection[GRAPHICS_ENGINE_NUMBER_OF_SPOT_LIGHTS];

	};

	struct PerSpotLightShadowBuffer
	{
		Matrix44<float> myViewProjectionMatrix;
	};

	struct PerObjectShadowBuffer
	{
		Matrix44f myOrientation;
	};

	struct OnResizeBuffer
	{
		Matrix44f myProjectionMatrix;
	};

	struct PerSurfaceTextureBuffer
	{
		COMObjectPointer<ID3D11ShadeResourceView> myTextures[static_cast<unsigned int>(eTextureType::NR_OF_TEXTURETYPES)];
	};
}



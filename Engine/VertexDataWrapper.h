#pragma once
#include "../Common Utilities/Vector.h"
#include "COMObjectPointer.h"
#include "../Common Utilities/SharedPointer.h"

struct D3D11_INPUT_ELEMENT_DESC;

namespace GraphicsEngine
{

	struct VertexPosCol
	{
		Vector3<float> myPosition;
		Vector4<float> myColor;
	};

	struct VertexPosColUV
	{
		Vector3<float> myPosition;
		Vector3<float> myColor;
		Vector2<float> myUV;
	};

	struct VertexPosNormUV
	{
		Vector3<float> myPosition;
		Vector2<float> myUV;
		Vector3<float> myNormal;
	};

	struct VertexPosNormUVTan
	{
		Vector3<float> myPosition;
		Vector3<float> myNormal;
		Vector2<float> myUV;
		Vector3<float> myTangent;
	};

	struct  VertexPosUV
	{
		Vector3<float> myPosition;
		Vector3<float> myUV;
	};

	struct VertexPosWeightBonesNormUVTan
	{
		Vector3<float> myPosition;
		Vector3<float> myNormal;
		Vector2<float> myUV;
		Vector3<float> myTangent;
	};

	struct VertexPosNormalUVBiNormalTan
	{
		Vector3<float> myPosition;
		float myPadding1;
		Vector3<float> myNormal;
		float myPadding2;
		Vector2<float> myUV;
		Vector2f myPadding3;
		Vector3<float> myBinormal;
		float myPadding4;
		Vector3<float> myTangent;
		float myPadding5;
	};

	struct VertexPosVelocitySizeAlphaTime
	{
		Vector3f myPosition;
		float mySize;
		Vector3f myVelocity;
		float myAlpha;
		float myTotalTime;
	};

	enum eVertexType
	{
		NONE,
		VERTEX_POS,
		VERTEX_POS_COL,
		VERTEX_POS_COL_UV,
		VERTEX_POS_NORM_UV,
		VERTEX_POS_NORM_UV_TAN,
		VERTEX_POS_UV,
		VERTEX_POS_WEIGHT_BONES_NORM_UV_TAN,
		VERTEX_POS_NORM_UV_BI_NORMAL_TAN,
		VERTEX_POS_VELOCITY_SIZE_ALPHA_TIME,
		VERTEX_POS_SCALE_ALPHA_TOTALTIME,
		VERTEX_TERRAIN_POS_NORM_UV_BINORM_TAN,
		VERTEX_SPRITE_POS_COL_UV,
	};

	enum eVertexVariables
	{
		POSITION = 1,
		COLOR = 2,
		NORMAL = 4,
		TEXCOORD = 8,
		TANGENT = 16,
		BI_NORMAL = 32
	};

	struct VertexDataWrapper
	{
	public:
		VertexDataWrapper();
		~VertexDataWrapper();

		VertexPosColUV* GetDataAsVertexPosColUV();
		VertexPosNormUV* GetDataAsVertexPosNormUV();
		VertexPosNormUVTan* GetDataAsVertexPosNormUVTan();
		VertexPosUV* GetDataAsVertexPosUV();
		VertexPosWeightBonesNormUVTan* GetDataAsVertexPosWeightBonesNormUVTan();
		VertexPosNormalUVBiNormalTan* GetDataAsVertexPosNormUVBiNormTan();
		VertexPosVelocitySizeAlphaTime* GetDataAsVertexPosVelocitySizeAlphaTime();

		int myNumberOfVertexes;
		int mySize;
		int myStride;
		eVertexType myType;
		SharedPointer<char> myVertexData;
		
		void SetVertexType(const char someVertexVariables);

	};
}

namespace GE = GraphicsEngine;

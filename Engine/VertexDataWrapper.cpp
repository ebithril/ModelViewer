#include "Engine_Precompiled.h"
#include "VertexDataWrapper.h"
#include <d3d11_2.h>

namespace GraphicsEngine
{

	VertexDataWrapper::VertexDataWrapper()
	{
	}


	VertexDataWrapper::~VertexDataWrapper()
	{
	}

	VertexPosColUV* VertexDataWrapper::GetDataAsVertexPosColUV()
	{
		return reinterpret_cast<VertexPosColUV*>(myVertexData.get());
	}

	VertexPosNormUV* VertexDataWrapper::GetDataAsVertexPosNormUV()
	{
		return reinterpret_cast<VertexPosNormUV*>(myVertexData.get());
	}

	VertexPosNormUVTan* VertexDataWrapper::GetDataAsVertexPosNormUVTan()
	{
		return reinterpret_cast<VertexPosNormUVTan*>(myVertexData.get());
	}

	VertexPosUV* VertexDataWrapper::GetDataAsVertexPosUV()
	{
		return reinterpret_cast<VertexPosUV*>(myVertexData.get());
	}

	VertexPosWeightBonesNormUVTan* VertexDataWrapper::GetDataAsVertexPosWeightBonesNormUVTan()
	{
		return reinterpret_cast<VertexPosWeightBonesNormUVTan*>(myVertexData.get());
	}

	VertexPosNormalUVBiNormalTan* VertexDataWrapper::GetDataAsVertexPosNormUVBiNormTan()
	{
		return reinterpret_cast<VertexPosNormalUVBiNormalTan*>(myVertexData.get());
	}

	VertexPosVelocitySizeAlphaTime* VertexDataWrapper::GetDataAsVertexPosVelocitySizeAlphaTime()
	{
		return reinterpret_cast<VertexPosVelocitySizeAlphaTime*>(myVertexData.get());

	}

	void VertexDataWrapper::SetVertexType(const char someVertexVariables)
	{
		switch (someVertexVariables)
		{
		case eVertexVariables::POSITION + eVertexVariables::COLOR:
			myType = eVertexType::VERTEX_POS_COL;
			break;

		case eVertexVariables::POSITION + eVertexVariables::COLOR + eVertexVariables::TEXCOORD:
			myType = eVertexType::VERTEX_POS_COL_UV;
			break;

		case eVertexVariables::POSITION + eVertexVariables::NORMAL + eVertexVariables::TEXCOORD:
			myType = eVertexType::VERTEX_POS_NORM_UV;
			break;
		case eVertexVariables::POSITION + eVertexVariables::NORMAL + eVertexVariables::TEXCOORD + eVertexVariables::BI_NORMAL + eVertexVariables::TANGENT:
			myType = eVertexType::VERTEX_POS_NORM_UV_BI_NORMAL_TAN;
			break;

		case eVertexVariables::POSITION + eVertexVariables::NORMAL + eVertexVariables::TEXCOORD + eVertexVariables::TANGENT:
			myType = eVertexType::VERTEX_POS_NORM_UV_TAN;
			break;

		case eVertexVariables::POSITION + eVertexVariables::TEXCOORD:
			myType = eVertexType::VERTEX_POS_UV;
			break;

			/*case  eVertexVariables::POSITION + eVertexVariables::NORMAL + eVertexVariables::TEXCOORD + eVertexVariables:: + eVertexVariables::TANGENT:
				myType = eVertexType::VERTEX_POS_WEIGHT_BONES_NORM_UV_TAN;
				break;*/

		default:
			DL_ASSERT("Unknown vertexformat");
			break;
		}
	}

}

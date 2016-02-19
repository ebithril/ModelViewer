#pragma once
#include "../Common Utilities/GrowingArray.h"
	
struct D3D11_INPUT_ELEMENT_DESC;

namespace GraphicsEngine
{
	struct VertexIndexWrapper;
	struct VertexDataWrapper;

	struct ModelDataStruct
	{
		CU::GrowingArray<D3D11_INPUT_ELEMENT_DESC*> myInputDescription;
		VertexIndexWrapper* myIndexData;
		VertexDataWrapper* myVertexData;
		CU::GrowingArray<Surface> mySurfaces;
	};
}

namespace GE = GraphicsEngine;
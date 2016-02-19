#pragma once

#include "../Common Utilities/SharedPointer.h"

enum DXGI_FORMAT;

namespace GraphicsEngine
{
	struct VertexIndexWrapper
	{
	public:
		VertexIndexWrapper();
		~VertexIndexWrapper();

		DXGI_FORMAT myFormat;
		SharedPointer<char> myIndexData;
		int myNumberOfIndexes;
		int mySize;
	};
}

namespace GE = GraphicsEngine;
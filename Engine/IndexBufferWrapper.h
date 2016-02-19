#pragma once

#include "COMObjectPointer.h"
#include "../Common Utilities/SharedPointer.h"

struct ID3D11Buffer;
struct ID3D11Device;
enum DXGI_FORMAT;

namespace GraphicsEngine
{
	struct VertexIndexWrapper;
	struct IndexBufferWrapper
	{
	public:
		int myByteOffset;
		COMObjectPointer<ID3D11Buffer> myIndexBuffer;
		DXGI_FORMAT myIndexBufferFormat;
		unsigned int myIndexCount;
		SharedPointer<char> myData;
	
		void Create(COMObjectPointer<ID3D11Device>& aDevice, VertexIndexWrapper* someIndexData);

		IndexBufferWrapper();
		~IndexBufferWrapper();
		void UnLoad();
	private:
	
	};
}

namespace GE = GraphicsEngine;

#pragma once

#include "COMObjectPointer.h"
#include "../Common Utilities/SharedPointer.h"

struct ID3D11Device;
struct ID3D11Buffer;

namespace GraphicsEngine
{
	struct VertexDataWrapper;

	struct DynamicVertexBufferWrapper
	{
	public:
		unsigned int myByteOffset;
		int myNumberOfBuffers;
		int myStartSlot;
		unsigned int myStride;
		COMObjectPointer<ID3D11Buffer> myVertexBuffer;

		void Create(COMObjectPointer<ID3D11Device>& aDevice, VertexDataWrapper* someVertexData);

		DynamicVertexBufferWrapper();
		~DynamicVertexBufferWrapper();

	private:

	};
}
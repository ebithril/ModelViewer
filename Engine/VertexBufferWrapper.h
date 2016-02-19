#pragma once

#include "COMObjectPointer.h"
#include "../Common Utilities/SharedPointer.h"


struct ID3D11Device;
struct ID3D11Buffer;

namespace GraphicsEngine
{

	struct VertexDataWrapper;

	struct VertexBufferWrapper
	{
	public:
		unsigned int myByteOffset;
		int myNumberOfBuffers;
		int myStartSlot;
		unsigned int myStride;
		COMObjectPointer<ID3D11Buffer> myVertexBuffer;
		SharedPointer<char> myData;

		void Create(COMObjectPointer<ID3D11Device>& aDevice, VertexDataWrapper* someVertexData);
		void CreateDynamic(COMObjectPointer<ID3D11Device>& aDevice, VertexDataWrapper* someVertexData);

		VertexBufferWrapper();
		~VertexBufferWrapper();

		void UnLoad();

	private:

	};
}
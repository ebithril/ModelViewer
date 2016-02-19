#include "Engine_Precompiled.h"
#include "DynamicVertexBufferWrapper.h"
#include "VertexDataWrapper.h"
#include <d3d11_2.h>
#include "WindowsFunctions.h"

namespace GraphicsEngine
{
	DynamicVertexBufferWrapper::DynamicVertexBufferWrapper()
	{
	}


	DynamicVertexBufferWrapper::~DynamicVertexBufferWrapper()
	{
	}

	void DynamicVertexBufferWrapper::Create(COMObjectPointer<ID3D11Device>& aDevice, VertexDataWrapper* someVertexData)
	{
		myStride = someVertexData->myStride;
		myStartSlot = 0;
		myByteOffset = 0;
		myNumberOfBuffers = 1;

		D3D11_BUFFER_DESC vertexBufferDescription = { 0 };
		vertexBufferDescription.ByteWidth = someVertexData->mySize;
		vertexBufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vertexBufferDescription.Usage = D3D11_USAGE_DYNAMIC;

		HRESULT result;

		result = aDevice->CreateBuffer(&vertexBufferDescription, NULL, &myVertexBuffer);

		if (FAILED(result))
		{
			WF::AssertComAndWindowsError(result, "Vertex buffer failed to create.");
		}
	}
}
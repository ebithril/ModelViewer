#include "Engine_Precompiled.h"
#include "VertexBufferWrapper.h"
#include "VertexDataWrapper.h"
#include "WindowsFunctions.h"
#include <d3d11_2.h>

namespace GraphicsEngine
{

	void VertexBufferWrapper::Create(COMObjectPointer<ID3D11Device>& aDevice, VertexDataWrapper* someVertexData)
	{
		myStride = someVertexData->myStride;
		myStartSlot = 0;
		myByteOffset = 0;
		myNumberOfBuffers = 1;

		D3D11_BUFFER_DESC vertexBufferDescription = { 0 };
		vertexBufferDescription.ByteWidth = someVertexData->mySize;
		vertexBufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDescription.CPUAccessFlags = 0;
		vertexBufferDescription.Usage = D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA vertexSubResource = { someVertexData->myVertexData.get(), 0, 0 };

		HRESULT result;

		result = aDevice->CreateBuffer(&vertexBufferDescription, &vertexSubResource, &myVertexBuffer);

		if (FAILED(result))
		{
			WF::AssertComAndWindowsError(result, "Vertex buffer failed to create.");
		}

		myData = someVertexData->myVertexData;
	}

	void VertexBufferWrapper::CreateDynamic(COMObjectPointer<ID3D11Device>& aDevice, VertexDataWrapper* someVertexData)
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

		D3D11_SUBRESOURCE_DATA vertexSubResource = { someVertexData->myVertexData.get(), 0, 0 };

		HRESULT result;

		result = aDevice->CreateBuffer(&vertexBufferDescription, &vertexSubResource, &myVertexBuffer);

		if (FAILED(result))
		{
			WF::AssertComAndWindowsError(result, "Vertex buffer failed to create.");
		}

		myData = someVertexData->myVertexData;
	}

	VertexBufferWrapper::VertexBufferWrapper()
	{
		myVertexBuffer = nullptr;
		myData = nullptr;
	}


	VertexBufferWrapper::~VertexBufferWrapper()
	{
	}

	void VertexBufferWrapper::UnLoad()
	{
		if (myVertexBuffer.Get() != nullptr)
		{
			//myVertexBuffer->Release();
			myVertexBuffer = nullptr;

		}
		if (myData != nullptr)
		{
			//myData.reset();
			myData = nullptr;
		}
	}

}


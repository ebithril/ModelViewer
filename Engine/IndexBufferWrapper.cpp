#include "Engine_Precompiled.h"
#include "IndexBufferWrapper.h"
#include "VertexIndexWrapper.h"
#include <d3d11_2.h>
#include "WindowsFunctions.h"

namespace GraphicsEngine
{

	void IndexBufferWrapper::Create(COMObjectPointer<ID3D11Device>& aDevice, VertexIndexWrapper* someIndexData)
	{
		myByteOffset = 0;
		myIndexBufferFormat = someIndexData->myFormat;
		myIndexCount = someIndexData->myNumberOfIndexes;

		D3D11_BUFFER_DESC indexBufferDesc{ 0 };

		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.ByteWidth = someIndexData->mySize;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA indiceSubResource = { someIndexData->myIndexData.get(), 0, 0 };

		HRESULT result;
		result = aDevice->CreateBuffer(&indexBufferDesc, &indiceSubResource, &myIndexBuffer);

		if (FAILED(result) == true)
		{
			WF::AssertComAndWindowsError(result, "Index buffer failed to create.");
		}

		myData = someIndexData->myIndexData;
	}

	IndexBufferWrapper::IndexBufferWrapper()
	{
		myIndexBuffer = nullptr;
		myData = nullptr;
	}

	IndexBufferWrapper::~IndexBufferWrapper()
	{
	}

	void IndexBufferWrapper::UnLoad()
	{
		if (myData != nullptr)
		{
			//myData.reset();
			myData = nullptr;
		}

		if (myIndexBuffer.Get() != nullptr)
		{
			//myIndexBuffer->Release();
			myIndexBuffer = nullptr;
		}
	}
}


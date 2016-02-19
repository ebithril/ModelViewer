#include "Engine_Precompiled.h"
#include "Texture.h"
#include "DDSTextureLoader.h"
#include "WindowsFunctions.h"
#include <d3d11_2.h>
#include "TextureType.h"
#include "DirectXEngine.h"

namespace GraphicsEngine
{
	Texture::Texture(DirectXEngine* anEngine, const Vector2<int>& aResolution, DXGI_FORMAT aFormat, bool aShaderResource, bool aRenderTarget, bool asUnorderdAccessView, bool aDepthStencilView, bool asACubeMap) :Texture()
	{
		for (int i = 0; i < 6; i++)
		{
			myRenderTargets[i] = nullptr;
		}

		myImageSize = aResolution;

		D3D11_TEXTURE2D_DESC texDesc;
		texDesc.Width = aResolution.myX;
		texDesc.Height = aResolution.myY;
		texDesc.MipLevels = 1;
		if (asACubeMap == true)
		{
			texDesc.ArraySize = 6;
			texDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
		}
		else
		{
			texDesc.ArraySize = 1;
			texDesc.MiscFlags = 0;
		}
		texDesc.Format = aFormat;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = 0;
		if (aShaderResource == true)
		{
			texDesc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
		}
		if (aRenderTarget == true)
		{
			texDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
		}
		if (asUnorderdAccessView == true)
		{
			texDesc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;
		}
		if (aDepthStencilView == true)
		{
			texDesc.BindFlags |= D3D11_BIND_DEPTH_STENCIL;
		}

		texDesc.CPUAccessFlags = 0;
		


		ID3D11Texture2D* texture;

		HRESULT hr;

		hr = anEngine->GetDevice()->CreateTexture2D(&texDesc, 0, &texture);
		if (FAILED(hr))
		{
			WF::AssertComAndWindowsError(hr, "Failed to create Texture");
		}

		if (aRenderTarget == true)
		{

			D3D11_RENDER_TARGET_VIEW_DESC RTDesc;

			RTDesc.Format = texDesc.Format;
			if (asACubeMap == false)
			{
				RTDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
				RTDesc.Texture2D.MipSlice = 0;
				hr = anEngine->GetDevice()->CreateRenderTargetView(texture, &RTDesc, &myRenderTargets[0]);
				if (FAILED(hr))
				{
					WF::AssertComAndWindowsError(hr, "Failed to create RenderTargetView");
				}
			}
			else
			{
				RTDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
				RTDesc.Texture2DArray.ArraySize = 1;
				RTDesc.Texture2DArray.FirstArraySlice = 0;
				RTDesc.Texture2DArray.MipSlice = 0;

				for (int i = 0; i < 6; i++)
				{
					RTDesc.Texture2DArray.FirstArraySlice = static_cast<unsigned int>(i);

					hr = anEngine->GetDevice()->CreateRenderTargetView(texture, &RTDesc, &myRenderTargets[i]);
					if (FAILED(hr))
					{
						WF::AssertComAndWindowsError(hr, "Failed to create RenderTargetView");
					}
				}
			}



		}

		if (aShaderResource == true)
		{

			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

			if (texDesc.Format == DXGI_FORMAT::DXGI_FORMAT_R32_TYPELESS)
			{
				srvDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
			}
			else
			{
				srvDesc.Format = texDesc.Format;
			}
			if (asACubeMap == true)
			{
				srvDesc.ViewDimension = D3D10_1_SRV_DIMENSION_TEXTURECUBE;
				srvDesc.TextureCube.MipLevels = 1;
				srvDesc.TextureCube.MostDetailedMip = 0;
			}
			else
			{
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				srvDesc.Texture2D.MostDetailedMip = 0;
				srvDesc.Texture2D.MipLevels = 1;
			}
			hr = anEngine->GetDevice()->CreateShaderResourceView(texture, &srvDesc, &myTexture);
			if (FAILED(hr))
			{
				WF::AssertComAndWindowsError(hr, "Failed to create ShaderResourceView");
			}
		}
		else
		{
			myTexture = nullptr;
		}

		if (asUnorderdAccessView == true)
		{
			D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
			uavDesc.Format = texDesc.Format;
			uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
			uavDesc.Texture2D.MipSlice = 0;
			hr = anEngine->GetDevice()->CreateUnorderedAccessView(texture, &uavDesc, &myUnorderedAccessView);
			if (FAILED(hr))
			{
				WF::AssertComAndWindowsError(hr, "Failed to create UnorderedAccessView");
			}
		}
		else
		{
			myUnorderedAccessView = nullptr;
		}

		if (aDepthStencilView == true)
		{
			D3D11_DEPTH_STENCIL_VIEW_DESC description;

			if (texDesc.Format == DXGI_FORMAT::DXGI_FORMAT_R32_TYPELESS)
			{
				description.Format = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
			}
			else
			{
				description.Format = aFormat;
			}


			description.Flags = 0;
			description.ViewDimension = D3D11_DSV_DIMENSION::D3D11_DSV_DIMENSION_TEXTURE2D;
			description.Texture2D.MipSlice = 0;
			HRESULT result = anEngine->GetDevice()->CreateDepthStencilView(texture, &description, &myDepthStencilView);
			if (FAILED(result) == true)
			{
				WF::AssertComAndWindowsError(result, "Failed to create DepthStencilView");
			}
		}
		else
		{
			myDepthStencilView = nullptr;
		}

		hr = texture->QueryInterface<IDXGISurface>(&mySurface);

		texture->Release();
	}

	void Texture::SetShaderResourceView(ID3D11ShaderResourceView* aResourceView, const Vector2<int>& aResolution)
	{
		myImageSize = aResolution;
		myTexture = aResourceView;
	}

	void Texture::SetRenderTargetView(ID3D11RenderTargetView* aRenderTargetView, const Vector2<int>& aResolution)
	{
		myImageSize = aResolution;
		myRenderTargets[0] = aRenderTargetView;
	}

	COMObjectPointer<IDXGISurface>& Texture::GetSurface()
	{
		return mySurface;
	}


	COMObjectPointer<ID3D11RenderTargetView>& Texture::GetRenderTargetView()
	{
		return myRenderTargets[0];
	}

	COMObjectPointer<ID3D11RenderTargetView>& Texture::GetRenderTargetView(const int anIndex)
	{
		return myRenderTargets[anIndex];
	}

	COMObjectPointer<ID3D11UnorderedAccessView>& Texture::GetUnorderedAccessView()
	{
		return myUnorderedAccessView;
	}

	COMObjectPointer<ID3D11DepthStencilView>& Texture::GetDepthStencilView()
	{
		return myDepthStencilView;
	}

	const Vector2<int>& Texture::GetImageSize()
	{
		return myImageSize;
	}

	Texture::Texture()
	{
		for (int i = 0; i < 6; i++)
		{
			myRenderTargets[i] = nullptr;
		}
		myTexture = nullptr;
		myUnorderedAccessView = nullptr;
		myDepthStencilView = nullptr;
		myImageSize = Vector2<int>(0, 0);
	}

	Texture::Texture(const Texture& aTexture)
	{
		*this = aTexture;
	}

	Texture::~Texture()
	{
	}

	const std::string& Texture::GetFileName()
	{
		return myFileName;
	}

	const eTextureType Texture::GetTextureType()
	{
		return myTextureType;
	}

	COMObjectPointer<ID3D11ShaderResourceView>& Texture::GetShaderView()
	{
		return myTexture;
	}

	bool Texture::LoadTexture(const std::string& aFileName, const eTextureType aTextureType, COMObjectPointer<ID3D11Device>& aDevice)
	{
		myFileName = aFileName;
		myTextureType = aTextureType;
		HRESULT result;

		result = DirectX::CreateDDSTextureFromFile(aDevice.Get(), WF::WideStringConvertion(aFileName.c_str()).c_str(), nullptr, &myTexture);

		if (FAILED(result))
		{
			std::string errorText = "Failed to create dds texture with filepath: ";
			errorText += myFileName;

			WF::AssertComAndWindowsError(result, errorText);
		}

		ID3D11Texture2D *pTextureInterface = 0;
		ID3D11Resource *res;
		myTexture.Get()->GetResource(&res);
		res->QueryInterface<ID3D11Texture2D>(&pTextureInterface);
		D3D11_TEXTURE2D_DESC desc;
		pTextureInterface->GetDesc(&desc);

		myImageSize = Vector2<int>(desc.Width, desc.Height);

		pTextureInterface->Release();
		res->Release();

		if (FAILED(result))
		{
			std::string errorString = "Failed to create texture from filename: " + aFileName;
			WF::AssertComAndWindowsError(result, errorString.c_str());
		}

		return true;
	}
}
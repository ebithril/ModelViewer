#pragma once

#include <string>
#include "../Common Utilities/Vector.h"
#include "COMObjectPointer.h"

struct ID3D11ShaderResourceView;
struct ID3D11RenderTargetView;
struct ID3D11UnorderedAccessView;
struct ID3D11DepthStencilView;
struct ID3D11Device;
struct IDXGISurface;
enum DXGI_FORMAT;

namespace GraphicsEngine
{
	enum class eTextureType;
	class DirectXEngine;

	class Texture
	{
	public:
		Texture(DirectXEngine* anEngine, const Vector2<int>& aResolution, DXGI_FORMAT aFormat, bool aShaderResource, bool aRenderTarget, bool asUnorderdAccessView = false, bool aDepthStencilView = false, bool asACubeMap = false);
		Texture(const Texture& aTexture);

		void SetShaderResourceView(ID3D11ShaderResourceView* aResourceView, const Vector2<int>& aResolution);
		void SetRenderTargetView(ID3D11RenderTargetView* aRenderTargetView, const Vector2<int>& aResolution);

		Texture();
		~Texture();

		const Vector2<int>& GetImageSize();
		const std::string& GetFileName();
		const eTextureType GetTextureType();
		COMObjectPointer<ID3D11ShaderResourceView>& GetShaderView();
		COMObjectPointer<ID3D11RenderTargetView>& GetRenderTargetView();
		COMObjectPointer<ID3D11RenderTargetView>& GetRenderTargetView(const int anIndex);
		COMObjectPointer<ID3D11UnorderedAccessView>& GetUnorderedAccessView();
		COMObjectPointer<ID3D11DepthStencilView>& GetDepthStencilView();
		COMObjectPointer<IDXGISurface>& GetSurface();
		bool LoadTexture(const std::string& aFileName, const eTextureType aTextureType, COMObjectPointer<ID3D11Device>& aDevice);

	private:
		std::string myFileName;
		eTextureType myTextureType;
		COMObjectPointer<ID3D11ShaderResourceView> myTexture;
		CU::StaticArray<COMObjectPointer<ID3D11RenderTargetView>, 6> myRenderTargets;
		COMObjectPointer<ID3D11UnorderedAccessView> myUnorderedAccessView;
		COMObjectPointer<ID3D11DepthStencilView> myDepthStencilView;
		COMObjectPointer<IDXGISurface> mySurface;
		Vector2<int> myImageSize;
	};
}

namespace GE = GraphicsEngine;


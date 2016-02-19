#pragma once
#include <Windows.h>
#include <d3d11_2.h>
#include <d3d11.h>
#include "SetupInfo.h"
#include "../Common Utilities/Observer.h"
#include "COMObjectPointer.h"
#include "RenderTargetData.h"
#include "../Common Utilities/GrowingArray.h"
#include "ShadowManager.h"

namespace CommonUtilities
{
	struct RenderCommandFont;
}

namespace CU = CommonUtilities;

namespace GraphicsEngine
{
	class Scene;
	class DirectX2D;
	struct SceneRenderData;
	class Camera;
	class Renderer;
	class Texture;
	

	class DirectXEngine : public CU::Observer
	{
	public:

		COMObjectPointer<ID3D11DeviceContext>& GetContext();
		COMObjectPointer<ID3D11Device>& GetDevice();
		const SetupInfo& GetSetupInfo();

		void RenderScenes();
		void RenderScenes(Texture& aRenderTargetView, Texture& aDepthStencilView, bool aShouldRenderText);
		void ShutDown();

		bool Init(HWND& aHwnd, WNDPROC aWindowProc, const SetupInfo& aInfoArgument);
		void InitRenderer();

		Texture WriteTextOnTexture(Texture* aBackground, const CU::GrowingArray<CU::RenderCommandFont>& someFonts); //Not thread friendly
		HWND& GetWindowHandle();

		~DirectXEngine();
		DirectXEngine();
		
		ShadowManager& GetShadowManager();

	private:

		RenderTargetData mySwapChainRenderTarget;
		RenderTargetData mySceneRenderTarget;

		COMObjectPointer<ID3D11Device> my3DDevice;
		COMObjectPointer<ID3D11DeviceContext> myContext;
		COMObjectPointer<ID3D11Texture2D> myDepthBuffer;
		COMObjectPointer<ID3D11DepthStencilState> myDepthState;
		D3D_DRIVER_TYPE myDriverType;
		D3D_FEATURE_LEVEL myFeatureLevel;
		HWND myHWND;
		SetupInfo myInfoArgument;
		COMObjectPointer<IDXGISwapChain> mySwapChain;

		Renderer* myRenderer;

		DirectX2D* my2DEngine;

		bool HandleOnResizeEvent(const CU::PoolPointer<CU::Event>& anEvent);

		void OnResize(const Vector2<int>& aResolution);

		void Create2DEngine();

		bool D3DDeviceSetup();
		bool D3DSetup();
		bool D3DStencilBufferSetup();
		bool D3DSwapChainSetup();
		DXGI_SWAP_CHAIN_DESC CreateSwapChainDescription();
		bool D3DViewPortSetup(const Vector2<int>& aResolution);
		bool WindowSetup(WNDPROC aWindowProc, const SetupInfo& aInfoArugment);

		void DrawFonts(const CU::GrowingArray<CU::RenderCommandFont>& someFonts);
		void RenderTextOnTexture();

		void SwitchBuffers();

		bool DirectXInitiatedTest();	
		
	};
}

namespace GE = GraphicsEngine;


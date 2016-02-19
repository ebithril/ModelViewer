#include "Engine_Precompiled.h"
#include "DirectXEngine.h"
#include "SceneRenderData.h"
#include "DirectX2D.h"
#include "WindowsFunctions.h"
#include <comdef.h>
#include <limits.h>
#include "d3dx11effect.h"
#include "RenderTargetData.h"
#include "Renderer.h"
#include <d3d11.h>
#include <direct.h>
#include "TextureMerger.h"
#include "Texture.h"

#include "../Common Utilities/VTuneAPI.h"
#include "../Common Utilities/DL_Debug.h"
#include "../Common Utilities/GraphicsEnums.h"
#include "../Common Utilities/EventManager.h"

#pragma comment (lib, "d3d11.lib")

VTUNE_API_CREATE_DOMAIN(locFontRenderDomain, "FontRenderDomain")
VTUNE_API_CREATE_HANDLE("FontRender", locFontRenderTimeStamp)

VTUNE_API_CREATE_DOMAIN(locSceneRenderDomain, "SceneRenderDomain")
VTUNE_API_CREATE_HANDLE("SceneRender", locSceneRenderTimeStamp)

VTUNE_API_CREATE_DOMAIN(locSwapBackBufferDomain, "SwapBackBufferDomain")
VTUNE_API_CREATE_HANDLE("SwapBackBuffer", locSwapBackBufferTimeStamp)


namespace GraphicsEngine
{

	COMObjectPointer<ID3D11DeviceContext>& DirectXEngine::GetContext()
	{
		DirectXInitiatedTest();
		return myContext;
	}

	COMObjectPointer<ID3D11Device>& DirectXEngine::GetDevice()
	{
		DirectXInitiatedTest();

		return my3DDevice;
	}

	void DirectXEngine::OnResize(const Vector2<int>& aResolution)
	{
		myContext->OMSetRenderTargets(NULL, NULL, NULL);
		//mySwapChainRenderTarget.myRenderTarget->Release();
		//myDepthBuffer->Release();
		//mySwapChainRenderTarget.myDepthStencil->Release();
		//myDepthState->Release();
		myContext->Flush();
		D3DViewPortSetup(myInfoArgument.myResolution);
		HRESULT hr;
		hr = mySwapChain->ResizeBuffers(0, aResolution.x, aResolution.y, DXGI_FORMAT_UNKNOWN, 0);
		if (FAILED(hr))
		{
			DL_ASSERT("Failed to resize swapchain buffer.");
		}
		D3DStencilBufferSetup();


		COMObjectPointer<IDXGISurface> surface;

		HRESULT result = mySwapChain->GetBuffer(0, IID_PPV_ARGS(&surface));

		if (FAILED(result) == true)
		{
			DL_ASSERT("Failed to get swapchain buffer.");
		}

		my2DEngine->Recreate(myHWND, surface);

		myRenderer->OnResize(myInfoArgument.myResolution);
	}

	void DirectXEngine::ShutDown()
	{

	}

	//bool DirectXEngine::HandleEvent(const CU::Event& anEvent)
	//{
	//	switch (anEvent.GetMyType())
	//	{
	//	case CU::eEvent::RESIZE:

	//		/*if (mySwapChain.Get() != nullptr)
	//		{
	//		my2DEngine->OnResize();
	//		RECT rc;
	//		GetClientRect(myHWND, &rc);
	//		myInfoArgument.myResolution.myX = rc.right - rc.left;
	//		myInfoArgument.myResolution.myY = rc.bottom - rc.top;
	//		OnResize(myInfoArgument.myResolution);
	//		}*/

	//		break;

	//	default:
	//		break;
	//	}
	//	return true;
	//}

	HWND& DirectXEngine::GetWindowHandle()
	{
		return myHWND;
	}

	bool DirectXEngine::HandleOnResizeEvent(const CU::PoolPointer<CU::Event>& anEvent)
	{
		anEvent;
		//OnResize(myInfoArgument.myResolution);
		return true;
	}

	bool DirectXEngine::Init(HWND& aHwnd, WNDPROC aWindowProc, const GE::SetupInfo& aInfoArgument)
	{
		if (my3DDevice.Get() != nullptr)
		{
			DL_ASSERT("DirectX already initiated.");
		}

		ObserveEvent(CU::eEvent::RESIZE, HANDLE_EVENT_FUNCTION(HandleOnResizeEvent), MININT32);

		myHWND = aHwnd;

		if (myHWND == NULL)
		{
			WindowSetup(aWindowProc, aInfoArgument);
		}
		else
		{
			myInfoArgument = aInfoArgument;
		}
		if (D3DSetup() == false)
		{
			return false;
		}

		D3D11_RASTERIZER_DESC rasterizerDesc;
		ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

		rasterizerDesc.AntialiasedLineEnable = TRUE;
		rasterizerDesc.CullMode = D3D11_CULL_BACK;
		rasterizerDesc.DepthBias = 0;
		rasterizerDesc.DepthBiasClamp = 0.0f;
		rasterizerDesc.DepthClipEnable = TRUE;
		rasterizerDesc.FillMode = D3D11_FILL_SOLID;
		rasterizerDesc.FrontCounterClockwise = FALSE;
		rasterizerDesc.MultisampleEnable = FALSE;
		rasterizerDesc.ScissorEnable = FALSE;
		rasterizerDesc.SlopeScaledDepthBias = 0.0f;
		rasterizerDesc.DepthClipEnable = TRUE;

		COMObjectPointer<ID3D11RasterizerState> rasterizerState;

		HRESULT result = my3DDevice->CreateRasterizerState(&rasterizerDesc, &rasterizerState);
		if (FAILED(result))
		{
			DL_ASSERT("Failed to create rasterizer stage.");
		}

		myContext->RSSetState(rasterizerState.Get());

		Create2DEngine();

		myRenderer = new Renderer();

		return true;
	}

	void DirectXEngine::InitRenderer()
	{
		myRenderer->Init(this);
	}

	bool DirectXEngine::D3DDeviceSetup()
	{
		UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#ifdef _DEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		HRESULT result;

		result = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, 0, 0, D3D11_SDK_VERSION, &my3DDevice, &myFeatureLevel, &myContext);

		if (FAILED(result))
		{
			createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
			result = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, 0, 0, D3D11_SDK_VERSION, &my3DDevice, &myFeatureLevel, &myContext);
			if (FAILED(result))
			{
				DL_ASSERT("Direct3D failed to initalize");
			}
		}

		if (myFeatureLevel < D3D_FEATURE_LEVEL_11_0)
		{
			DL_ASSERT("DirectX 11 not supported by this machine.");
		}

		/*result = (my3DDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, ));

		if (FAILED(result))
		{
		DL_ASSERT("Direct3D failed to check Multi sample quality level.");
		}

		if (ourMSAAQuality <= 0)
		{
		DL_ASSERT("4x MSAA not supported.");
		}*/
		return true;
	}

	bool DirectXEngine::D3DSetup()
	{
		if (D3DDeviceSetup() == false)
		{
			return false;
		}
		if (D3DSwapChainSetup() == false)
		{
			return false;
		}
		if (D3DStencilBufferSetup() == false)
		{
			return false;
		}
		if (D3DViewPortSetup(myInfoArgument.myResolution) == false)
		{
			return false;
		}
		return true;
	}

	bool DirectXEngine::D3DStencilBufferSetup()
	{
		HRESULT result;

		ID3D11Texture2D* backbuffer;
		ID3D11RenderTargetView* aRenderTarget;

		mySwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backbuffer));
		result = my3DDevice->CreateRenderTargetView(backbuffer, NULL, &aRenderTarget);
		mySwapChainRenderTarget.myRenderTarget.SetRenderTargetView(aRenderTarget, myInfoArgument.myResolution);

		if (FAILED(result) == true)
		{
			WF::AssertComAndWindowsError(result, "Failed to create rendertarget.");
		}

		backbuffer->Release();

		mySceneRenderTarget.myRenderTarget = Texture(this, myInfoArgument.myResolution, DXGI_FORMAT_R8G8B8A8_UNORM, true, true);


		mySwapChainRenderTarget.myDepthStencil = Texture(this, myInfoArgument.myResolution, DXGI_FORMAT_D24_UNORM_S8_UINT, false, false, false, true);
		mySceneRenderTarget.myDepthStencil = Texture(this, myInfoArgument.myResolution, DXGI_FORMAT_D24_UNORM_S8_UINT, false, false, false, true);


		return true;
	}

	bool DirectXEngine::D3DSwapChainSetup()
	{
		HRESULT result;

		IDXGIDevice* dxgiDevice = nullptr;
		result = my3DDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));

		if (FAILED(result) == true)
		{
			DL_ASSERT("Failed to query interface for dxgiDevice.");
		}

		IDXGIAdapter* dxgiAdapter = nullptr;
		result = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&dxgiAdapter));

		if (FAILED(result) == true)
		{
			DL_ASSERT("Failed to get dxgiAdapter.");
		}

		IDXGIFactory* dxgiFactory = nullptr;
		result = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&dxgiFactory));

		if (FAILED(result) == true)
		{
			DL_ASSERT("Failed to get dxgiFactory.");
		}

		DXGI_SWAP_CHAIN_DESC swapChainDescription = CreateSwapChainDescription();

		result = dxgiFactory->CreateSwapChain(my3DDevice.Get(), &swapChainDescription, &mySwapChain);

		if (FAILED(result) == true)
		{
			DL_ASSERT("Failed to create swapchain.");
		}
		dxgiAdapter->Release();
		dxgiDevice->Release();
		dxgiFactory->Release();
		return true;
	}

	DXGI_SWAP_CHAIN_DESC DirectXEngine::CreateSwapChainDescription()
	{
		DXGI_SWAP_CHAIN_DESC swapChainDescription;
		DXGI_MODE_DESC modeDescription;
		modeDescription.Width = myInfoArgument.myResolution.myX;
		modeDescription.Height = myInfoArgument.myResolution.myY;
		modeDescription.RefreshRate.Numerator = 60;
		modeDescription.RefreshRate.Denominator = 1;
		modeDescription.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		modeDescription.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		modeDescription.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		swapChainDescription.SampleDesc.Count = 4;
		my3DDevice->CheckMultisampleQualityLevels(modeDescription.Format, swapChainDescription.SampleDesc.Count, &swapChainDescription.SampleDesc.Quality);

		/*	if (swapChainDescription.SampleDesc.Quality > 0)
			{
			swapChainDescription.SampleDesc.Quality -= 1;
			}
			else*/
		{
			swapChainDescription.SampleDesc.Count = 1;
			swapChainDescription.SampleDesc.Quality = 0;
		}

		swapChainDescription.BufferDesc = modeDescription;
		swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDescription.BufferCount = 1;
		swapChainDescription.OutputWindow = myHWND;
		swapChainDescription.Windowed = myInfoArgument.myWindowedMode;
		swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDescription.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		return swapChainDescription;
	}

	bool DirectXEngine::D3DViewPortSetup(const Vector2<int>& aResolution)
	{
		D3D11_VIEWPORT viewPort;

		viewPort.TopLeftX = 0.f;
		viewPort.TopLeftY = 0.f;
		viewPort.Width = static_cast<float>(aResolution.myX);
		viewPort.Height = static_cast<float>(aResolution.myY);
		viewPort.MinDepth = 0.f;
		viewPort.MaxDepth = 1.f;

		myContext->RSSetViewports(1, &viewPort);

		return true;
	}

	bool DirectXEngine::WindowSetup(WNDPROC aWindowProc, const SetupInfo& aInfoArugment)
	{
		myInfoArgument = aInfoArugment;

		HINSTANCE currentHInstance = GetModuleHandle(NULL);
		WNDCLASSEX wcex;

		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = 0;
		wcex.lpfnWndProc = aWindowProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = currentHInstance;
		wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOWTEXT);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = myInfoArgument.mySessionName.c_str();
		wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

		if (RegisterClassEx(&wcex) == false)
		{
			DL_ASSERT("Failed to register window class");
		}


		RECT rect = { myInfoArgument.myWindowLocation.x, myInfoArgument.myWindowLocation.y, myInfoArgument.myResolution.myX + myInfoArgument.myWindowLocation.x, myInfoArgument.myResolution.myY + myInfoArgument.myWindowLocation.y };

		AdjustWindowRect(&rect, WS_POPUP | WS_CAPTION | WS_MINIMIZEBOX | WS_VISIBLE, false);

		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;

		//rect.bottom += rect.left - rect.top;

		//myInfoArgument.myResolution.myX = rect.right - rect.left;
		//myInfoArgument.myResolution.myY = rect.bottom - rect.top;

		myHWND = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			aInfoArugment.mySessionName.c_str(),
			aInfoArugment.mySessionName.c_str(),
			WS_POPUP | WS_CAPTION | WS_MINIMIZEBOX | WS_VISIBLE,
			//myInfoArgument.myWindowLocation.x, myInfoArgument.myWindowLocation.y, myInfoArgument.myResolution.myX, myInfoArgument.myResolution.myY,
			rect.left - 2,
			rect.top - 2,
			width + 4,
			height + 4,
			NULL, NULL, currentHInstance, NULL);

		if (myHWND == false)
		{
			std::string errorMessage = "Window was unable to be created. ";
			DL_ASSERT(errorMessage.c_str());
		}

		//ShowWindow(myHWND, SW_SHOW);
		return true;
	}

	void DirectXEngine::SwitchBuffers()
	{
		HRESULT result = mySwapChain->Present(0, 0);

		if (FAILED(result) == true)
		{
			DL_ASSERT("Failed to render swapchain.");
		}
	}

	bool DirectXEngine::DirectXInitiatedTest()
	{
		if (my3DDevice == nullptr)
		{
			DL_ASSERT("Graphics DirectX not initiated!");
			return false;
		}
		else
		{
			return true;
		}
	}

	DirectXEngine::DirectXEngine()
	{
		my3DDevice = nullptr;
		myContext = nullptr;
		myDepthBuffer = nullptr;
		myHWND = nullptr;
		mySwapChain = nullptr;
		myDepthState = nullptr;
	}

	DirectXEngine::~DirectXEngine()
	{
		myContext->ClearState();

		myContext->OMSetRenderTargets(0, NULL, NULL);
		mySwapChain = nullptr;
		myDepthState = nullptr;
		myDepthBuffer = nullptr;

		myContext->Flush();

#ifdef _DEBUG
		ID3D11Debug* debug;
		my3DDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&debug));
		debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
		debug->Release();
#endif
	}

	const SetupInfo& DirectXEngine::GetSetupInfo()
	{
		return myInfoArgument;
	}

	void DirectXEngine::DrawFonts(const CU::GrowingArray<CU::RenderCommandFont>& someFonts)
	{
		my2DEngine->Render(someFonts);
	}

	Texture DirectXEngine::WriteTextOnTexture(Texture* aBackground, const CU::GrowingArray<CU::RenderCommandFont>& someFonts) //Not thread friendly
	{
		Texture theTextTex(this, aBackground->GetImageSize(), DXGI_FORMAT_R8G8B8A8_UNORM, true, true, false);

		float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		myContext->ClearRenderTargetView(theTextTex.GetRenderTargetView().Get(), color);
		myContext->OMSetRenderTargets(1, &theTextTex.GetRenderTargetView(), NULL);
		my2DEngine->Render(someFonts, theTextTex.GetSurface(), theTextTex.GetImageSize());

		Texture newTexture(this, aBackground->GetImageSize(), DXGI_FORMAT_R16G16B16A16_FLOAT, true, true);
		TextureMerger::GetInstance()->MergeTextures(&newTexture, aBackground, &theTextTex, "RenderSecondaryOverride");

		return newTexture;
	}

	void DirectXEngine::RenderScenes()
	{
		RenderScenes(mySwapChainRenderTarget.myRenderTarget, mySwapChainRenderTarget.myDepthStencil, true);
		SwitchBuffers();
	}

	void DirectXEngine::RenderScenes(Texture& aRenderTargetView, Texture& aDepthStencilView, bool aShouldRenderText)
	{
		CU::GrowingArray<CU::ScenePrioID> mySceneIDs = CU::EventManager::GetInstance()->GetRenderBuffer().GetSceneIDInRenderOrder();
		RenderTargetData targetData;
		targetData.myRenderTarget = aRenderTargetView;
		targetData.myDepthStencil = aDepthStencilView;

		if (mySceneIDs.Size() > 0)
		{
			float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
			myContext->ClearRenderTargetView(aRenderTargetView.GetRenderTargetView().Get(), color);
			myContext->ClearDepthStencilView(aDepthStencilView.GetDepthStencilView().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, NULL);

			myContext->ClearRenderTargetView(mySceneRenderTarget.myRenderTarget.GetRenderTargetView().Get(), color);
			myContext->ClearDepthStencilView(mySceneRenderTarget.myDepthStencil.GetDepthStencilView().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, NULL);



			for (unsigned short i = 0; i < mySceneIDs.Size(); i++)
			{
				CU::SceneRenderCommands& currentRenderCommands = CU::EventManager::GetInstance()->GetRenderBuffer().GetRenderCommands(mySceneIDs[i].myID);

				if (currentRenderCommands.mySceneData != nullptr)
				{
					VTUNE_API_TASK_BEGIN(locSceneRenderDomain, locSceneRenderTimeStamp);
					/*if (i == 0)
					{
						myRenderer->RenderScene(targetData, currentRenderCommands);
					}
					else*/
					{
						myContext->ClearRenderTargetView(mySceneRenderTarget.myRenderTarget.GetRenderTargetView().Get(), color);
						myContext->ClearDepthStencilView(mySceneRenderTarget.myDepthStencil.GetDepthStencilView().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, NULL);

						myRenderer->RenderScene(mySceneRenderTarget, currentRenderCommands);

						TextureMerger::GetInstance()->MergeTextures(&aRenderTargetView, &mySceneRenderTarget.myRenderTarget, &mySceneRenderTarget.myRenderTarget, "BlendFirstWithTarget");
					}
					VTUNE_API_TASK_END(locSceneRenderDomain);

					if (aShouldRenderText == true)
					{

						VTUNE_API_TASK_BEGIN(locFontRenderDomain, locFontRenderTimeStamp);

						my2DEngine->Render(currentRenderCommands.myFontRenderCommands);

						VTUNE_API_TASK_END(locFontRenderDomain);
					}
				}
			}
		}
		CU::EventManager::GetInstance()->AddEvent(CU::Event(CU::eEvent::NEW_FRAME));
	}

	void DirectXEngine::Create2DEngine()
	{
		COMObjectPointer<IDXGISurface> surface;

		HRESULT result = mySwapChain->GetBuffer(0,
			IID_PPV_ARGS(&surface));

		if (FAILED(result) == true)
		{
			DL_ASSERT("Failed to get swapchain buffer.");
		}

		my2DEngine = new DirectX2D();
		my2DEngine->Create(myHWND, surface);
	}

	ShadowManager& DirectXEngine::GetShadowManager()
	{
		return myRenderer->GetShadowManager();
	}

}

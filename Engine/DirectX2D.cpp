#include "Engine_Precompiled.h"
#include "DirectX2D.h"
#include "WindowsFunctions.h"
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include "../Common Utilities/RenderCommandFont.h"
#include <bitset>
#include <locale>
#include <algorithm>
#include <codecvt>
#include "Engine.h"
#include "d3dx11effect.h"
#include "DirectXEngine.h"

#include "ResourceFontCollectionLoader.h"
#include "ResourceFontFileLoader.h"

#include <comdef.h>


#pragma comment (lib, "d2d1.lib")
#pragma comment (lib, "dwrite.lib")

namespace GraphicsEngine
{
	DirectX2D::DirectX2D()
	{
		myHwnd = NULL;
		myFactory = nullptr;
		myRenderTarget = nullptr;
		myBrush = nullptr;
		myWriteFactory = nullptr;
		myTextFormat = nullptr;
		myTextLayout = nullptr;
	}

	DirectX2D::~DirectX2D()
	{
		myWriteFactory->UnregisterFontCollectionLoader(ResourceFontCollectionLoader::GetLoader());
		myWriteFactory->UnregisterFontFileLoader(ResourceFontFileLoader::GetLoader());
	}

	void DirectX2D::Create(HWND anHwnd, COMObjectPointer<IDXGISurface>& aSurface)
	{
		aSurface;
		anHwnd;
#ifndef NSIGHT
		mySetupInfo = Engine::GetInstance()->GetSetupInfo();

		myHwnd = anHwnd;
		HRESULT result;
		result = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &myFactory);
		if (FAILED(result) == true)
		{
			DL_ASSERT("Failed to create 2dFactory.");
		}

		FLOAT dpiX;
		FLOAT dpiY;
		myFactory->GetDesktopDpi(&dpiX, &dpiY);

		D2D1_RENDER_TARGET_PROPERTIES props =
			D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
			dpiX,
			dpiY
			);

		result = myFactory->CreateDxgiSurfaceRenderTarget(aSurface.Get(), &props, &myRenderTarget);

		if (FAILED(result) == true)
		{
			DL_ASSERT("Failed to create rendertarget.");
		}

		result = myRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &myBrush);
		if (FAILED(result))
		{
			DL_ASSERT("Failed to create brush");
		}

		result = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&myWriteFactory));

		if (FAILED(result) == true)
		{
			DL_ASSERT("Failed to create Dwrite Factory");
		}

		result = myWriteFactory->CreateTextFormat(L"Arial", NULL, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL, 30.0f, L"en-us", &myTextFormat);

		/*result = myWriteFactory->CreateTextFormat(L"Arial", NULL, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL, 200.0f, L"en-us", &mySecondTextFormat);*/

		/*if (FAILED(result) == true)
		{
			DL_ASSERT("Failed to create text format.");
		}*/

		/*myTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		myTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

		mySecondTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		mySecondTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);*/

		myWriteFactory->RegisterFontFileLoader(ResourceFontFileLoader::GetLoader());
		myWriteFactory->RegisterFontCollectionLoader(ResourceFontCollectionLoader::GetLoader());

		LoadCustomFonts();
#endif
	}

	void DirectX2D::Render(const CU::GrowingArray<CU::RenderCommandFont>& someRenderCommandFonts, COMObjectPointer<IDXGISurface>& aSurface, Vector2<int> aTargetSize)
	{
		someRenderCommandFonts;
		aSurface;
		aTargetSize;

#ifndef NSIGHT
		COMObjectPointer<ID2D1RenderTarget> theTarget;
		FLOAT dpiX;
		FLOAT dpiY;
		myFactory->GetDesktopDpi(&dpiX, &dpiY);

		D2D1_RENDER_TARGET_PROPERTIES props =
			D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
			dpiX,
			dpiY
			);

		HRESULT result = myFactory->CreateDxgiSurfaceRenderTarget(aSurface.Get(), &props, &theTarget);
		result;
		
		Render(someRenderCommandFonts, theTarget.Get(), &aTargetSize);

#endif
	}

	void DirectX2D::Render(const CU::GrowingArray<CU::RenderCommandFont>& someRenderCommandFonts, ID2D1RenderTarget* aRenderTarget, Vector2<int>* aTargetSize)
	{
		someRenderCommandFonts;
		aRenderTarget;
		aTargetSize;

#ifndef NSIGHT
		if (aRenderTarget == nullptr)
			aRenderTarget = myRenderTarget.Get();

		if (someRenderCommandFonts.Size() != 0)
		{
			aRenderTarget->BeginDraw();

			for (unsigned short i = 0; i < someRenderCommandFonts.Size(); ++i)
			{
				CU::RenderCommandFont renderCommand = someRenderCommandFonts[i];

				if (aTargetSize != nullptr)
				{
					renderCommand.myTextBox.x *= aTargetSize->x;
					renderCommand.myTextBox.y *= aTargetSize->y;
				}
				else
				{
					renderCommand.myTextBox.x *= mySetupInfo.myResolution.x;
					renderCommand.myTextBox.y *= mySetupInfo.myResolution.y;
				}

				renderCommand.myTextBox.x += renderCommand.myPixelOffset.x;
				renderCommand.myTextBox.y += renderCommand.myPixelOffset.y;

				if (myTextFormat.Get())
					myTextFormat->Release();
				if (myTextLayout.Get())
					myTextLayout->Release();

				D2D1_POINT_2F renderOrigin = { renderCommand.myTextBox.x, renderCommand.myTextBox.y };

				HRESULT result = myWriteFactory->CreateTextFormat(WF::WideStringConvertion(renderCommand.myFontName.c_str()).c_str(),
					NULL,
					DWRITE_FONT_WEIGHT_REGULAR,
					DWRITE_FONT_STYLE_NORMAL,
					DWRITE_FONT_STRETCH_NORMAL,
					renderCommand.myFontSize,
					L"en-us",
					&myTextFormat);

				if (FAILED(result) == true)
				{
					WF::AssertComAndWindowsError(result, "Failed to create IDWriteTextFormat.");
					return;
				}

				D2D1_COLOR_F textColor;
				CopyMemory(&textColor, &renderCommand.myTextColor, sizeof(float) * 4);
				myBrush->SetColor(textColor);

				std::string renderString = renderCommand.myText.c_str();
				GetStringWithoutEffectTags(renderString);

				result = myWriteFactory->CreateTextLayout(
					WF::WideStringConvertion(renderString.c_str()).c_str(),
					renderString.length(),
					myTextFormat.Get(),
					renderCommand.myTextBox.z,
					renderCommand.myTextBox.w,
					&myTextLayout);

				if (FAILED(result) == true)
				{
					DL_ASSERT("Failed to create IDWriteTextLayout");
					return;
				}

				// Process effect lists
				renderString = renderCommand.myText.c_str();
				ProcessTextEffectsInString(renderString, myTextLayout, myBrush);

				myTextLayout->SetParagraphAlignment(static_cast<DWRITE_PARAGRAPH_ALIGNMENT>(renderCommand.myTextVerticalAlignment));
				myTextLayout->SetTextAlignment(DWRITE_TEXT_ALIGNMENT(renderCommand.myTextAlignment));

				if ((renderCommand.myTextBoxHotSpot & CU::eTextBoxHotSpot::VERT_CENTER) != 0)
					renderOrigin.y = renderCommand.myTextBox.y - (renderCommand.myTextBox.w * 0.5f);
				if ((renderCommand.myTextBoxHotSpot & CU::eTextBoxHotSpot::BOTTOM) != 0)
					renderOrigin.y = renderCommand.myTextBox.y - renderCommand.myTextBox.w;

				if ((renderCommand.myTextBoxHotSpot & CU::eTextBoxHotSpot::CENTER) != 0)
					renderOrigin.x = renderCommand.myTextBox.x - (renderCommand.myTextBox.z * 0.5f);
				if ((renderCommand.myTextBoxHotSpot & CU::eTextBoxHotSpot::RIGHT) != 0)
					renderOrigin.x = renderCommand.myTextBox.x - renderCommand.myTextBox.z;

				aRenderTarget->DrawTextLayout(renderOrigin, myTextLayout.Get(), myBrush.Get());
			}

			aRenderTarget->EndDraw();
		}
#endif
	}

	void DirectX2D::SetHwnd(HWND anHwnd)
	{
		anHwnd;
#ifndef NSIGHT
		anHwnd;
		/*myHwnd = ;
		RECT rect;
		GetClientRect(myHwnd, &rect);

		D2D1_SIZE_U size = D2D1::SizeU(rect.right - rect.left, rect.bottom - rect.top);

		if (myRenderTarget != nullptr)
		{
		myRenderTarget->Release();
		}

		HRESULT result;

		result = myFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(myHwnd, size), &myRenderTarget);
		if (FAILED(result) == true)
		{
		DL_ASSERT("Failed to create Rendertarget.");
		}


		*/
#endif
	}

	void DirectX2D::RegisterCustomFont()
	{
		//myWriteFactory->RegisterFontFileLoader();
	}

	void DirectX2D::Recreate(HWND anHwnd, COMObjectPointer<IDXGISurface>& aSurface)
	{
		anHwnd;
		aSurface;

#ifndef NSIGHT
		mySetupInfo = Engine::GetInstance()->GetSetupInfo();

		myHwnd = anHwnd;

		FLOAT dpiX;
		FLOAT dpiY;
		myFactory->GetDesktopDpi(&dpiX, &dpiY);

		D2D1_RENDER_TARGET_PROPERTIES props =
			D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
			dpiX,
			dpiY
			);

		HRESULT result = myFactory->CreateDxgiSurfaceRenderTarget(aSurface.Get(), &props, &myRenderTarget);

		if (FAILED(result) == true)
		{
			DL_ASSERT("Failed to create rendertarget.");
		}

#endif
	}

	void DirectX2D::OnResize()
	{
		myRenderTarget = nullptr;
	}

	COMObjectPointer<ID3D11ShaderResourceView> DirectX2D::CreateTextTexture(COMObjectPointer<ID3D11Device>& aDevice, const CU::GrowingArray<CU::RenderCommandFont>& someRenderCommands)
	{
		aDevice; 
		someRenderCommands;

		COMObjectPointer<ID3D11Texture2D> texture;
		COMObjectPointer<ID3D11ShaderResourceView> resourceView;
#ifndef NSIGHT
		D3D11_TEXTURE2D_DESC textureDesc;
		DXGI_SAMPLE_DESC sampleDesc;
		ZeroMemory(&sampleDesc, sizeof(DXGI_SAMPLE_DESC));
		ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));

		textureDesc.Width = 512;
		textureDesc.Height = 512;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		
		HRESULT result = aDevice->CreateTexture2D(&textureDesc, nullptr, &texture);
		if (FAILED(result))
		{
			WF::AssertComAndWindowsError(result, "Failed to create 2d texture.");
		}


		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		ZeroMemory(&shaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;
		

		result = aDevice->CreateShaderResourceView(texture.Get(), &shaderResourceViewDesc, &resourceView);
		if (FAILED(result))
		{
			WF::AssertComAndWindowsError(result, "Failed to create shader resource view.");
		}

		const float dxgiDpi = 96.0f;


		COMObjectPointer<ID2D1RenderTarget> renderTarget;

		COMObjectPointer<IDXGISurface> surface;
		result = texture->QueryInterface<IDXGISurface>(&surface);

		if (FAILED(result))
		{
			WF::AssertComAndWindowsError(result, "Failed to get surface.");
		}

		D2D1_RENDER_TARGET_PROPERTIES props =
			D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
			dxgiDpi,
			dxgiDpi
			);

		result = myFactory->CreateDxgiSurfaceRenderTarget(surface.Get(), &props, &renderTarget);

		Render(someRenderCommands, renderTarget.Get());

#endif
		return resourceView;
	}

	void DirectX2D::LoadCustomFonts()
	{
		myWriteFactory->GetSystemFontCollection(&mySystemFontCollection, TRUE);

		static const char* fontFiles[] = { "Norse.otf" };

		HRESULT hr = myWriteFactory->CreateCustomFontCollection(
			ResourceFontCollectionLoader::GetLoader(),
			fontFiles,
			sizeof(const char*),
			&myCustomFontCollection);

		_com_error err(hr);
		DL_PRINT(err.ErrorMessage());
	}

	void DirectX2D::GetStringWithoutEffectTags(std::string& anOutputString)
	{
		while (anOutputString.find_first_of('{') != anOutputString.npos)
		{
			// Get the next effect list.
			size_t commandListStart = anOutputString.find_first_of('{');
			size_t commandListEnd = anOutputString.find_first_of('}');

			if (commandListStart != anOutputString.npos && commandListEnd == anOutputString.npos)
				return;

			anOutputString.erase(commandListStart, commandListEnd - commandListStart + 1);
		}
	}

	void DirectX2D::ProcessTextEffectsInString(const std::string& anOutputString, GE::COMObjectPointer<IDWriteTextLayout> aTextLayout, GE::COMObjectPointer<ID2D1SolidColorBrush>& aDefaultBrush)
	{
		std::string restString = anOutputString;
		std::string currentEffectCommandList;

		while (restString.find_first_of('{') != restString.npos)
		{
			// Get the next effect list.
			size_t commandListStart = restString.find_first_of('{');
			size_t commandListEnd = restString.find_first_of('}');

			if (commandListStart != restString.npos && commandListEnd == restString.npos)
				return;

			currentEffectCommandList = restString.substr(commandListStart + 1, commandListEnd - commandListStart - 1);
			restString.erase(commandListStart, commandListEnd - commandListStart + 1);

			// Process the list
			StringReplaceAll(currentEffectCommandList, " ", "");
			StringReplaceAll(currentEffectCommandList, "\t", "");
			while (currentEffectCommandList.empty() == false)
			{
				size_t commandEnd = currentEffectCommandList.find_first_of(';');
				size_t valueStart = currentEffectCommandList.find_first_of(':');

				if (commandEnd == currentEffectCommandList.npos)
					commandEnd = currentEffectCommandList.length();

				std::string flagName = "";
				std::string valueString = "";

				if (commandEnd < valueStart)	// No-value flag
				{
					flagName = currentEffectCommandList.substr(0, commandEnd);
					currentEffectCommandList.erase(0, commandEnd + 1);
				}
				else
				{
					flagName = currentEffectCommandList.substr(0, valueStart);
					valueString = currentEffectCommandList.substr(valueStart + 1, commandEnd - (valueStart + 1));
					currentEffectCommandList.erase(0, commandEnd + 1);
				}

				std::transform(flagName.begin(), flagName.end(), flagName.begin(), ::tolower);
				std::transform(valueString.begin(), valueString.end(), valueString.begin(), ::tolower);

				DWRITE_TEXT_RANGE commandRange;
				commandRange.startPosition = commandListStart;
				commandRange.length = INT_MAX;

				ProcessCommand(flagName, valueString, aTextLayout, commandRange, aDefaultBrush);
			}
		}
	}

	void DirectX2D::ProcessCommand(const std::string& aFlagName, const std::string& aValue, GE::COMObjectPointer<IDWriteTextLayout> aTextLayout, const DWRITE_TEXT_RANGE& aRange, GE::COMObjectPointer<ID2D1SolidColorBrush>& aDefaultBrush)
	{
		if (aFlagName == "reset" || aFlagName == "default")								// Reset / Default
		{
			aTextLayout->SetFontWeight(DWRITE_FONT_WEIGHT_NORMAL, aRange);
			aTextLayout->SetUnderline(false, aRange);
			aTextLayout->SetStrikethrough(false, aRange);

			aTextLayout->SetDrawingEffect(aDefaultBrush.Get(), aRange);
			aTextLayout->SetFontFamilyName(L"Arial", aRange);
		}


		// Commands that require values after this point
		if (aValue.empty())
			return;

		if (aFlagName == "bold")														// Bold
		{
			if (aValue == "true")
			{
				aTextLayout->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD, aRange);
			}
			else
			{
				aTextLayout->SetFontWeight(DWRITE_FONT_WEIGHT_NORMAL, aRange);
			}
		}
		else if (aFlagName == "italic")													// Italic
		{
			if (aValue == "true")
			{
				aTextLayout->SetFontStyle(DWRITE_FONT_STYLE_ITALIC, aRange);
			}
			else
			{
				aTextLayout->SetFontStyle(DWRITE_FONT_STYLE_NORMAL, aRange);
			}
		}
		else if (aFlagName == "underline")												// Underline
		{
			aTextLayout->SetUnderline(aValue == "true", aRange);
		}
		else if (aFlagName == "strikethrough")
		{
			aTextLayout->SetStrikethrough(aValue == "true", aRange);
		}
		else if (aFlagName == "color")													// Color
		{
			Vector4f color = GetColorFromHexString(aValue);
			D2D1_COLOR_F d2Color;
			d2Color.r = color.r;
			d2Color.g = color.g;
			d2Color.b = color.b;
			d2Color.a = color.a;

			COMObjectPointer<ID2D1SolidColorBrush> colorBrush;
			if (FAILED(myRenderTarget->CreateSolidColorBrush(d2Color, &colorBrush)))
			{
				DL_DEBUG("Failed to create SolidColorBrush (DirectX2D::ProcessCommand()).");
				return;
			}

			aTextLayout->SetDrawingEffect(colorBrush.Get(), aRange);
		}
		else if (aFlagName == "fontsize")												// Size
		{
			float emSize = static_cast<float>(atof(aValue.c_str()));
			aTextLayout->SetFontSize(emSize, aRange);
		}
		else if (aFlagName == "font")
		{
			std::wstring wValue;
			wValue.assign(aValue.begin(), aValue.end());

			if (aValue == "norse")
				aTextLayout->SetFontCollection(myCustomFontCollection.Get(), aRange);
			else
				aTextLayout->SetFontCollection(mySystemFontCollection.Get(), aRange);

			aTextLayout->SetFontFamilyName(wValue.c_str(), aRange);
		}
	}

	void DirectX2D::StringReplaceAll(std::string& str, const std::string& from, const std::string& to)
	{
		if (from.empty())
			return;
		size_t start_pos = 0;
		while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
			str.replace(start_pos, from.length(), to);
			start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
		}
	}

	const Vector4f DirectX2D::GetColorFromHexString(const std::string& aString) const
	{
		if (aString.empty())
			return {1, 0, 1, 1};

		size_t offset = 0;
		if (aString[0] == '#')
			offset = 1;
		else if (aString.substr(0, 2) == "0x")
			offset = 2;

		std::string hexString;

		if (aString.length() - offset == 3)
		{
			hexString += aString.substr(0, offset);
			hexString += aString.substr(offset, 1) + aString.substr(offset, 1);
			hexString += aString.substr(offset + 1, 1) + aString.substr(offset + 1, 1);
			hexString += aString.substr(offset + 2, 1) + aString.substr(offset + 2, 1);
		}
		else if(aString.length() - offset == 6)
		{
			hexString = aString;
		}
		else
		{
			return{ 1, 0, 1, 1 };
		}

		Vector4<long> colorui;

		colorui.r = strtol(hexString.substr(offset, 2).c_str(), nullptr, 16);
		colorui.g = strtol(hexString.substr(offset + 2, 2).c_str(), nullptr, 16);
		colorui.b = strtol(hexString.substr(offset + 4, 2).c_str(), nullptr, 16);

		Vector4f color;

		color.r = static_cast<float>(colorui.r) / 255.f;
		color.g = static_cast<float>(colorui.g) / 255.f;
		color.b = static_cast<float>(colorui.b) / 255.f;
		color.a = 1;

		return color;
	}
}
#pragma once
#include "../Common Utilities/GrowingArray.h"
#include "SetupInfo.h"
#include "COMObjectPointer.h"
#include "../Common Utilities/Vector4.h"

#include <map>

namespace CommonUtilities
{
	struct RenderCommandFont;
}
namespace CU = CommonUtilities;

struct IDWriteFontCollection;
struct ID2D1Factory;
struct IDWriteFontFace;
struct ID2D1RenderTarget;
struct ID2D1SolidColorBrush;
struct IDWriteFactory;
struct IDWriteTextFormat;
struct IDXGISurface;
struct ID3D11ShaderResourceView;
struct ID3D11Device;
struct ID3D11RenderTargetView;
struct IDWriteTextLayout;
struct DWRITE_TEXT_RANGE;

namespace GraphicsEngine
{
	class DirectX2D
	{
	public:
		DirectX2D();
		~DirectX2D();
		void Create(HWND anHwnd, COMObjectPointer<IDXGISurface>& aSurface);
		void SetHwnd(HWND anHwnd);
		void OnResize();
		void Recreate(HWND anHwnd, COMObjectPointer<IDXGISurface>& aSurface);

		void Render(const CU::GrowingArray<CU::RenderCommandFont>& myRenderCommandFonts, COMObjectPointer<IDXGISurface>& aSurface, Vector2<int> aTargetSize);
		void Render(const CU::GrowingArray<CU::RenderCommandFont>& myRenderCommandFonts, ID2D1RenderTarget* aRenderTarget = nullptr, Vector2<int>* aTargetSize = nullptr);

		COMObjectPointer<ID3D11ShaderResourceView> DirectX2D::CreateTextTexture(COMObjectPointer<ID3D11Device>& aDevice, const CU::GrowingArray<CU::RenderCommandFont>& someRenderCommands);

		void RegisterCustomFont();

	private:
		void LoadCustomFonts();

		void GetStringWithoutEffectTags(std::string& anOutputString);
		void ProcessTextEffectsInString(const std::string& anOutputString, GE::COMObjectPointer<IDWriteTextLayout> aTextLayout, GE::COMObjectPointer<ID2D1SolidColorBrush>& aDefaultBrush);
		void ProcessCommand(const std::string& aFlagName, const std::string& aValue, GE::COMObjectPointer<IDWriteTextLayout> aTextLayout, const DWRITE_TEXT_RANGE& aRange, GE::COMObjectPointer<ID2D1SolidColorBrush>& aDefaultBrush);
		const Vector4f GetColorFromHexString(const std::string& aString) const;
		void StringReplaceAll(std::string& str, const std::string& from, const std::string& to);

		HWND myHwnd;
		COMObjectPointer<ID2D1Factory> myFactory;
		COMObjectPointer<ID2D1RenderTarget> myRenderTarget;
		COMObjectPointer<ID2D1SolidColorBrush> myBrush;
		COMObjectPointer<IDWriteFactory> myWriteFactory;

		COMObjectPointer<IDWriteTextFormat> myTextFormat;
		COMObjectPointer<IDWriteTextLayout> myTextLayout;

		GE::COMObjectPointer<IDWriteFontCollection> myCustomFontCollection;
		GE::COMObjectPointer<IDWriteFontCollection> mySystemFontCollection;

		SetupInfo mySetupInfo;
	};
}

namespace GE = GraphicsEngine;
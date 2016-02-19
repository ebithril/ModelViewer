#include "Common Utilities_Precompiled.h"
#include "RenderCommandFont.h"

namespace CommonUtilities
{
	RenderCommandFont::RenderCommandFont(
		const Vector2<float>& aPosition, const CU::String<4096>& aText,
		const Vector4<float>& aColor,
		const float& aScale,
		const Vector2<float>& aTextBoxSize,
		eTextHorizontalAlignment aTextAlignment,
		eTextBoxHotSpot aTextHotspot,
		eTextVerticalAlignment aVerticalAlignment,
		const eLayers aLayer, const std::string& aFontName)
	{
		myText = aText;
		myTextColor = aColor;
		myLayer = aLayer;
		myFontSize = aScale * 12.f;
		myTextAlignment = aTextAlignment;
		myTextBoxHotSpot = aTextHotspot;
		myTextVerticalAlignment = aVerticalAlignment;

		myTextBox.x = aPosition.x;
		myTextBox.y = aPosition.y;
		myTextBox.z = aTextBoxSize.x;
		myTextBox.w = aTextBoxSize.y;
		myPixelOffset = { 0, 0 };

		myFontName = aFontName;
	}

	RenderCommandFont::RenderCommandFont()
	{

	}

	RenderCommandFont::~RenderCommandFont()
	{

	}
}
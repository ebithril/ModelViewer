#pragma once
#include "Vector.h"
#include "CUString.h"
#include "GraphicsEnums.h"
#include "Macros.h"

namespace CommonUtilities
{
	enum eTextBoxHotSpot : unsigned char //Default is Top left
	{
		TOP = 0x00,
		VERT_CENTER = 0x04,
		BOTTOM = 0x08,

		LEFT = 0x00,
		CENTER = 0x01,
		RIGHT = 0x02
	};

	enum class eTextHorizontalAlignment : unsigned char
	{
		LEFT_EDGE,
		RIGHT_EDGE,
		CENTER,
		JUSTIFIED,
	};

	enum class eTextVerticalAlignment : unsigned char
	{
		TOP,
		BOTTOM,
		CENTER,
	};

	struct RenderCommandFont
	{
	public:
		RenderCommandFont(const Vector2<float>& aPosition, const CU::String<4096>& aText,
			const Vector4<float>& aColor = Vector4<float>(0, 0, 0, 1),
			const float& aScale = 1.f,
			const Vector2<float>& aTextBoxSize = Vector2<float>(200.f, 0.f),
			eTextHorizontalAlignment aTextAlignment = eTextHorizontalAlignment::CENTER,
			eTextBoxHotSpot aTextHotspot = eTextBoxHotSpot(0),
			eTextVerticalAlignment aVerticalAlignment = eTextVerticalAlignment::CENTER,
			const eLayers aLayer = eLayers::TEXT, const std::string& aFontName = "Arial");

		RenderCommandFont();
		~RenderCommandFont();

		Vector4<float> myTextBox;
		CU::String<4096> myText;
		Vector4<float> myTextColor;

		float myFontSize;
		eTextBoxHotSpot myTextBoxHotSpot;
		eTextHorizontalAlignment myTextAlignment;
		eTextVerticalAlignment myTextVerticalAlignment;
		std::string myFontName;
		Vector2<float> myPixelOffset;

		eLayers myLayer;
	};
}

namespace CU = CommonUtilities;
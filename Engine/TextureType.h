#pragma once
#include <string>

namespace GraphicsEngine
{
	enum class eTextureType
	{
		DIFFUSE,
		NORMALMAP,
		ROUGHNESS,
		SUBSTANCE,
		AO,
		EMISSIVE,
		NR_OF_TEXTURETYPES,
	};

	std::string GetVariableName(eTextureType aTextureType);
}

#include "Engine_Precompiled.h"
#include "TextureType.h"
#include "../Common Utilities/DL_Assert.h"

namespace GraphicsEngine
{
	std::string GetVariableName(eTextureType aTextureType)
	{
		switch (aTextureType)
		{
		case GraphicsEngine::eTextureType::DIFFUSE:
			return "DiffuseTexture";

		case GraphicsEngine::eTextureType::NORMALMAP:
			return "NormalTexture";

		case GraphicsEngine::eTextureType::ROUGHNESS:
			return "RoughnessTexture";

		case GraphicsEngine::eTextureType::SUBSTANCE:
			return "SubstanceTexture";

		case GraphicsEngine::eTextureType::AO:
			return "AOTexture";

		case GraphicsEngine::eTextureType::EMISSIVE:
			return "EmissiveTexture";

		default:
			DL_ASSERT("Unknown texture type.");
			return "Error";
		}
	}
}
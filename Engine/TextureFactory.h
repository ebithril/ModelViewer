#pragma once

#include "Texture.h"
#include "../Common Utilities/Map.h"

struct ID3D11Device;

namespace GraphicsEngine
{
	enum class eTextureType;

	class TextureFactory
	{
	public:
		TextureFactory();
		~TextureFactory();

		void Init(COMObjectPointer<ID3D11Device>& aDevice);

		const bool IsTextureLoaded(const std::string& aTextureName);
		Texture* GetTexture(const std::string& aTextureName);
		Texture* LoadTexture(const std::string& aFilePath, const eTextureType aTextureType);
		void LoadAllTextures();

		void UnloadTextures();

	private:
		CU::Map<std::string, Texture> myTextures;
		CU::GrowingArray<std::string> myLoadedTextures;
		COMObjectPointer<ID3D11Device> myDevice;
	};

}

namespace GE = GraphicsEngine;
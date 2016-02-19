#include "Engine_Precompiled.h"
#include "TextureFactory.h"
#include "Texture.h"
#include "WindowsFunctions.h"
#include "TextureType.h"
#include "../Common Utilities/CUString.h"
//TEST
#include "Engine.h"
#include "SetupInfo.h"
#include "DXGIFormat.h"
#include <d3d11.h>

namespace GraphicsEngine
{
	TextureFactory::TextureFactory()
	{
		myDevice = nullptr;
	}

	TextureFactory::~TextureFactory()
	{
		myDevice->Release();
	}

	void TextureFactory::Init(COMObjectPointer<ID3D11Device>& aDevice)
	{
		myDevice = aDevice;
		myLoadedTextures.Init(8);
	}

	const bool TextureFactory::IsTextureLoaded(const std::string& aTextureName)
	{
		return myTextures.KeyExists(aTextureName) == true;
	}

	Texture* TextureFactory::GetTexture(const std::string& aTextureName)
	{
		if (myTextures.KeyExists(aTextureName) == true)
		{
			return &myTextures.Get(aTextureName);
		}
		else
		{
			std::string errorString = "Texture with name : " + aTextureName + " doesn't exist.";
			DL_ASSERT(errorString.c_str());
			return nullptr;
		}
	}
	
	Texture* TextureFactory::LoadTexture(const std::string&	aFilePath, const eTextureType aTextureType)
	{
		START_LOG_TIMER();

		std::string fileName = WF::GetFileNameWithoutExtension(aFilePath);

		DirectXEngine* theEngine = Engine::GetInstance()->GetEngine();
		if (myTextures.KeyExists(fileName) == false)
		{
			Texture newTexture(theEngine, Engine::GetInstance()->GetSetupInfo().myResolution, DXGI_FORMAT_R8G8B8A8_UNORM, true, true, false);

			int index = aFilePath.rfind(".");
			if (aFilePath.substr(index, aFilePath.length() - index) == ".dds")
			{
				newTexture.LoadTexture(aFilePath, aTextureType, myDevice);
			}
			else
			{
				newTexture = myTextures["pink"];
			}


			myLoadedTextures.Add(fileName);
			myTextures[fileName] = newTexture;
		}
		
		float timeSpent = END_LOG_TIMER();
		CU::String<200> message = "\nLoaded texture with name: ";
		message += fileName.c_str();
		message += "\nTime spent : ";
		message += timeSpent;
		message += " milliseconds.";
		RESOURCE_LOG(message.c_str());

		return &myTextures.Get(fileName);
	}

	void TextureFactory::LoadAllTextures()
	{
		CU::GrowingArray<std::string> files = WF::GetFilesWithExtension("Data\\", ".dds");

		for (unsigned short i = 0; i < files.Size(); i++)
		{
			LoadTexture(files[i], eTextureType::DIFFUSE);
		}
	}

	void TextureFactory::UnloadTextures()
	{
		myTextures.Clear();
		myLoadedTextures.RemoveAll();
	}

}




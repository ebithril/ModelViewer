#include "Engine_Precompiled.h"
#include "Engine.h"
#include "DirectXEngine.h"
#include "../Common Utilities/DL_Debug.h"
#include "GfxFactoryWrapper.h"
#include "VertexDataWrapper.h"
#include "TextureType.h"
#include "EffectFactory.h"
#include "ModelFactory.h"
#include "TextureFactory.h"
#include "Texture.h"
#include "Effect.h"
#include "EmitterFactory.h"
#include "StreakFactory.h"

namespace GraphicsEngine
{
	Engine* Engine::ourInstance = nullptr;

	bool Engine::Create(HWND& aHwnd, WNDPROC aWindowProc, const SetupInfo& aInfoArgument)
	{
		if (ourInstance != nullptr)
		{
			DL_ASSERT("Engine already created.");
		}
		GfxFactoryWrapper::Create();
		ourInstance = new Engine();
		ourInstance->Init(aHwnd, aWindowProc, aInfoArgument);

		ENGINE_LOG("Engine created succesfully.");

		return true;
	}

	Engine* Engine::GetInstance()
	{
		InitTrueTest();
		return ourInstance;
	}

	//TEST
	DirectXEngine* Engine::GetEngine()
	{
		return myDirectX;
	}

	void Engine::Destroy()
	{
		SAFE_DELETE(ourInstance);
	}

	HWND& Engine::GetWindowHandle()
	{
		return myDirectX->GetWindowHandle();
	}

	void Engine::RenderScenes()
	{
		myDirectX->RenderScenes();
	}

	const SetupInfo& Engine::GetSetupInfo() const
	{
		return myDirectX->GetSetupInfo();
	}

	void Engine::InitTrueTest()
	{
		if (ourInstance == nullptr)
		{
			DL_ASSERT("Engine not initiated");
		}
	}

	Engine::Engine()
	{
		myDirectX = nullptr;
		ourInstance = nullptr;
	}

	Engine::~Engine()
	{
		myDirectX->ShutDown();
		delete myDirectX;
	}

	void Engine::Init(HWND& aHwnd, WNDPROC aWindowProc, const SetupInfo& aInfoArgument)
	{
		myDirectX = new DirectXEngine();
		myDirectX->Init(aHwnd, aWindowProc, aInfoArgument);

		GfxFactoryWrapper::GetInstance()->Init(myDirectX->GetDevice());

		myDirectX->InitRenderer();
	}
}
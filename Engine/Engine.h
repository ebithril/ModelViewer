#pragma once
#include <Windows.h>
#include <string>
#include "SceneRenderData.h"

namespace GraphicsEngine
{
	class DirectXEngine;
	struct SetupInfo;
	struct SceneRenderData;

	class Engine
	{
	public: 
		static bool Create(HWND& aHwnd, WNDPROC aWindowProc, const SetupInfo& aInfoArgument);
		static Engine* GetInstance();
		void Destroy();
		void RenderScenes();
		const SetupInfo& GetSetupInfo() const;

		//TEST
		DirectXEngine* GetEngine();

		HWND& GetWindowHandle();

	private:
		Engine();
		~Engine();

		void Init(HWND& aHwnd, WNDPROC aWindowProc, const SetupInfo& aInfoArgument);
		static Engine* ourInstance;
		DirectXEngine* myDirectX;
		static void InitTrueTest();

	
	};
}

namespace GE = GraphicsEngine;


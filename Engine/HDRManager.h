#pragma once
#include "../Common Utilities/StaticArray.h"

struct ID3D11RenderTargetView;

namespace GraphicsEngine
{
	class FullScreenHelper;
	class DispatchHelper;
	class Texture;
	class DirectXEngine;

	class HDRManager
	{
	public:
		HDRManager();
		~HDRManager();

		void Init(int aWidth, int aHeight, FullScreenHelper* aPostProcesser, DispatchHelper* aDispatcher, DirectXEngine* aDirectXEngine);
		void Process(COMObjectPointer<ID3D11RenderTargetView>& aDestination, Texture* aSource);
		void OnResize(const Vector2<int>& aResolution);
	private:
		int myWidth;
		int myHeight;

		Texture** myDownSampleTargets;
		int myTargetCount;

		CU::StaticArray<Texture*, 3> myOldLuminanceTargets;
		CU::StaticArray<Texture*, 3> myAdaptedLuminanceTargets;

		FullScreenHelper* myPostProcess;
		DispatchHelper* myDispatcher;
		DirectXEngine* myDirectXEngine;
	};

}
namespace GE = GraphicsEngine;
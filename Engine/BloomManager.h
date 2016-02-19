#pragma once

#include "../Common Utilities/Vector.h"
struct ID3D11RenderTargetView;

namespace GraphicsEngine
{
	class Texture;
	class FullScreenHelper;
	class DispatchHelper;
	class DirectXEngine;
	class BloomManager
	{
	public:
		BloomManager();
		~BloomManager();

		void Init(int aWidth, int aHeight, FullScreenHelper* aPostProcess, DispatchHelper* aDispatcher, DirectXEngine* anEngine);
		void Process(COMObjectPointer<ID3D11RenderTargetView>& aRenderTargetView, Texture* aSource);
		void OnResize(const Vector2<int>& aResolution);
		void Blur(Texture* aSource, Texture* aBounce);

	private:
		void DownSample(Texture* aSource);

		int myWidth;
		int myHeight;


		Texture* myBlurTarget;
		Texture* myDownSampleTarget2X2;
		Texture* myDownSampleBlurTarget2X2;
		Texture* myDownSampleTarget4X4;
		Texture* myDownSampleBlurTarget4X4;
		Texture* myDownSampleTarget16X16;
		Texture* myDownSampleBlurTarget16X16;

		DirectXEngine* myDirectXEngine;
		FullScreenHelper* myPostProcess;
		DispatchHelper* myDispatcher;
	};

}
namespace GE = GraphicsEngine;
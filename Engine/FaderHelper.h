#pragma once
#include "../Common Utilities/Vector.h"
#include "../Common Utilities/Observer.h"
struct ID3D11RenderTargetView;


namespace GraphicsEngine
{

	enum class eFadeTypes
	{
		FADE_IN,
		FADE_OUT
	};

	class Texture;
	class FullScreenHelper;
	class DispatchHelper;
	class DirectXEngine;
	class FaderHelper : CU::Observer
	{
	public:
		FaderHelper();
		~FaderHelper();

		void Init(int aWidth, int aHeight, FullScreenHelper* aPostProcess, DispatchHelper* aDispatcher, DirectXEngine* anEngine);
		void Process(COMObjectPointer<ID3D11RenderTargetView>& aRenderTargetView, Texture* aSource);
		void OnResize(const Vector2<int>& aResolution);

	private:

		bool HandleFadeInEvent(const CU::PoolPointer<CU::Event>& anEvent);
		bool HandleFadeOutEvent(const CU::PoolPointer<CU::Event>& anEvent);

		int myWidth;
		int myHeight;
		float myFadeAmount;
		float myFadeTime;
		float myFadeDeltaTime;

		bool myIsFading;

		eFadeTypes myFadeType;
		Vector3<float> myFadeColor;


		Texture* myFadedTarget;

		DirectXEngine* myDirectXEngine;
		FullScreenHelper* myPostProcess;
		DispatchHelper* myDispatcher;
	};


}
namespace GE = GraphicsEngine;
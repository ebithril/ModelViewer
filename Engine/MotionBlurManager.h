#pragma once

#pragma once

#include "../Common Utilities/Vector.h"
struct ID3D11RenderTargetView;

namespace GraphicsEngine
{
	class Texture;
	class FullScreenHelper;
	class DirectXEngine;

	class MotionBlurManager
	{
	public:
		MotionBlurManager();
		~MotionBlurManager();

		void Init(FullScreenHelper* aPostProcess, DirectXEngine* anEngine);
		void Process(COMObjectPointer<ID3D11RenderTargetView>& aRenderTargetView, Texture* aSource, Texture* aVelocityTexture);

	private:

		DirectXEngine* myDirectXEngine;
		FullScreenHelper* myPostProcess;
	};

}
namespace GE = GraphicsEngine;

#pragma once
#include "Effect.h"

namespace GraphicsEngine
{
	class Effect;
	class Texture;
	class DirectXEngine;
	class DispatchHelper
	{
	public:
		DispatchHelper();
		~DispatchHelper();
		bool Init(DirectXEngine* aDirectXEngine);
		void Dispatch(Texture* aTarget, Texture* aSource, std::string aTechnique, int xDispatch, int yDispatch);
	private:

		Effect* myComputeEffect;
		DirectXEngine* myDirectXEngine;
	};
}
namespace GE = GraphicsEngine;

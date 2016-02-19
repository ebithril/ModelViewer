#pragma once
#include "VertexBufferWrapper.h"
#include "VertexDataWrapper.h"

struct ID3D11RenderTargetView;
struct ID3D11ShaderResourceView;

namespace GraphicsEngine
{
	class Effect;
	class Texture;
	class DirectXEngine;

	class FullScreenHelper
	{
	public:
		FullScreenHelper();
		~FullScreenHelper();

		bool Init(DirectXEngine* aDevice);

		void Process(std::string aTechniqueName, Texture* aTargetTexture, Texture* aSourceTexture);
		void Process(std::string aTechniqueName, COMObjectPointer<ID3D11RenderTargetView>& aTarget, Texture* aSourceTexture);
		void Process(std::string aTechniqueName, COMObjectPointer<ID3D11RenderTargetView>& aTarget, int aWidth, int aHeight, Texture* aSourceTexture);
		void Process(std::string aTechniqueName, COMObjectPointer<ID3D11RenderTargetView>& aTarget, int aWidth, int aHeight, Texture* aSourceTexture, float aFadeAmount);
		Effect* GetEffect();

		void SetFadeColor(const Vector3<float> aFadeColor);

	private:
		bool InitVertexBuffer();
		void RenderQuad(std::string aTechniqueName, COMObjectPointer<ID3D11RenderTargetView>& aTarget, const Vector2<int>& aTargetSize, COMObjectPointer<ID3D11ShaderResourceView>& aSource, const Vector2<int>& aSourceSize, float aFadeAmount);

		Effect* myEffect;
		VertexDataWrapper myVertexDataWrapper;
		VertexBufferWrapper myVertexBufferWrapper;
		Vector3<float> myFadeColor;

		DirectXEngine* myDirectXEngine;

	};
}
namespace GE = GraphicsEngine;
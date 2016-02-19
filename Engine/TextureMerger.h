#pragma once
#include "VertexBufferWrapper.h"
#include "VertexDataWrapper.h"
#include <string>
#include "../Common Utilities/Vector.h"

struct ID3D11InputLayout;
namespace GraphicsEngine
{
	class Effect;
	class Texture;
	class DirectXEngine;

	struct TextureVertex
	{
		Vector3f myPosition;
		Vector2f myUV;
	};

	class TextureMerger
	{
	public:
		TextureMerger();
		~TextureMerger();

		static void Create(Effect* anEffect);
		static TextureMerger* GetInstance();

		void Init(Effect* anEffect);
		void MergeTextures(Texture* aDestination, Texture* aFirstTexture, Texture* aSecondTexture, std::string aTechnique);


	private:
		Effect* myEffect;
		VertexBufferWrapper myVertexBuffer;
		VertexDataWrapper myVertexData;
		TextureVertex myTextureVertices[6];
		ID3D11InputLayout* myVertexLayout;

		static TextureMerger* ourInstance;

	};
}


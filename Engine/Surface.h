#pragma once
#include "../Common Utilities/StaticArray.h"
#include "TextureType.h"

enum D3D_PRIMITIVE_TOPOLOGY;

struct ID3DX11EffectShaderResourceVariable;

namespace GraphicsEngine
{
	class Texture;
	class Effect;

	class Surface
	{
	public:

		Surface();
		~Surface();

		void Activate();
		int GetIndexCount()const;
		int GetIndexStart()const;
		int GetVertexCount()const;
		int GetVertexStart()const;
		const D3D_PRIMITIVE_TOPOLOGY& GetPrimologyType() const;

		void SetEffect(Effect* anEffect);
		void SetIndexCount(const int anIndexCount);
		void SetIndexStart(const int anIndexStart);
		void SetVertexStart(const int aVertexStart);
		void SetVertexCount(const int aVertexCount);
		void SetPrimologyType(const D3D_PRIMITIVE_TOPOLOGY& aTopology);
		void SetTexture(Texture* aTexture, const eTextureType aTextureType, bool anAsOriginalTexture = true);
		void SetTextTexture(Texture* aTexture);

		const CU::StaticArray<Texture*, static_cast<int>(eTextureType::NR_OF_TEXTURETYPES)> GetTextures() const;

	private:

		int myIndexStart;
		int myIndexCount;

		int myVertexStart;
		int myVertexCount;

		bool myHasRenderedText;

		CU::StaticArray<Texture*, static_cast<int>(eTextureType::NR_OF_TEXTURETYPES)> myTextures;
		Texture* myOriginalTexture;
		Texture* myTextTexture;

		Effect* myEffect;
		D3D_PRIMITIVE_TOPOLOGY myPrimitiveTopology;
	};
}

namespace GE = GraphicsEngine;
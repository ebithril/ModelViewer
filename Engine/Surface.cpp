#include "Engine_Precompiled.h"
#include "Surface.h"
#include "d3dx11effect.h"
#include "Effect.h"
#include "Texture.h"

#include "Engine.h"
#include "DirectXEngine.h"

namespace GraphicsEngine
{
	Surface::Surface()
		: myPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	{
		myHasRenderedText = true;
	}

	Surface::~Surface()
	{
	}

	void Surface::Activate()
	{	
		if (myEffect != nullptr)
		{
			if (myHasRenderedText == false)
			{
				myTextures[static_cast<int>(eTextureType::DIFFUSE)] = myTextTexture;
			}
			myEffect->SetTextures(myTextures);
		}
	}

	int Surface::GetIndexCount() const
	{
		return myIndexCount;
	}

	int Surface::GetIndexStart() const
	{
		return myIndexStart;
	}

	int Surface::GetVertexCount() const
	{
		return myVertexCount;
	}

	void Surface::SetEffect(Effect* anEffect)
	{
		myEffect = anEffect;
	}

	void Surface::SetIndexCount(int aIndexCount)
	{
		myIndexCount = aIndexCount;
	}

	void Surface::SetIndexStart(const int anIndexStart)
	{
		myIndexStart = anIndexStart;
	}

	void Surface::SetPrimologyType(const D3D_PRIMITIVE_TOPOLOGY& aTopology)
	{
		myPrimitiveTopology = aTopology;
	}

	const D3D_PRIMITIVE_TOPOLOGY& Surface::GetPrimologyType() const
	{
		return myPrimitiveTopology;
	}

	void Surface::SetTexture(Texture* aTexture, const eTextureType aTextureType, bool anAsOriginalTexture)
	{
		if (anAsOriginalTexture == true)
		{
			myOriginalTexture = aTexture;
		}
		myTextures[static_cast<int>(aTextureType)] = aTexture;

		if (aTexture == nullptr)
		{
			int a;
			a = 10;
		}
	}

	void Surface::SetTextTexture(Texture* aTexture)
	{
		myTextTexture = aTexture;
		myHasRenderedText = false;
	}

	void Surface::SetVertexStart(int aVertexStart)
	{
		myVertexStart = aVertexStart;
	}

	void Surface::SetVertexCount(const int aVertexCount)
	{
		myVertexCount = aVertexCount;
	}

	const CU::StaticArray<Texture*, static_cast<int>(eTextureType::NR_OF_TEXTURETYPES)> Surface::GetTextures() const
	{
		return myTextures;
	}
}



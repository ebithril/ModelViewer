#pragma once

#include "Model.h"
#include "../Engine/Instance.h"

namespace GraphicsEngine
{
	// TODO: Alter sprite to send position and size as instance-data and let the GPU handle scaling and positioning.
	// TODO: Split up into Quad and Sprite classes, similar to Haaf's Game Engine.

	class Sprite
	{
	public:
		struct SpriteVertex
		{
			Vector2f myPosition;
			Vector2f myTextureCoordinates;
			Vector4f myColor;
		};

	public:
		Sprite();
		~Sprite();

		void Render();
		void Unload();

		void SetTexture(Texture* aTexture);
		void SetPosition(const Vector2f& aSpritePosition);
		void SetSize(const Vector2f& aSpriteSize);
		void SetColor(const Vector4f& aSpriteColor);

		const Vector2f& GetPosition() const;
		const Vector2f& GetSize() const;
		const Vector4f& GetColor() const;

	private:
		const CU::PoolPointer<Model> GenerateSpriteMesh();
		void GenerateVertices(SpriteVertex** aVertexArrPtr, unsigned long& aVertexCount);
		void GenerateIndices(unsigned int** anIndexArrPtr, unsigned long& anIndexCount);

		Vector2f
			myPosition,
			mySize;

		Vector4f
			myColor;


		Instance mySpriteInstance;
		GraphicsEngine::Texture* myTexture;
		GraphicsEngine::Texture* myDefaultTexture;
	};
}
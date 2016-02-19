#include "Engine_Precompiled.h"
#include "Sprite.h"
#include "../Common Utilities/Vector.h"

#include "VertexIndexWrapper.h"
#include "VertexDataWrapper.h"
#include "TerrainInstance.h"

#include "../Common Utilities/RenderCommandInstance.h"
#include "../Common Utilities/EventManager.h"

#include "EffectFactory.h"
#include "ModelFactory.h"
#include "TextureFactory.h"

#include "Engine.h"
#include "SetupInfo.h"

using namespace GraphicsEngine;

Sprite::Sprite()
	: mySpriteInstance(GenerateSpriteMesh())
{
	myDefaultTexture = GE::GfxFactoryWrapper::GetInstance()->GetTextureFactory()->GetTexture("White");

	SetPosition({ 10, 10 });
	SetSize({ 200, 500 });
	SetColor({ 1, 1, 1, 1 });
}

Sprite::~Sprite()
{

}

void Sprite::Render()
{
	mySpriteInstance.Update();

	Matrix44f orientation;

	Vector3f position(myPosition.x, myPosition.y, 0.f);
	Vector3f size(mySize.x, mySize.y, 1);

	Vector2<int> resolution = Engine::GetInstance()->GetSetupInfo().myResolution;
	Vector3f resolutionF = Vector3f(static_cast<float>(resolution.x), static_cast<float>(resolution.y), 1.f);

	if (position.x > 1 || position.y > 1)
		position /= resolutionF;

	if (size.x > 1 || size.y > 1)
		size /= resolutionF;

	orientation.Scale(size);
	orientation.SetTranslation(position);

	CU::RenderCommandInstance cmd;
	cmd.myModel = &mySpriteInstance;
	if (myTexture)
	{
		cmd.myDiffuseTexture = myTexture;
	}
	else
	{
		cmd.myDiffuseTexture = myDefaultTexture;
	}
	cmd.myDebugColor = myColor;
	cmd.myOrientation = orientation;
	cmd.myLastFrameOrientation = orientation;
	cmd.myEmmisiveTexture = nullptr;
	CU::EventManager::GetInstance()->AddRenderCommand(cmd);
}

void Sprite::Unload()
{
	myTexture = nullptr;
	SetPosition({ 10, 10 });
	SetSize({ 200, 500 });
	SetColor({ 1, 1, 1, 1 });
}

void Sprite::SetTexture(Texture* aTexture)
{
	myTexture = aTexture;
}

void Sprite::SetPosition(const Vector2f& aSpritePosition)
{
	myPosition = aSpritePosition;
}

void Sprite::SetSize(const Vector2f& aSpriteSize)
{
	mySize = aSpriteSize;
}

void Sprite::SetColor(const Vector4f& aSpriteColor)
{
	myColor = aSpriteColor;
}

const Vector2f& Sprite::GetPosition() const
{
	return myPosition;
}

const Vector2f& Sprite::GetSize() const
{
	return mySize;
}

const Vector4f& Sprite::GetColor() const
{
	return myColor;
}




const CU::PoolPointer<Model> Sprite::GenerateSpriteMesh()
{
	GE::VertexDataWrapper vertexData;
	GE::VertexIndexWrapper indexData;

	SpriteVertex* vertexArr = nullptr;
	unsigned int* indexArr = nullptr;

	unsigned long vertexCount = 0;
	unsigned long indexCount = 0;

	GenerateVertices(&vertexArr, vertexCount);
	GenerateIndices(&indexArr, indexCount);

	vertexData.myNumberOfVertexes = vertexCount;
	vertexData.mySize = sizeof(SpriteVertex) * vertexCount;
	vertexData.myStride = sizeof(SpriteVertex);
	vertexData.myVertexData = reinterpret_cast<char*>(vertexArr);
	vertexData.myType = GE::eVertexType::VERTEX_SPRITE_POS_COL_UV;

	indexData.myFormat = DXGI_FORMAT_R32_UINT;
	indexData.myNumberOfIndexes = indexCount;
	indexData.mySize = sizeof(unsigned int) * indexCount;
	indexData.myIndexData = reinterpret_cast<char*>(indexArr);

	GE::Effect* spriteEffect = GE::GfxFactoryWrapper::GetInstance()->GetEffectFactory()->GetEffect("SpriteShader");
	GE::ModelFactory& modelFactory = *GE::GfxFactoryWrapper::GetInstance()->GetModelFactory();
	CU::PoolPointer<Model> model = modelFactory.CreateUnsearchableModelFromBufferData(vertexData, indexData);
	model->SetEffect(spriteEffect);
	return model;
}

void Sprite::GenerateVertices(SpriteVertex** aVertexArrPtr, unsigned long& aVertexCount)
{
	aVertexCount = 4;
	(*aVertexArrPtr) = new SpriteVertex[aVertexCount];

	SpriteVertex* v = *aVertexArrPtr;

	Vector2f usedPosition(0, 0);
	Vector2f usedSize(1, 1);

	v[0].myPosition = usedPosition;
	v[1].myPosition = v[0].myPosition + Vector2f(usedSize.x, 0);
	v[2].myPosition = v[0].myPosition + Vector2f(0, usedSize.y);
	v[3].myPosition = v[0].myPosition + Vector2f(usedSize.x, usedSize.y);

	for (char i = 0; i < 4; ++i)
	{
		v[i].myColor = { 1, 1, 1, 1 };
	}

	v[0].myTextureCoordinates = { 0, 0 };
	v[1].myTextureCoordinates = { 1, 0 };
	v[2].myTextureCoordinates = { 0, 1 };
	v[3].myTextureCoordinates = { 1, 1 };
}

void Sprite::GenerateIndices(unsigned int** anIndexArrPtr, unsigned long& anIndexCount)
{
	anIndexCount = 6;
	(*anIndexArrPtr) = new unsigned int[anIndexCount];

	unsigned int* i = *anIndexArrPtr;

	i[0] = 0;
	i[1] = 1;
	i[2] = 2;

	i[3] = 1;
	i[4] = 3;
	i[5] = 2;
}
#pragma once

#include "fbxsdk.h"
#include <vector>
#include "../Common Utilities/Matrix.h"
#include "../Common Utilities/GrowingArray.h"

#define FBXLoaderArray CU::GrowingArray


enum FBXTextureType
{
	DIFFUSE,
	NORMALMAP,
	ROUGHNESS,
	SUBSTANCE,
	AO,
	EMISSIVE,
	NR_OF_TEXTURETYPES,
};

struct TextureInfo
{
	std::string myFileName;
	FBXTextureType myType;
};

struct KeyFrame
{
	float myTime;
	Matrix44f myMatrix;
};

struct Bone 
{
	std::string myName;
	float myAnimationTime;
	int myId;
	Matrix44f myBaseOrientation;
	Matrix44f myBindMatrix;
	std::vector<KeyFrame> myFrames;
	std::vector<int> myChilds;
};

struct AnimationData
{
	std::string myName;

	Matrix44f myBindMatrix;
	int myRootBone;
	std::vector<Bone> myBones;
};

struct ModelData
{
	enum
	{
		VERTEX_VBO,
		INDEX_VBO,
		VBO_COUNT,
	};

	enum LayoutType
	{
		VERTEX_POS,
		VERTEX_NORMAL,
		VERTEX_UV,
		VERTEX_BINORMAL,
		VERTEX_TANGENT,
		VERTEX_SKINWEIGHTS,
		VERTEX_BONEID,
	};

	struct Layout
	{
		int mySize;
		int myOffset;
		LayoutType myType;
	};

	// For every material, record the offsets in every VBO and triangle counts
	struct SubMesh
	{
		SubMesh() : IndexOffset(0), TriangleCount(0) {}

		int IndexOffset;
		int TriangleCount;
	};

	float* myVertexBuffer;
	unsigned int* myIndicies;
	int myVertexCount;
	int myVertexStride;
	int myIndexCount;

	std::vector<TextureInfo> myTextures;
	FBXLoaderArray<Layout> myLayout;
	FBXLoaderArray<SubMesh*> mSubMeshes;

	bool myHasBiNormal;
	bool myHasSkinweights;
	bool myHasTangents;
	bool mHasNormal;
	bool mHasUV;
	bool mAllByControlPoint;

	ModelData();
};

	

struct TextureData
{
	std::vector<TextureInfo> myTextures;
};

struct AnimationCurves
{
	FbxAnimCurve* myRotationCurve[4];
	FbxAnimCurve* myScalingCurve[3];
	FbxAnimCurve* myTtranslationCurve[3];
};

enum ELightType
{
	ESpotLight,
	EDirectionalLight,
	EPointLight,
};

struct Light
{
	ELightType myType;
	Vector3<float> myColor;
	Vector3<float> myDirection;
	Matrix44f myOrientation;
	float myIntensity;
	float myInnerAngle;
	float myOuterAngle;
};

struct LodGroup
{
	int myDisplayLevels;
	CU::GrowingArray<float> myThreashHolds;
};

struct Camera
{
	float myFov;
};

class FbxModelData
{
public:
	FbxModelData()
	{
		myData = nullptr;
		myLodGroup = nullptr;
		myAnimation = nullptr;
		myTextureData = nullptr;
		myLight = nullptr;
		myCamera = nullptr;
	}

	~FbxModelData();

	ModelData* myData;
	LodGroup* myLodGroup;
	Light* myLight;
	Camera* myCamera;
	AnimationData* myAnimation;
	TextureData* myTextureData;
	AnimationCurves* myAnimationCurves;
	FBXLoaderArray<	KeyFrame > myAnimatedOrientation;
	Matrix44f myOrientation;
	Matrix44f myRotationPivot;
	FBXLoaderArray<FbxModelData*> myChilds;
};

class FBXLoader
{
public:
	FBXLoader();
	~FBXLoader();

	FbxModelData* loadModel( const char* aFile );
private:
	FbxScene* LoadScene(const char* aFile);
	FbxModelData* myLoadingModel;

	FbxIOSettings* mySettings;
	FbxManager* myFbxManager;

};

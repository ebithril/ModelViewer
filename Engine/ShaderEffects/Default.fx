#define PI 3.14159
#define NUMBER_OF_POINT_AND_SPOT_LIGHTS 10

matrix myBones[32];

cbuffer PerObjectDataBuffer
{
	matrix myToWorldMatrix;
	matrix myWorldViewProjection;
	matrix myLastFrameToWorldMatrix;
	matrix myLastFrameWorldViewProjectionMatrix;
	float4 myDebugColor;
	float4 myCutOffValue;
};

cbuffer PerFrameCameraBuffer
{
	matrix myToViewMatrix;
	float3 myCameraPosition;
	float myTotalTime;
	float3 myViewDirection;
	float myCameraPadding;
};

cbuffer PerFrameLightBuffer
{
	float4 myDirectionalLightColor;
	float3 myDirectionalLightDirection;
	float myPadding;
	matrix myDirectionalLightViewProjection;
};

cbuffer OnResizeBuffer
{
	matrix myProjectionMatrix;
}

Texture2D DiffuseTexture;
Texture2D NormalTexture;
Texture2D RoughnessTexture;
Texture2D SubstanceTexture;
Texture2D AOTexture;
Texture2D EmissiveTexture;
Texture2D DirectionalShadowMap;
TextureCube AmbientCubeMap;

Texture2D FogOfWarTexture;
Texture2D BlackMaskTexture;
float TerrainSize;
float3 TerrainPosition;

struct VertexInput
{
	float3 myPosition : POSITION;
	float myPadding1 : PSIZE0;
	float3 myNormal : NORMAL;
	float myPadding2 : PSIZE1;
	float2 myTexturePosition : TEXCOORD0;
	float2 myPadding3 : TEXCOORD1;
	float3 myBiNormal : BINORMAL;
	float myPadding4 : PSIZE2;
	float3 myTangent : TANGENT;
	float myPadding5 : PSIZE3;
	float4 myBoneWeights : WEIGHTS;
	int4 myBoneIndecies : BONES;
};

struct PixelInput
{
	float4 myPosition : SV_POSITION;
	float4 myLastFramePosition : POSITION0;
	float4 myWorldPosition : TEXCOORD0;
	float4 myLastFrameWorldPosition : POSITION2;
	float4 myPixelPosition : POSITION3;
	float4 myDirectionalLightShadowCoordinates : TEXCOORD1;
	float3 myNormal : NORMAL;
	float myPadding1 : FOG0;
	float3 myBiNormal : BINORMAL;
	float3 myViewDirection : TEXCOORD2;
	float myPadding2 : FOG1;
	float3 myTangent : TANGENT;
	float myPadding3 : FOG2;
	float2 myTexturePosition : TEXCOORD3;
	float2 myPadding4 : TEXCOORD4;
	float3 myPixelPositionInProjectionSpace : TEXCOORD5;
	float4 myPixelPositionOnTerrain : POSITION15;
};

struct PixelData
{
	float4 myAlbedo;
	float3 myMetalnessAlbedo;
	float3 myNormal;
	float3 myMetalness;
	float3 mySubstance;
	float3 myAmbientOcclusion;
	float myRoughness;
	float myRoughnessOffsetted;
};

struct ReturnData
{
	float4 myColor;
	float2 myVelocity;
};

//--------------------------------------------------------------------------------------
// States
//--------------------------------------------------------------------------------------
DepthStencilState EnableDepth
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
	DepthFunc = LESS_EQUAL;
};

DepthStencilState DisableDepth
{
	DepthEnable = FALSE;
	DepthWriteMask = ZERO;
	DepthFunc = LESS_EQUAL;
};

DepthStencilState DisableDepthWrite
{
	DepthEnable = TRUE;
	DepthWriteMask = ZERO;
	DepthFunc = LESS_EQUAL;
};

BlendState NoBlending
{
	AlphaToCoverageEnable = FALSE;
	BlendEnable[0] = FALSE;
};

BlendState NoBlend
{
	BlendEnable[0] = FALSE;
};

BlendState Shield_Health_Blending
{
	BlendEnable[0] = TRUE;
	SrcBlend = DEST_COLOR;
	DestBlend = SRC_COLOR;
	BlendOp = ADD;
	SrcBlendAlpha = ZERO;
	DestBlendAlpha = ZERO;
	BlendOpAlpha = ADD;
	RenderTargetWriteMask[0] = 0x0F;

};


BlendState AlphaBlend
{
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
	DestBlend = INV_SRC_ALPHA;
	BlendOp = ADD;
	SrcBlendAlpha = ZERO;
	DestBlendAlpha = ZERO;
	BlendOpAlpha = ADD;
	RenderTargetWriteMask[0] = 0x0F;
};

BlendState BLEND_THIS_SHIT
{
	BlendEnable[0] = TRUE;
	SrcBlend = ONE;
	DestBlend = ONE;
	BlendOp = ADD;
	SrcBlendAlpha = ONE;
	DestBlendAlpha = ONE;
	BlendOpAlpha = ADD;
	RenderTargetWriteMask[0] = 0x0F;
};


SamplerState DefaultSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
	AddressW = WRAP;
};

SamplerState ClampSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
	AddressW = CLAMP;
};

SamplerState BorderBlackSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = BORDER;
	AddressV = BORDER;
	AddressW = BORDER;
	BorderColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
};

SamplerComparisonState PercentageClosureFilteringSampler
{
	Filter = COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	AddressU = BORDER;
	AddressV = BORDER;
	AddressW = BORDER;
	BorderColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

	ComparisonFunc = LESS_EQUAL;
};

//--------------------------------------------------------------------------------------
// Functions
//--------------------------------------------------------------------------------------

matrix IdentityMatrix()
{
	return matrix
		(1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
}

matrix RotateAroundZ(float aRadians)
{
	float cosinus = cos(aRadians);
	float sinus = sin(aRadians);

	return matrix(
		cosinus, sinus, 0, 0,
		-sinus, cosinus, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
		);
}

PixelData GetPixelData(PixelInput aPixelInput)
{
	PixelData returnData;
	returnData.myAlbedo = DiffuseTexture.Sample(DefaultSampler, aPixelInput.myTexturePosition);
	returnData.myMetalness = SubstanceTexture.Sample(DefaultSampler, aPixelInput.myTexturePosition).xyz;
	returnData.myRoughness = RoughnessTexture.Sample(DefaultSampler, aPixelInput.myTexturePosition).x;

	float3 normalMap = normalize((NormalTexture.Sample(DefaultSampler, aPixelInput.myTexturePosition).xyz * 2) - 1);

	float3x3 tangentMatrix = (float3x3(normalize(aPixelInput.myTangent), normalize(aPixelInput.myBiNormal), normalize(aPixelInput.myNormal)));

	returnData.myNormal = normalize(mul(normalize(normalMap), tangentMatrix));
	//returnData.myNormal = aPixelInput.myNormal;
	//returnData.myNormal = normalize(aPixelInput.myNormal);

	returnData.myAmbientOcclusion = AOTexture.Sample(DefaultSampler, aPixelInput.myTexturePosition).xyz;
	//returnData.myAmbientOcclusion = 1;

	returnData.mySubstance = float3((0.04 - 0.04 * returnData.myMetalness) + returnData.myAlbedo.xyz * returnData.myMetalness);
	returnData.myMetalnessAlbedo = returnData.myAlbedo.xyz - returnData.myAlbedo.xyz * returnData.myMetalness;

	returnData.myRoughnessOffsetted = pow(8192, returnData.myRoughness);

	return returnData;
}

float2 CalculateVelocity(PixelInput anInput)
{
	float2 a = (anInput.myPixelPosition.xy / anInput.myPixelPosition.w);
		float2 b = (anInput.myLastFramePosition.xy / anInput.myLastFramePosition.w);
	return a - b;
}

RasterizerState BackFaceCulling
{
	CullMode = 3;
};

RasterizerState Solid
{
	CullMode = BACK;
	FillMode = SOLID;
};

RasterizerState NoCull
{
	CullMode = NONE;
	FillMode = SOLID;
};

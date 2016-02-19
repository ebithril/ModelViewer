#define NUMBER_OF_POINT_LIGHTS 6
#define NUMBER_OF_SPOT_LIGHTS 6

#define NUMBER_OF_SPLAT_MAPS 2

#define PI 3.14159265358979323846264f

Texture2D FogOfWarTexture;
Texture2D BlackMaskTexture;
Texture2D SplatMaps[NUMBER_OF_SPLAT_MAPS];
Texture2D SplatTextures[NUMBER_OF_SPLAT_MAPS * 4];
float2 SplatTextureSizes[NUMBER_OF_SPLAT_MAPS * 4];
float TerrainSize;
float3 TerrainPosition;

cbuffer PerObjectDataBuffer
{
	matrix myToWorldMatrix;
	matrix myWorldViewProjection;
	matrix myLastFrameToWorldMatrix;
	matrix myLastFrameWorldViewProjectionMatrix;
	float4 myDebugColor;
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
	float4 myAmbientLightColor;
	float4 myLightColor;
	float3 myLightDirection;
	float mySpecularPower;
	float4 mySpecularColor;
};

cbuffer PerObjectPointLightBuffer
{
	float4 myPointLightColor[NUMBER_OF_POINT_LIGHTS];
	float4 myPointLightPositionAndRange[NUMBER_OF_POINT_LIGHTS];
};

cbuffer PerObjectSpotLightBuffer
{
	float4 mySpotLightColor[NUMBER_OF_SPOT_LIGHTS];
	float4 mySpotLightPositionAndRange[NUMBER_OF_SPOT_LIGHTS];
	float4 mySpotLightDirectionAndAngle[NUMBER_OF_SPOT_LIGHTS];
	matrix myDepthBiasViewProjection[NUMBER_OF_SPOT_LIGHTS];
};

cbuffer OnResizeBuffer
{
	matrix myProjectionMatrix;
}

Texture2D SpotLightShadowMaps[NUMBER_OF_SPOT_LIGHTS];

RasterizerState Solid
{
	FillMode = SOLID;
	CullMode = BACK;
};
RasterizerState NoCulling
{
	FillMode = SOLID;
	CullMode = NONE;
};

RasterizerState Wireframe
{
	FillMode = WIREFRAME;
	CullMode = NONE;
};

SamplerState DefaultSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
	AddressW = WRAP;
};

SamplerState PointSampler
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = WRAP;
	AddressV = WRAP;
	AddressW = WRAP;
};

DepthStencilState EnableDepth
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
	DepthFunc = LESS_EQUAL;
};

BlendState NoBlend
{
	BlendEnable[0] = FALSE;
};

struct VertexInput
{
	float3 myPosition : POSITION;
	float2 myUV : TEXCOORD;
};

struct GeometryInput
{
	float4 myPosition : POSITION;
	float myTeam : TEXCOORD1;
};

struct PixelInput
{
	float4 myPosition : SV_POSITION;
	float4 myCenterPosition : POSITION;
	float4 myPixelPositionOnTerrain : POSITION2;
	float2 myTextureCoordinates : TEXCOORD0;
	float myTeam : TEXCOORD1;
};
GeometryInput VertexShaderFunction(VertexInput anInput)
{
	GeometryInput output = (GeometryInput)0;

	float3 pos = (anInput.myPosition - TerrainPosition) / float3(TerrainSize, 1, TerrainSize);

	//pos.x *= 0.695f;
	//pos.x += 0.175f;
	//
	//pos.z *= 0.72f;
	//pos.z += 0.175f;

	pos.y = pos.z;
	pos.z = 0.5f;
	pos *= 2.0f;
	pos -= float3(1, 1, 0);
	pos.y = -pos.y;

	output.myPosition = float4(pos, 1);
	output.myTeam = anInput.myUV.x;

	return output;
}

[maxvertexcount(45)]
void GS(point GeometryInput data[1], inout LineStream<PixelInput> SpriteStream)
{
	PixelInput output = (PixelInput)0;

	output.myCenterPosition = data[0].myPosition;
	output.myTeam = data[0].myTeam;

	const float sizeX = 0.09f;
	const float sizeTopY = 0.09;
	const float sizeY = 0.06f;
	const float tilt = 0.125f;

	//const float sizeX = 0.02f;
	//const float sizeTopY = 0.0f;
	//const float sizeY = 0.02f;
	//const float tilt = 0.0f;

	output.myPosition = float4(data[0].myPosition.x - sizeX - tilt, data[0].myPosition.y - sizeY - sizeTopY, data[0].myPosition.z, 1.0);
	output.myPixelPositionOnTerrain = output.myPosition;
	SpriteStream.Append(output);

	output.myPosition = float4(data[0].myPosition.x + sizeX + tilt, data[0].myPosition.y - sizeY - sizeTopY, data[0].myPosition.z, 1.0);
	output.myPixelPositionOnTerrain = output.myPosition;
	SpriteStream.Append(output);

	output.myPosition = float4(data[0].myPosition.x + sizeX, data[0].myPosition.y + sizeY, data[0].myPosition.z, 1.0);
	output.myPixelPositionOnTerrain = output.myPosition;
	SpriteStream.Append(output);

	output.myPosition = float4(data[0].myPosition.x - sizeX, data[0].myPosition.y + sizeY, data[0].myPosition.z, 1.0);
	output.myPixelPositionOnTerrain = output.myPosition;
	SpriteStream.Append(output);

	output.myPosition = float4(data[0].myPosition.x - sizeX - tilt, data[0].myPosition.y - sizeY - sizeTopY, data[0].myPosition.z, 1.0);
	output.myPixelPositionOnTerrain = output.myPosition;
	SpriteStream.Append(output);



	SpriteStream.RestartStrip();
}



float4 PixelShaderFunction(PixelInput anInput) : SV_Target
{
	return float4(1, 1, 1, 1);
}

technique11 Render
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VertexShaderFunction()));
		SetGeometryShader(CompileShader(gs_5_0, GS()));
		SetPixelShader(CompileShader(ps_5_0, PixelShaderFunction()));
		SetRasterizerState(Solid);
		SetDepthStencilState(EnableDepth, 0);
		SetBlendState(NoBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
	}
};
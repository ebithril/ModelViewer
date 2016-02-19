#define NUMBER_OF_POINT_LIGHTS 6
#define NUMBER_OF_SPOT_LIGHTS 6
#define NUMBER_OF_SPLAT_MAPS 1

Texture2D SplatMaps[NUMBER_OF_SPLAT_MAPS];
Texture2D SplatTextures[NUMBER_OF_SPLAT_MAPS * 4 + 1];
float2 SplatTextureSizes[NUMBER_OF_SPLAT_MAPS * 4 + 1];
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
	CullMode = FRONT;
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

DepthStencilState EnableDepth
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
	DepthFunc = LESS_EQUAL;
};

DepthStencilState DisableDepth
{
	DepthEnable = FALSE;
	DepthWriteMask = 0;
	DepthFunc = LESS_EQUAL;
};

BlendState NoBlend
{
	BlendEnable[0] = TRUE;
	SrcBlend = ONE;
	DestBlend = ONE;
	BlendOp = MAX;
	SrcBlendAlpha = ONE;
	DestBlendAlpha = ONE;
	BlendOpAlpha = ADD;
	RenderTargetWriteMask[0] = 0x0F;
};


struct VertexInput
{
	float3 myPosition : POSITION;
	float2 myUV : TEXCOORD;
};

struct GeometryInput
{
	float4 myPosition : POSITION;
	float mySize : TEXCOORD1;
};

struct PixelInput
{
	float4 myPosition : SV_POSITION;
	float4 myCenterPosition : POSITION;
	float4 myPixelPositionOnTerrain : POSITION2;
	float2 myTextureCoordinates : TEXCOORD0;
	float mySize : TEXCOORD1;
};

GeometryInput VertexShaderFunction(VertexInput anInput)
{
	GeometryInput output = (GeometryInput)0;

	//output.myPosition = mul(myToWorldMatrix, float4(anInput.myPosition,1));
	float3 pos2 = anInput.myPosition;
	float3 pos = (anInput.myPosition - TerrainPosition) / float3(TerrainSize, 1, TerrainSize);
	pos.y = pos.z;
	pos.z = 0.5f;
	pos *= 2.0f;
	pos -= float3(1,1,0);
	pos.y =  -pos.y;

	output.myPosition = float4(pos,1);
	output.mySize = (anInput.myUV.x * 1.2f * 2.0f) / TerrainSize;


	return output;
}
[maxvertexcount(45)]
void GS(point GeometryInput data[1], inout TriangleStream<PixelInput> SpriteStream)
{
	PixelInput output = (PixelInput)0;

	output.myCenterPosition = data[0].myPosition;

const float ittheltvarv = 3.14159265358979323846264f*2.0f;
const float ettsteg = 0.4487989505128276054946633404685;


	output.myPosition = float4(data[0].myPosition.x - data[0].mySize, data[0].myPosition.y + data[0].mySize, data[0].myPosition.z, 1.0);
	output.myPixelPositionOnTerrain = output.myPosition;
	output.mySize = data[0].mySize;
	SpriteStream.Append(output);
	
	output.myPosition = float4(data[0].myPosition.x - data[0].mySize, data[0].myPosition.y - data[0].mySize, data[0].myPosition.z, 1.0);
	output.myPixelPositionOnTerrain = output.myPosition;
	output.mySize = data[0].mySize;
	SpriteStream.Append(output);
	
	output.myPosition = float4(data[0].myPosition.x + data[0].mySize, data[0].myPosition.y + data[0].mySize, data[0].myPosition.z, 1.0);
	output.myPixelPositionOnTerrain = output.myPosition;
	output.mySize = data[0].mySize;
	SpriteStream.Append(output);
	
	
	output.myPosition = float4(data[0].myPosition.x + data[0].mySize, data[0].myPosition.y - data[0].mySize, data[0].myPosition.z, 1.0);
	output.myPixelPositionOnTerrain = output.myPosition;
	output.mySize = data[0].mySize;
	SpriteStream.Append(output);

	SpriteStream.RestartStrip();
}


float4 PixelShaderFunctionUnlit(PixelInput anInput) : SV_Target
{
	float gradStart = anInput.mySize - (15.0 / TerrainSize);
	float l = length(anInput.myPixelPositionOnTerrain - anInput.myCenterPosition) - gradStart;
	
	float gradient = saturate(l / (anInput.mySize - gradStart));


	return float4(1, 1, 1, 1) * (1 - gradient);
}



technique11 Render
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VertexShaderFunction()));
		SetGeometryShader(CompileShader(gs_5_0, GS()));
		SetPixelShader(CompileShader(ps_5_0, PixelShaderFunctionUnlit()));
		SetRasterizerState(Solid);
		SetDepthStencilState(DisableDepth, 0);
		SetBlendState(NoBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
	}
};




#define PI 3.14159265358979323846264f

Texture2D NotificationTexture;
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




cbuffer OnResizeBuffer
{
	matrix myProjectionMatrix;
}


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

BlendState BLEND_THIS_SHIT
{
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_COLOR;
	DestBlend = INV_SRC_COLOR;
	BlendOp = ADD;
	SrcBlendAlpha = ZERO;
	DestBlendAlpha = ZERO;
	BlendOpAlpha = ADD;
	RenderTargetWriteMask[0] = 0x0F;

	/*AlphaToCoverageEnable = FALSE;
	BlendEnable[0] = TRUE;*/
};

struct VertexInput
{
	float3 myPosition : POSITION;
	float2 myTextureCoordinates : TEXCOORD0;
};

struct GeometryInput
{
	float4 myPosition : POSITION;
	float2 myTextureCoordinates : TEXCOORD0;
	float myFade : TEXCOORD1;
};

struct PixelInput
{
	float4 myPosition : SV_POSITION;
	float2 myTextureCoordinates : TEXCOORD0;
	float myFade : TEXCOORD1;
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
	output.myFade = anInput.myTextureCoordinates.x;

	return output;
}

[maxvertexcount(45)]
void GS(point GeometryInput data[1], inout TriangleStream<PixelInput> SpriteStream)
{
	PixelInput output = (PixelInput)0;


	const float size = 0.085f;


	output.myFade = data[0].myFade;
	
	output.myPosition = float4(data[0].myPosition.x - size, data[0].myPosition.y + size, data[0].myPosition.z, 1.0);
	output.myTextureCoordinates = float2(0, 1);
	SpriteStream.Append(output);

	output.myPosition = float4(data[0].myPosition.x + size, data[0].myPosition.y + size, data[0].myPosition.z, 1.0);
	output.myTextureCoordinates = float2(1, 1);
	SpriteStream.Append(output);

	output.myPosition = float4(data[0].myPosition.x - size, data[0].myPosition.y - size, data[0].myPosition.z, 1.0);
	output.myTextureCoordinates = float2(0,0);
	SpriteStream.Append(output);

	output.myPosition = float4(data[0].myPosition.x + size, data[0].myPosition.y - size, data[0].myPosition.z, 1.0);
	output.myTextureCoordinates = float2(1, 0);
	SpriteStream.Append(output);



	SpriteStream.RestartStrip();
}



float4 PixelShaderFunction(PixelInput anInput) : SV_Target
{

	float4 texColor = NotificationTexture.Sample(DefaultSampler, anInput.myTextureCoordinates);
	

	return texColor * anInput.myFade * 0.8f;
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
		SetBlendState(BLEND_THIS_SHIT, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
	}
};


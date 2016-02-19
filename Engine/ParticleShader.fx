
matrix myToViewMatrix;
matrix myProjectionMatrix;

Texture2D DiffuseTexture;
Texture2D FogOfWarTexture;
Texture2D BlackMaskTexture;

float TerrainSize;

struct VertexInput
{
	float3 myPosition : POSITION;
	float myTotalTime : TEXCOORD0;
	float3 myVelocity : TEXCOORD1;
	float myRotation : TEXCOORD2;
	float2 myScale : PSIZE;
	float myAlpha : TEXCOORD3;
};

struct GeometryInput
{
	float3 myPosition : POSITION;
	float myTotalTime : TEXCOORD0;
	float3 myVelocity : TEXCOORD1;
	float myRotation : TEXCOORD2;
	float2 myScale : PSIZE;
	float myAlpha : TEXCOORD3;
	float2 myTerrainPosition : TEXCOORD4;
};

struct PixelInput
{
	float4 myPosition : SV_POSITION;
	float2 myTexturePosition : TEXCOORD0;
	float myAlpha : COLOR0;
	float2 myTerrainPosition : TEXCOORD1;
};

DepthStencilState EnableDepth
{
	DepthEnable = TRUE;
	DepthWriteMask = ZERO;
	DepthFunc = LESS_EQUAL;
};

BlendState BLEND_THIS_SHIT
{
	BlendEnable[0] = TRUE;
	SrcBlend = ONE;
	DestBlend = ONE;
	BlendOp = ADD;
	SrcBlendAlpha = ZERO;
	DestBlendAlpha = ZERO;
	BlendOpAlpha = ADD;
	RenderTargetWriteMask[0] = 0x0F;

	/*AlphaToCoverageEnable = FALSE;
	BlendEnable[0] = TRUE;*/
};

static const float2 GeometryTexcoords[4] =
{
	float2(0, 1),
	float2(1, 1),
	float2(0, 0),
	float2(1, 0)

};

SamplerState DefaultSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
	AddressW = WRAP;
};

GeometryInput VertexShaderFunction(VertexInput aVertexInput)
{
	GeometryInput output = (GeometryInput)0;

	output.myPosition = aVertexInput.myPosition;
	output.myScale = aVertexInput.myScale;
	output.myAlpha = aVertexInput.myAlpha;
	output.myTotalTime = aVertexInput.myTotalTime;
	output.myTerrainPosition = aVertexInput.myPosition.xz;
	

	return output;
}

[maxvertexcount(4)]
void GeometryShaderFunction(point GeometryInput input[1], inout TriangleStream<PixelInput> SpriteStream)
{
	PixelInput output;

	float4 viewPosition = mul(float4(input[0].myPosition, 1.0f), myToViewMatrix);

	float2 halfSize = input[0].myScale * 0.5f;

	float4 geometryPositions[4] =
	{
		float4(-halfSize.x, halfSize.y, 0, 0),
		float4(halfSize.x, halfSize.y, 0, 0),
		float4(-halfSize.x, -halfSize.y, 0, 0),
		float4(halfSize.x, -halfSize.y, 0, 0)
	};

	for (int i = 0; i < 4; i++)
	{
		output.myTerrainPosition = input[0].myPosition.xz;
		output.myPosition = mul(viewPosition + geometryPositions[i], myProjectionMatrix);
		output.myTexturePosition = GeometryTexcoords[i];
		output.myAlpha = saturate(input[0].myAlpha);
		SpriteStream.Append(output);
	}

	SpriteStream.RestartStrip();
}

float4 PixelShaderFunction(PixelInput aPixelInput) : SV_Target
{
	float2 vertexPositionOnTerrain = (aPixelInput.myTerrainPosition) / max(TerrainSize, 1);
	float fogOfWarValue = FogOfWarTexture.Sample(DefaultSampler, vertexPositionOnTerrain);
	float blackMaskValue = BlackMaskTexture.Sample(DefaultSampler, vertexPositionOnTerrain);


	if (blackMaskValue == 0 && TerrainSize > 0)
	{
		discard;
	}

	float4 color = DiffuseTexture.Sample(DefaultSampler, aPixelInput.myTexturePosition);
	color.a *= aPixelInput.myAlpha;
	color.xyz = color.xyz * (color.a * (fogOfWarValue + 0.35f));
	return saturate(color);
}



technique11 Render
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VertexShaderFunction()));
		SetGeometryShader(CompileShader(gs_5_0, GeometryShaderFunction()));
		SetPixelShader(CompileShader(ps_5_0, PixelShaderFunction()));

		SetDepthStencilState(EnableDepth, 0);
		SetBlendState(BLEND_THIS_SHIT, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
	}
}

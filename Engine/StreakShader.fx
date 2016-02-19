matrix myToViewMatrix;
matrix myProjectionMatrix;

float LastPointTime;
float LifeTime;

Texture2D DiffuseTexture;

struct VertexInput
{
	float3 myPosition : POSITION;
	float myTime : TEXCOORD0;
	float mySize : TEXCOORD1;
	float myAlpha : TEXCOORD2;
};

struct GeometryInput
{
	float3 myPosition : POSITION;
	float myTime : TEXCOORD0;
	float mySize : TEXCOORD1;
	float myAlpha : TEXTCOORD2;
};

struct PixelInput
{
	float4 myPosition : SV_POSITION;
	float2 myTexturePosition : TEXCOORD0;
	float myAlpha : COLOR0;
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
	SrcBlend = SRC_COLOR;
	DestBlend = INV_SRC_COLOR;
	BlendOp = ADD;
	SrcBlendAlpha = ZERO;
	DestBlendAlpha = ZERO;
	BlendOpAlpha = ADD;
	RenderTargetWriteMask[0] = 0x0F;
};

static const float2 GeometryTexcoords[4] =
{
	float2(0, 0),
	float2(1, 0),
	float2(1, 1),
	float2(0, 1)
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

	output.myPosition = mul(float4(aVertexInput.myPosition, 1), myToViewMatrix).xyz;
	output.mySize = aVertexInput.mySize;
	output.myAlpha = aVertexInput.myAlpha;
	output.myTime = aVertexInput.myTime;

	return output;
}

[maxvertexcount(4)]
void GeometryShaderFunction(lineadj GeometryInput input[4], inout TriangleStream<PixelInput> SpriteStream)
{
	float3 firstVector = input[0].myPosition - input[2].myPosition;
	float2 firstNormal = normalize(float2(-firstVector.y, firstVector.x));
	float3 secondvector = input[1].myPosition - input[3].myPosition;
	float2 secondNormal = normalize(float2(-secondvector.y, secondvector.x));

	float2 firstSize = max(input[0].mySize, 0) * 0.5f;
	float2 secondSize = max(input[1].mySize, 0) * 0.5f;

	float4 geometryPositions[4] =
	{
		float4((firstSize * firstNormal + input[0].myPosition.xy), input[0].myPosition.z, 1),
		float4((secondSize * secondNormal + input[1].myPosition.xy), input[1].myPosition.z, 1),
		float4((secondSize * -secondNormal + input[1].myPosition.xy), input[1].myPosition.z, 1),
		float4((firstSize * -firstNormal + input[0].myPosition.xy), input[0].myPosition.z, 1)
	};

	float UScroll1 = 1 - ((LastPointTime - input[0].myTime) / LifeTime);
	float UScroll2 = 1 - ((LastPointTime - input[1].myTime) / LifeTime);

	PixelInput vertex1;
	vertex1.myPosition = mul(geometryPositions[0], myProjectionMatrix);
	vertex1.myTexturePosition = GeometryTexcoords[0];
	vertex1.myTexturePosition.x = UScroll1;
	vertex1.myAlpha = saturate(input[0].myAlpha);
	SpriteStream.Append(vertex1);

	PixelInput vertex2;
	vertex2.myPosition = mul(geometryPositions[1], myProjectionMatrix);
	vertex2.myTexturePosition = GeometryTexcoords[1];
	vertex2.myTexturePosition.x = UScroll1;
	vertex2.myAlpha = saturate(input[1].myAlpha);
	SpriteStream.Append(vertex2);

	PixelInput vertex4;
	vertex4.myPosition = mul(geometryPositions[3], myProjectionMatrix);
	vertex4.myTexturePosition = GeometryTexcoords[3];
	vertex4.myTexturePosition.x = UScroll1;
	vertex4.myAlpha = saturate(input[0].myAlpha);
	SpriteStream.Append(vertex4);

	PixelInput vertex3;
	vertex3.myPosition = mul(geometryPositions[2], myProjectionMatrix);
	vertex3.myTexturePosition = GeometryTexcoords[2];
	vertex3.myTexturePosition.x = UScroll1;
	vertex3.myAlpha = saturate(input[1].myAlpha);
	SpriteStream.Append(vertex3);



	SpriteStream.RestartStrip();
}

float4 PixelShaderFunction(PixelInput aPixelInput) : SV_Target
{
	float4 color = DiffuseTexture.Sample(DefaultSampler, aPixelInput.myTexturePosition);
	color.a = color.a * aPixelInput.myAlpha;
	color.xyz = color.xyz * color.a;
	return color;
}

RasterizerState NoCull
{
	CullMode = NONE;
};

technique11 Render
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VertexShaderFunction()));
		SetGeometryShader(CompileShader(gs_5_0, GeometryShaderFunction()));
		SetPixelShader(CompileShader(ps_5_0, PixelShaderFunction()));
		SetRasterizerState(NoCull);
		SetDepthStencilState(EnableDepth, 0);
		SetBlendState(BLEND_THIS_SHIT, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
	}
}

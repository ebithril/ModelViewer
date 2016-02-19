
matrix myToViewMatrix;
matrix myProjectionMatrix;
matrix myDecalOrientation;
Texture2D DiffuseTexture;
matrix myWorldViewProjection;
float4 myColor;

struct VertexInput
{
	float4 myPosition : POSITION;
	float4 myNormal : NORMAL;
	float2 myUVPos : TEXCOORD0;
};

struct PixelInput
{
	float4 myViewSpacePosition : SV_POSITION;
	float2 myTexturePosition : TEXCOORD0;
	float4 myWorldPosition : TEXCOORD1;
	float3 myNormal : NORMAL;

};



DepthStencilState EnableDepth
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

BlendState BLEND_THIS_SHIT
{
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
	DestBlend = ONE;
	BlendOp = ADD;
	SrcBlendAlpha = ZERO;
	DestBlendAlpha = ZERO;
	BlendOpAlpha = ADD;
	RenderTargetWriteMask[0] = 0x0F;

	/*AlphaToCoverageEnable = FALSE;
	BlendEnable[0] = TRUE;*/
};


SamplerState DefaultSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
	AddressW = WRAP;
};

PixelInput VertexShaderFunction(VertexInput aVertexInput)
{
	PixelInput output;

	//output.myViewSpacePosition = mul(myProjectionMatrix, mul(myToViewMatrix, float4(aVertexInput.myPosition + myDecalPosition, 1.0f)));
	output.myViewSpacePosition = mul(aVertexInput.myPosition, myWorldViewProjection);
	output.myWorldPosition = mul(aVertexInput.myPosition, myDecalOrientation).xyzw;
	output.myTexturePosition = aVertexInput.myUVPos;
	output.myNormal = aVertexInput.myNormal.xyz;
	output.myNormal = normalize(output.myNormal);

	return output;
}



float4 PixelShaderFunction(PixelInput aPixelInput) : SV_Target
{
	float2 uv = aPixelInput.myTexturePosition;
	float4 color = DiffuseTexture.Sample(DefaultSampler, uv);
	//color.xyz *= color.a;
	return color * myColor;
}



technique11 Render
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VertexShaderFunction()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PixelShaderFunction()));
		SetDepthStencilState(EnableDepth, 0);
		SetBlendState(BLEND_THIS_SHIT, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
	}
}

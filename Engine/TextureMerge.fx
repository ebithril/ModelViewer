Texture2D DiffuseTexture;
Texture2D SecondaryDiffuseTexture;

RasterizerState NoCull
{
	CullMode = NONE;
	FillMode = SOLID;
};

DepthStencilState DisableDepth
{
	DepthEnable = FALSE;
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
	SrcBlend = ONE;
	DestBlend = ONE;
	BlendOp = ADD;
	SrcBlendAlpha = ONE;
	DestBlendAlpha = ONE;
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

struct PSInput
{
	float4 myPosition : SV_POSITION;
	float2 myTexturePosition : TEXCOORD0;
};

struct VSInput
{
	float3 myPosition : POSITION;
	float2 myTexturePosition : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PSInput VS(VSInput input)
{
	PSInput output;
	output.myPosition = float4(input.myPosition.xyz, 1);
	output.myTexturePosition = input.myTexturePosition;
	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS_Secondary_Override(PSInput anInput) : SV_Target
{
	float4 diffTex = DiffuseTexture.Sample(DefaultSampler, anInput.myTexturePosition);
	float4 secondaryDiffTex = SecondaryDiffuseTexture.Sample(DefaultSampler, anInput.myTexturePosition);

	float4 color = float4((diffTex.xyz * diffTex.a + secondaryDiffTex.xyz * secondaryDiffTex.a), 1);

	if (secondaryDiffTex.a == 0)
	{
		return diffTex;
	}
	return secondaryDiffTex;
}

float4 PS_Blend_First_With_Target(PSInput anInput) : SV_Target
{
	float4 diffuse = DiffuseTexture.Sample(DefaultSampler, anInput.myTexturePosition);

	diffuse.w = 16.f;
	//diffuse.xyz = diffuse.xyz + diffuse.xyz * 9.9f;

	return diffuse;
}
//--------------------------------------------------------------------------------------

technique11 RenderSecondaryOverride
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_Secondary_Override()));
		SetDepthStencilState(DisableDepth, 0);
		SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetRasterizerState(NoCull);
	}
}

technique11 BlendFirstWithTarget
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_Blend_First_With_Target()));
		SetDepthStencilState(DisableDepth, 0);
		SetBlendState(BLEND_THIS_SHIT, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetRasterizerState(NoCull);
	}
}
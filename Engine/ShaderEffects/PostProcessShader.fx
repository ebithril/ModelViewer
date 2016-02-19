Texture2D DiffuseTexture;
Texture2D SecondaryDiffuseTexture;
Texture2D TertiaryDiffuseTexture;
Texture2D QuartaryDiffuseTexture;

float3 FadeColor;
float2 InvTargetSize;
float2 InvSourceSize;
float DeltaTime;
float Exposure;
float FadeAmount;

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

BlendState FadeBlend
{
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
	DestBlend = INV_SRC_ALPHA;
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

SamplerState MotionBlurSample
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
	AddressW = CLAMP;
};

SamplerState SamplePoint
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = WRAP;
	AddressV = WRAP;
	AddressW = WRAP;
};


struct PixelInput
{
	float4 myPosition : SV_POSITION;
	float2 myTexturePosition : TEXCOORD0;
};

struct VertexInput
{
	float3 myPosition : POSITION;
	float2 myTexturePosition : TEXCOORD0;

};

Texture2D TextTexture;

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PixelInput VS(VertexInput aVertexInput)
{
	PixelInput vertexOutput;

	vertexOutput.myPosition = float4(aVertexInput.myPosition.xyz, 1.0f);
	vertexOutput.myTexturePosition = aVertexInput.myTexturePosition;

	return vertexOutput;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PixelInput anInput) : SV_Target
{
	return DiffuseTexture.SampleLevel(DefaultSampler, anInput.myTexturePosition, 0);
}

float4 PSFade(PixelInput anInput) : SV_Target
{
	float fade = 1 - FadeAmount;

	float4 colorToReturn = DiffuseTexture.SampleLevel(DefaultSampler, anInput.myTexturePosition, 0) * fade;
	colorToReturn += float4(FadeColor.xyz, 1)*FadeAmount;
	return colorToReturn;
}

float4 PSBloom(PixelInput anInput) : SV_Target
{
	float4 firstSample = DiffuseTexture.SampleLevel(DefaultSampler, anInput.myTexturePosition, 0);
	float4 secondSample = SecondaryDiffuseTexture.SampleLevel(DefaultSampler, anInput.myTexturePosition, 0);
	float4 thirdSample = TertiaryDiffuseTexture.SampleLevel(DefaultSampler, anInput.myTexturePosition, 0);
	float4 fourthSample = QuartaryDiffuseTexture.SampleLevel(DefaultSampler, anInput.myTexturePosition, 0);

	return firstSample + secondSample + thirdSample + fourthSample;
}

float LinearToLum(float4 aColor)
{
	return (aColor.r*0.3 + aColor.g * 0.59 + aColor.b * 0.11);
}


float4 PSPoint(PixelInput anInput) : SV_Target
{
	return DiffuseTexture.SampleLevel(SamplePoint, anInput.myTexturePosition, 0);
}

float PSLum(PixelInput anInput) : SV_Target
{
	return LinearToLum(DiffuseTexture.SampleLevel(SamplePoint, anInput.myTexturePosition, 0));
}

float4 PSAdaptLuminance(PixelInput anInput) : SV_Target
{
	float oldAdaptedLum = SecondaryDiffuseTexture.SampleLevel(SamplePoint, anInput.myTexturePosition, 0).r;
	float currentLum = DiffuseTexture.SampleLevel(SamplePoint, anInput.myTexturePosition, 0).r;

	// The user's adapted luminance level is simulated by closing the gap between
	// adapted luminance and current luminance by 2% every frame, based on a
	// 30 fps rate. This is not an accurate model of human adaptation, which can
	// take longer than half an hour.
	float newLum = oldAdaptedLum + (currentLum - oldAdaptedLum) * (1 - exp(-0.4 * DeltaTime));
	return float4(newLum, newLum, newLum, 1.0f);
}

//--Tonemap Functions-------------------------------------------------------------------
float A = 0.22;
float B = 0.30;
float C = 0.10;
float D = 0.20;
float E = 0.01;
float F = 0.30;

float WhitePoint = 11.2;


float3 Uncharted2Tonemap(float3 x)
{
	return ((x*(A*x + C*B) + D*E) / (x*(A*x + B) + D*F)) - E / F;
}

float lumMin = 0.5f;
float lumMax = 10.0;

float4 PSToneMapUncharted2(PixelInput input) : SV_Target
{
	float4 color;
	color = DiffuseTexture.SampleLevel(SamplePoint, input.myTexturePosition, 0);

	float luminance = SecondaryDiffuseTexture.SampleLevel(DefaultSampler, input.myTexturePosition, 0).r;
	if (luminance > 0 || luminance < 0)
	{
		luminance = 0.1;
	}
	if (luminance != luminance || luminance == 0)
	{
		luminance = 0.1;
	}

	luminance = min(max(luminance, lumMin), lumMax);
	//return float4(luminance, luminance, luminance, 1);

	//float exposure = 0.18f;//(1.03f-(2/(2+log10((luminance.r)+1))))*AutoExposureAdjustment;

		float4 texColor = float4(color.xyz*(Exposure) / (luminance),1);
		float3 curr = 2 * Uncharted2Tonemap(texColor.xyz);

		float3 whiteScale = 1.0f / Uncharted2Tonemap(WhitePoint);
		float3 color2 = curr*whiteScale;

		return float4(color2, 1);
}

//--------------------------------------------------------------------------------------

float4 PSMotionBlur(PixelInput anInput) : SV_Target
{
	float4 color = DiffuseTexture.SampleLevel(DefaultSampler, anInput.myTexturePosition, 0);

	float2 texturePos = anInput.myTexturePosition;

	float2 velocity = SecondaryDiffuseTexture.SampleLevel(SamplePoint, anInput.myTexturePosition, 0).rg;
	//return float4(velocity.rg, 0, 0);
	velocity.y *= -1.f;

	velocity *= 0.35f;

	texturePos += velocity;

	int numberOfSamples = 4;

	for (int i = 0; i < numberOfSamples; ++i, texturePos += velocity)
	{
		//texturePos = clamp(texturePos, texturePosMin, texturePosMax);
		// Sample the color buffer along the velocity vector.
		float4 currentColor = DiffuseTexture.SampleLevel(MotionBlurSample, texturePos, 0);
			// Add the current color to our color sum.
			color += currentColor;
	}
	// Average all of the samples to get the final blur color.
	float4 finalColor = color / numberOfSamples;

	return finalColor;
}


technique11 Bloom
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PSBloom()));
		SetDepthStencilState(DisableDepth, 0);
		SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
	}
}

technique11 Render
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
		SetDepthStencilState(DisableDepth, 0);
		SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
	}
}

technique11 RenderPoint
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PSPoint()));
		SetDepthStencilState(DisableDepth, 0);
		SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
	}
}

technique11 RenderLum
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PSLum()));
		SetDepthStencilState(DisableDepth, 0);
		SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
	}
};

technique11 ToneMap
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PSToneMapUncharted2()));
		SetDepthStencilState(DisableDepth, 0);
		SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
	}
}

technique11 AdaptLuminance
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PSAdaptLuminance()));
		SetDepthStencilState(DisableDepth, 0);
		SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
	}
}

technique11 MotionBlur
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PSMotionBlur()));
		SetDepthStencilState(DisableDepth, 0);
		SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
	}
}
technique11 Fade
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PSFade()));
		SetDepthStencilState(DisableDepth, 0);
		SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		//SetBlendState(FadeBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
	}
}

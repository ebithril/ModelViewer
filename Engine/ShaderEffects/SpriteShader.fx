#include "ShaderFunctions.fx"


#define NUMBER_OF_SPLAT_MAPS 1

float4 Color;



struct SpriteVertexInput
{
	float2 myPosition : POSITION;
	float2 myTextureCoordinates : TEXCOORD;
	float4 myColor : COLOR;
};

struct SpritePixelInput
{
	float4 myPosition : SV_POSITION;
	float2 myTextureCoordinates : TEXCOORD;
};

SpritePixelInput VertexShaderFunction(SpriteVertexInput anInput)
{
	SpritePixelInput output = (SpritePixelInput)0;

	float4 finalPosition = mul(myToWorldMatrix, float4(anInput.myPosition, 0.5, 1));

	// From NDC projection-space to texel-space.
	finalPosition = float4(finalPosition.xy, 0.5, 1);
	finalPosition.y = 1 - finalPosition.y;
	finalPosition.xy *= 2.f;
	finalPosition.xy -= float2(1, 1);

	output.myPosition = finalPosition;
	output.myTextureCoordinates = anInput.myTextureCoordinates;

	return output;
}

float4 PixelShaderFunctionUnlit(SpritePixelInput anInput) : SV_Target
{
	return DiffuseTexture.Sample(DefaultSampler, anInput.myTextureCoordinates) * Color;
}

technique11 Render
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VertexShaderFunction()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PixelShaderFunctionUnlit()));
		SetRasterizerState(Solid);
		SetDepthStencilState(EnableDepth, 0);
		SetBlendState(AlphaBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
	}
};
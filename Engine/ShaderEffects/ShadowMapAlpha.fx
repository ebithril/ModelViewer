#include "ShaderFunctions.fx"

struct ShadowPixelInput
{
	float4 myPosition : SV_POSITION;
	float2 myTexturePosition : TEXCOORD;
};

matrix myToWorldShadowMatrix;
matrix myLightViewProjectionMatrix;

ShadowPixelInput VertexShaderFunction(VertexInput aVertexInput)
{
	PixelInput animationData;
	animationData = GetAnimationData(animationData, aVertexInput);
	ShadowPixelInput output;

	float4 vertexWorldPosition = mul(float4(animationData.myWorldPosition.xyz, 1), myToWorldShadowMatrix);
		output.myPosition = mul(vertexWorldPosition, myLightViewProjectionMatrix);

	output.myTexturePosition = aVertexInput.myTexturePosition;

	return output;
}

void PixelShaderFunction(ShadowPixelInput aPixelInput)
{
	float4 color = DiffuseTexture.Sample(DefaultSampler, aPixelInput.myTexturePosition);

	clip(color.a < 0.1f ? -1 : 1);
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
		SetRasterizerState(NoCull);
	}
}

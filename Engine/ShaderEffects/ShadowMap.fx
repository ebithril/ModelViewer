#include "ShaderFunctions.fx"

matrix myToWorldShadowMatrix;
matrix myLightViewProjectionMatrix;

struct ShadowPixelInput
{
	float4 myPosition : SV_POSITION;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
ShadowPixelInput VertexShaderFunction(VertexInput aVertexInput)
{
	PixelInput animationData;
	animationData = GetAnimationData(animationData, aVertexInput);
	ShadowPixelInput output;

	float4 vertexWorldPosition = mul(float4(animationData.myWorldPosition.xyz, 1), myToWorldShadowMatrix);
		output.myPosition = mul(vertexWorldPosition, myLightViewProjectionMatrix);
	return output;
}

//--------------------------------------------------------------------------------------
// Technique
//--------------------------------------------------------------------------------------
technique11 Render
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VertexShaderFunction()));
		SetGeometryShader(NULL);
		SetPixelShader(NULL);

		SetRasterizerState(Solid);
		SetDepthStencilState(EnableDepth, 0);
		SetBlendState(NoBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
	}
}

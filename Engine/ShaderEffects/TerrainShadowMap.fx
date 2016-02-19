#include "ShaderFunctions.fx"

matrix myToWorldShadowMatrix;
matrix myLightViewProjectionMatrix;

struct ShadowPixelInput
{
	float4 myPosition : SV_POSITION;
};

struct TerrainVertexInput
{
	float3 myPosition : POSITION;
	float3 myNormal : NORMAL;
	float2 myTextureCoordinates : TEXCOORD;
	float3 myBiNormal : BINORMAL;
	float3 myTangent : TANGENT;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
ShadowPixelInput VertexShaderFunction(TerrainVertexInput aVertexInput)
{
	ShadowPixelInput output;

	float4 vertexWorldPosition = mul(float4(aVertexInput.myPosition.xyz, 1), myToWorldShadowMatrix);
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
#include "Default.fx"

RasterizerState NoCulling
{
	CullMode = NONE;
};



PixelInput VertexShaderFunction(VertexInput aVertexInput)
{
	PixelInput vertexOutput;
	float4 worldPosition;

	myToViewMatrix;
	myToWorldMatrix;
	myProjectionMatrix;

	vertexOutput.myPosition = mul(myWorldViewProjection, float4(aVertexInput.myPosition, 1.0f));
	vertexOutput.myPosition.z = 0.99;

	vertexOutput.myTexturePosition = aVertexInput.myTexturePosition;

	vertexOutput.myNormal = mul((float3x3)myToWorldMatrix, aVertexInput.myNormal);
	vertexOutput.myNormal = normalize(vertexOutput.myNormal);

	vertexOutput.myWorldPosition = mul(float4(aVertexInput.myPosition, 1.0f), myToWorldMatrix);
	vertexOutput.myViewDirection = myCameraPosition.xyz - vertexOutput.myWorldPosition.xyz;
	vertexOutput.myViewDirection = normalize(vertexOutput.myViewDirection);

	vertexOutput.myTangent = mul((float3x3)myToWorldMatrix, aVertexInput.myTangent);

		return vertexOutput;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PixelShaderFunction(PixelInput aPixelInput) : SV_Target
{
	return DiffuseTexture.SampleLevel(ClampSampler, aPixelInput.myTexturePosition, 0);

	//return float4(0.3, 0.5, 1, 1);
}

technique11 Render
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VertexShaderFunction()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PixelShaderFunction()));

		SetDepthStencilState(DisableDepthWrite, 0);
		SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
	}
}
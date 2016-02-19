#include "Default.fx"

Texture2D TextTexture;

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PixelInput VertexShaderFunction(VertexInput aVertexInput)
{
	PixelInput vertexOutput;

	vertexOutput.myPosition = mul(myWorldViewProjection, float4(aVertexInput.myPosition, 1.0f));
	vertexOutput.myTexturePosition = aVertexInput.myTexturePosition;

	//if (vertexOutput.myPosition.z > 1)
	//{
	//	vertexOutput.myPosition.z = 0.99f;
	//}

	return vertexOutput;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
ReturnData PixelShaderFunction(PixelInput aPixelInput) : SV_Target
{
	ReturnData returnData;
	float4 diffuse = DiffuseTexture.Sample(DefaultSampler, aPixelInput.myTexturePosition);

	returnData.myColor = diffuse;
	returnData.myVelocity = float2(0, 0);
	returnData.myColor.xyz *= saturate(returnData.myColor.a);
	returnData.myColor.a = 1;
	return returnData;
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
		SetPixelShader(CompileShader(ps_5_0, PixelShaderFunction()));

		SetDepthStencilState(DisableDepth, 0);
		SetBlendState(BLEND_THIS_SHIT, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
	}
}

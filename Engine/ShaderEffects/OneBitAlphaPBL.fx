#include "ShaderFunctions.fx"

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PixelInput VertexShaderFunction(VertexInput aVertexInput)
{
	return GetPixelInputWithAnimationData(aVertexInput);
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
ReturnData PixelShaderFunction(PixelInput aPixelInput) : SV_Target
{
	PixelData data = GetPixelData(aPixelInput);

	if (data.myAlbedo.w <= 0)
	{
		discard;
	}

	ReturnData returnData ;
	returnData.myColor = 0;
	returnData.myVelocity = CalculateVelocity(aPixelInput);

	float4 totalLight = GetPBLTotalLight(aPixelInput);

		//returnData.myColor = totalLight * FogOfWarValue(aPixelInput);

		returnData.myColor.xyz = DistanceFog(totalLight, aPixelInput.myWorldPosition, float3(0.13, 0.08, 0.76), myCameraPosition);
	returnData.myColor *= FogOfWarValue(aPixelInput);
	return returnData;
}

ReturnData PixelShaderFunctionWithFow(PixelInput aPixelInput) : SV_Target
{
	float insideFow = FogOfWarCheck(aPixelInput);

	PixelData data = GetPixelData(aPixelInput);

	if (data.myAlbedo.w <= 0)
	{
		discard;
	}

	ReturnData returnData;
	returnData.myVelocity = CalculateVelocity(aPixelInput);

	float4 totalLight = GetPBLTotalLight(aPixelInput);

	returnData.myColor = totalLight;
	returnData.myColor *= insideFow;

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

		SetDepthStencilState(EnableDepth, 0);
		SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetRasterizerState(BackFaceCulling);
	}
}

technique11 RenderWithFow
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VertexShaderFunction()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PixelShaderFunction()));

		SetDepthStencilState(EnableDepth, 0);
		SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetRasterizerState(BackFaceCulling);
	}
}

technique11 RenderAlbedo
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VertexShaderFunction()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PixelShaderFunction_Albedo()));

		SetDepthStencilState(EnableDepth, 0);
		SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
	}
}

technique11 RenderMappedNormals
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VertexShaderFunction()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PixelShaderFunction_MappedNormals()));

		SetDepthStencilState(EnableDepth, 0);
		SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
	}
}

technique11 RenderMeshNormals
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VertexShaderFunction()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PixelShaderFunction_MeshNormals()));

		SetDepthStencilState(EnableDepth, 0);
		SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
	}
}

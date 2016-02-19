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
	ReturnData returnData;
	returnData.myColor = 0;
	returnData.myVelocity = CalculateVelocity(aPixelInput);
	//returnData.myColor = float4(0.2, 0.2, 0.2, 1) * AmbientCubeMap.Sample(DefaultSampler, aPixelInput).xyz;
	returnData.myColor = float4(0.2, 0.2, 0.2, 1) * DiffuseTexture.Sample(DefaultSampler, aPixelInput.myTexturePosition) + AmbientCubeMap.Sample(DefaultSampler, aPixelInput.myNormal);
	return returnData;

	float4 totalLight = GetPBLTotalLight(aPixelInput);

	//returnData.myColor = totalLight * FogOfWarValue(aPixelInput);

	returnData.myColor.xyz = DistanceFog(totalLight, aPixelInput.myWorldPosition, float3(0, 0, 1), myCameraPosition);
	returnData.myColor *= FogOfWarValue(aPixelInput);
	return returnData;
}

ReturnData PixelShaderFunctionWithFow(PixelInput aPixelInput) : SV_Target
{


	float insideFog = FogOfWarCheck(aPixelInput);
	if (insideFog == 0)
	{
		discard;
	}

	ReturnData returnData;
	returnData.myColor = 0;
	returnData.myVelocity = CalculateVelocity(aPixelInput);

	returnData.myColor = float4(1, 1, 0, 1);
	return returnData;


	float4 totalLight = GetPBLTotalLight(aPixelInput);


	returnData.myColor.xyz = DistanceFog(totalLight.xyz, aPixelInput.myWorldPosition, float3(0.13, 0.08, 0.76), myCameraPosition);
	returnData.myColor *= FogOfWarValue(aPixelInput);
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
		SetPixelShader(CompileShader(ps_5_0, PixelShaderFunctionWithFow()));

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

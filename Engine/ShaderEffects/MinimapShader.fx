#include "ShaderFunctions.fx"

Texture2D FOWTexture;
Texture2D UnitsMarkerTexture;
Texture2D CameraTexture;
Texture2D NotificationTexture;
//Texture2D BlackMaskTexture;

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
	returnData.myVelocity = CalculateVelocity(aPixelInput);

	//PixelData data = GetPixelData(aPixelInput);

	float2 fowTexCoord = aPixelInput.myTexturePosition;
	fowTexCoord.y = 1 - fowTexCoord.y;

	float fowValue = FOWTexture.Sample(DefaultSampler, fowTexCoord).r;
	float blackMaskValue = BlackMaskTexture.Sample(DefaultSampler, fowTexCoord).r;
	float4 unitsMarker = UnitsMarkerTexture.Sample(DefaultSampler, fowTexCoord);
	float camera = CameraTexture.Sample(DefaultSampler, fowTexCoord).r;
	float4 notificationMarker = NotificationTexture.Sample(DefaultSampler, fowTexCoord);

		returnData.myColor = float4(DiffuseTexture.Sample(DefaultSampler, aPixelInput.myTexturePosition).xyz, 1);
	returnData.myColor *= fowValue + blackMaskValue;
	//returnData.myColor *= blackMaskValue;

	returnData.myColor *= 1 - ((unitsMarker.r + unitsMarker.g + unitsMarker.b) * fowValue);
	returnData.myColor = saturate(returnData.myColor + (unitsMarker * fowValue));

	returnData.myColor *= 1 - ((notificationMarker.r + notificationMarker.g + notificationMarker.b));
	returnData.myColor = saturate(returnData.myColor + (notificationMarker));

	returnData.myColor += camera;
	returnData.myColor += notificationMarker;
	

	//float4 emissiveLighting = EmissiveTexture.Sample(DefaultSampler, aPixelInput.myTexturePosition);
	//returnData.myColor += emissiveLighting;

	float animTheta = -(myTotalTime * 0.25) + aPixelInput.myTexturePosition.y;
	float animFunction = (sin(animTheta * 8) * 16) - 15;
	animFunction *= 0.08;
	animFunction = saturate(animFunction);
	returnData.myColor += float4(animFunction * 0.75, animFunction, animFunction, 0);

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
		SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetRasterizerState(BackFaceCulling);
	}
}
#include "Default.fx"
#include "PBLLighting.fx"

cbuffer PerObjectLightBuffer
{
	LightData myLightData[NUMBER_OF_POINT_AND_SPOT_LIGHTS];
};


PixelInput GetAnimationData(PixelInput aVertexOutput, VertexInput aVertexInput)
{
	float4 pos = float4(aVertexInput.myPosition, 1);
		float3 norm = aVertexInput.myNormal;

		aVertexOutput.myWorldPosition = float4(0, 0, 0, 0);
	aVertexOutput.myNormal = float3(0, 0, 0);

	uint iBone = aVertexInput.myBoneIndecies.x;
	float fWeight = aVertexInput.myBoneWeights.x;

	aVertexOutput.myWorldPosition += fWeight * mul(pos, myBones[iBone]);
	aVertexOutput.myNormal += fWeight * (mul(float4(norm, 1), myBones[iBone])).xyz;

	//bone1
	iBone = aVertexInput.myBoneIndecies.y;
	fWeight = aVertexInput.myBoneWeights.y;

	aVertexOutput.myWorldPosition += fWeight * mul(pos, myBones[iBone]);
	aVertexOutput.myNormal += fWeight * (mul(float4(norm, 1), myBones[iBone])).xyz;
	//
	//bone2
	iBone = aVertexInput.myBoneIndecies.z;
	fWeight = aVertexInput.myBoneWeights.z;

	aVertexOutput.myWorldPosition += fWeight * mul(pos, myBones[iBone]);
	aVertexOutput.myNormal += fWeight * (mul(float4(norm, 1), myBones[iBone])).xyz;


	//bone3
	iBone = aVertexInput.myBoneIndecies.w;
	fWeight = aVertexInput.myBoneWeights.w;

	aVertexOutput.myWorldPosition += fWeight * mul(pos, myBones[iBone]);
	aVertexOutput.myNormal += fWeight * (mul(float4(norm, 1), myBones[iBone])).xyz;

	return aVertexOutput;
}

PixelInput GetPixelInputWithAnimationData(VertexInput aVertexInput)
{
	PixelInput vertexOutput = (PixelInput)0;

	vertexOutput = GetAnimationData(vertexOutput, aVertexInput);

	vertexOutput.myPosition = mul(myWorldViewProjection, vertexOutput.myWorldPosition);
	vertexOutput.myWorldPosition.w = 1;
	vertexOutput.myWorldPosition = mul(myToWorldMatrix, vertexOutput.myWorldPosition);

	vertexOutput.myTexturePosition = aVertexInput.myTexturePosition;

	vertexOutput.myNormal = mul((float3x3) myToWorldMatrix, vertexOutput.myNormal);
	vertexOutput.myNormal = normalize(vertexOutput.myNormal);

	vertexOutput.myViewDirection = myCameraPosition.xyz - vertexOutput.myWorldPosition.xyz;
	vertexOutput.myViewDirection = normalize(vertexOutput.myViewDirection);

	vertexOutput.myBiNormal = mul((float3x3) myToWorldMatrix, aVertexInput.myBiNormal);

	vertexOutput.myTangent = mul((float3x3) myToWorldMatrix, aVertexInput.myTangent);

	vertexOutput.myDirectionalLightShadowCoordinates = mul(myDirectionalLightViewProjection, vertexOutput.myWorldPosition);
	vertexOutput.myDirectionalLightShadowCoordinates.xyz = vertexOutput.myDirectionalLightShadowCoordinates.xyz / vertexOutput.myDirectionalLightShadowCoordinates.w;
	vertexOutput.myDirectionalLightShadowCoordinates.y = vertexOutput.myDirectionalLightShadowCoordinates.y * -1.f;
	vertexOutput.myDirectionalLightShadowCoordinates.xy = vertexOutput.myDirectionalLightShadowCoordinates.xy + 1;
	vertexOutput.myDirectionalLightShadowCoordinates.xy = vertexOutput.myDirectionalLightShadowCoordinates.xy * 0.5f;
	vertexOutput.myDirectionalLightShadowCoordinates.z -= 0.00001;

	return vertexOutput;
}

float3 DistanceFog(float3 aPixelColor, float3 aPixelPosition, float3 aFogColor, float3 aCameraPosition)
{
	float distance = length(aCameraPosition - aPixelPosition);

	float FogStart = 20;
	float FogEnd = 100;

	float3 viewDirection = normalize(aPixelPosition - aCameraPosition);
		float fogFactor = saturate((FogStart - distance) / (FogStart - FogEnd));
	float3 resultFogColor = lerp(aPixelColor, aFogColor, fogFactor);

	return resultFogColor;
}

float FogOfWarCheck(PixelInput aPixelInput)
{
	float2 vertexPositionOnTerrain = (aPixelInput.myWorldPosition.xz) / TerrainSize;

		return FogOfWarTexture.Sample(DefaultSampler, vertexPositionOnTerrain).r;

}

float GrayScale(float4 aColor)
{
	return (aColor.r*0.3 + aColor.g * 0.59 + aColor.b * 0.11);
}

float FogOfWarValue(PixelInput aPixelInput)
{
	float2 vertexPositionOnTerrain = (aPixelInput.myWorldPosition.xz) / TerrainSize;

	float4 fogOfWar = FogOfWarTexture.Sample(DefaultSampler, vertexPositionOnTerrain);
	float4 blackMask = BlackMaskTexture.Sample(DefaultSampler, vertexPositionOnTerrain);

	float blackMaskOnly = saturate(blackMask.r) * (1 - fogOfWar.r);
	float viewMask = fogOfWar.r;
	float undiscoveredMask = 1 - (blackMaskOnly + viewMask);

	//float4 grayScale = GrayScale(aColor);

	//outColor += undiscoveredMask * grayScale * 0.0f;

	return (blackMaskOnly * 0.3f) + viewMask;
}

float CalculateShadowFactor(Texture2D aShadowMap, float2 aTexturePosition, float aPixelDepth)
{
	//Percentage closure filtering
float percentLit = 0.f;
const float texelSize = 1.f / 4096.f;
const float2 offsets[9] =
{
	float2(-texelSize, -texelSize), float2(0.0f, -texelSize), float2(texelSize, -texelSize),
	float2(-texelSize, 0.0f), float2(0.0f, 0.0f), float2(texelSize, 0.0f),
	float2(-texelSize, texelSize), float2(0.0f, texelSize), float2(texelSize, texelSize)
};

[unroll]
for(int i = 0; i < 9; ++i)
{
	percentLit += aShadowMap.SampleCmpLevelZero(PercentageClosureFilteringSampler, aTexturePosition + offsets[i], aPixelDepth);
}

return percentLit /= 9.0f;

}

float4 GetPBLTotalLight(PixelInput aPixelInput)
{
	PixelData data = GetPixelData(aPixelInput);

  PBLInput pblInput;
  pblInput.myAlbedoColor = data.myAlbedo.xyz;
  pblInput.myEmissiveColor = EmissiveTexture.Sample(DefaultSampler, aPixelInput.myTexturePosition).xyz;
  pblInput.myMetalness = data.myMetalness.r;
  pblInput.myRoughness = data.myRoughness.r;
  pblInput.myAmbientOcclusion = data.myAmbientOcclusion.r;

	pblInput.myPixelWorldPosition = aPixelInput.myWorldPosition.xyz;
	pblInput.myWorldNormalDirection = data.myNormal;

	pblInput.myCameraPosition = myCameraPosition;
	pblInput.myCameraDirection = myViewDirection;

	DirectionalLight directionalLight;
	directionalLight.myColor = myDirectionalLightColor.xyz;
	directionalLight.myDirection = myDirectionalLightDirection;
	directionalLight.myShadowGradient = 0;

	float2 samplePoint = aPixelInput.myDirectionalLightShadowCoordinates.xy;
	float compareValue = aPixelInput.myDirectionalLightShadowCoordinates.z;
	directionalLight.myShadowGradient = CalculateShadowFactor(DirectionalShadowMap, samplePoint, compareValue);


	pblInput.myDirectionalLights[0] = directionalLight;

	pblInput.myLightData = myLightData;
	//float testColor = compareValue * 0.01;
	//return float4(testColor, testColor, testColor, 1);
	float directionalLightModifier = abs(-1 * FogOfWarValue(aPixelInput));
	return float4(PhysicallyBasedLighting(pblInput, directionalLightModifier), 1);
}

ReturnData PixelShaderFunction_MappedNormals(PixelInput aPixelInput) : SV_Target
{

	ReturnData returnData;
	returnData.myVelocity = CalculateVelocity(aPixelInput);
	PixelData data = GetPixelData(aPixelInput);

	returnData.myColor = saturate(float4(abs(data.myNormal), 1));

	return returnData;
}

ReturnData PixelShaderFunction_Albedo(PixelInput aPixelInput) : SV_Target
{
	ReturnData returnData;
	returnData.myVelocity = CalculateVelocity(aPixelInput);
	PixelData data = GetPixelData(aPixelInput);
	returnData.myColor = saturate(float4(data.myAlbedo.xyz, 1));
	return returnData;
}

ReturnData PixelShaderFunction_MeshNormals(PixelInput aPixelInput) : SV_Target
{
	ReturnData returnData;
	returnData.myVelocity = CalculateVelocity(aPixelInput);
	returnData.myColor = saturate(float4(abs(aPixelInput.myNormal), 1));
	returnData.myColor.a = 1;

	return returnData;
}

#include "ShaderFunctions.fx"

#define NUMBER_OF_SPLAT_MAPS 2

Texture2D SplatMaps[NUMBER_OF_SPLAT_MAPS];

Texture2D SplatAlbedo[NUMBER_OF_SPLAT_MAPS * 4];
Texture2D SplatNormal[NUMBER_OF_SPLAT_MAPS * 4];
Texture2D SplatMetalness[NUMBER_OF_SPLAT_MAPS * 4];
Texture2D SplatAO[NUMBER_OF_SPLAT_MAPS * 4];
Texture2D SplatRoughness[NUMBER_OF_SPLAT_MAPS * 4];
Texture2D SplatEmissive[NUMBER_OF_SPLAT_MAPS * 4];

float2 SplatTextureSizes[NUMBER_OF_SPLAT_MAPS * 4];

struct TerrainVertexInput
{
	float3 myPosition : POSITION;
	float3 myNormal : NORMAL;
	float2 myTextureCoordinates : TEXCOORD;
	float3 myBiNormal : BINORMAL;
	float3 myTangent : TANGENT;
};

struct TerrainPixelInput
{
	float4 myPosition : SV_POSITION;
	float4 myWorldPosition : POSITION;
	float3 myNormal : NORMAL;
	float2 myTextureCoordinates : TEXCOORD;
	float3 myBiNormal : BINORMAL;
	float3 myTangent : TANGENT;
	float4 myDirectionalLightShadowCoordinates : TEXCOORD1;
};

TerrainPixelInput VertexShaderFunction(TerrainVertexInput anInput)
{
	TerrainPixelInput output = (TerrainPixelInput)0;

	output.myWorldPosition = mul(myToWorldMatrix, float4(anInput.myPosition, 1));
	output.myPosition = mul(myWorldViewProjection, float4(anInput.myPosition,1));
	output.myNormal = normalize(anInput.myNormal);
	output.myTextureCoordinates = anInput.myTextureCoordinates;
	output.myBiNormal = normalize(anInput.myBiNormal);
	output.myTangent = normalize(anInput.myTangent);

	output.myDirectionalLightShadowCoordinates = mul(myDirectionalLightViewProjection, output.myWorldPosition);
	output.myDirectionalLightShadowCoordinates.xyz = output.myDirectionalLightShadowCoordinates.xyz / output.myDirectionalLightShadowCoordinates.w;
	output.myDirectionalLightShadowCoordinates.y = output.myDirectionalLightShadowCoordinates.y * -1.f;
	output.myDirectionalLightShadowCoordinates.xy = output.myDirectionalLightShadowCoordinates.xy + 1;
	output.myDirectionalLightShadowCoordinates.xy = output.myDirectionalLightShadowCoordinates.xy * 0.5f;
	output.myDirectionalLightShadowCoordinates.z -= 0.00001;

	return output;
}

float4 GetHighestColor(float4 color1, float factor1, float4 color2, float factor2)
{
	float depth = 0.2;
	float ma = max(color1.a + factor1, color2.a + factor2) - depth;

	float b1 = max(color1.a + factor1 - ma, 0);
	float b2 = max(color2.a + factor2 - ma, 0);

	return float4((color1.rgb * b1 + color2.rgb * b2) / (b1 + b2), 1);
}

float3 Slerp(float3 p0, float3 p1, float t)
{
  float dotp = dot(normalize(p0), normalize(p1));
  if ((dotp > 0.9999) || (dotp<-0.9999))
  {
    if (t<=0.5)
      return p0;
    return p1;
  }
  float theta = acos(dotp * 3.1415926535 / 180);
  float3 P = ((p0*sin((1-t)*theta) + p1*sin(t*theta)) / sin(theta));
  return P;
}

float4 PixelShaderFunctionPBL(TerrainPixelInput anInput) : SV_Target
{
	float2 fogOfWarTexCoord = anInput.myTextureCoordinates;
	fogOfWarTexCoord.y = 1 - fogOfWarTexCoord.y;
	float4 fogOfWar = FogOfWarTexture.SampleLevel(DefaultSampler, fogOfWarTexCoord, 8);
	float4 blackMask = BlackMaskTexture.SampleLevel(DefaultSampler, fogOfWarTexCoord, 8);

	PBLInput pblIn = (PBLInput)0;
	pblIn.myPixelWorldPosition = anInput.myWorldPosition;
	pblIn.myCameraPosition = myCameraPosition;
	pblIn.myCameraDirection = myViewDirection;

	pblIn.myDirectionalLights[0].myDirection = myDirectionalLightDirection;
	pblIn.myDirectionalLights[0].myColor = myDirectionalLightColor;
	pblIn.myDirectionalLights[0].myShadowGradient = 0;

	float2 samplePoint = anInput.myDirectionalLightShadowCoordinates.xy;
		float compareValue = anInput.myDirectionalLightShadowCoordinates.z;
	pblIn.myDirectionalLights[0].myShadowGradient = CalculateShadowFactor(DirectionalShadowMap, samplePoint, compareValue);

	float3 textureNormal = float3(0, 0, 0);

	float rest = 1;

	[unroll] for (int splatIndex = 0; splatIndex < NUMBER_OF_SPLAT_MAPS; splatIndex++)
	{
		float4 splatMap = SplatMaps[splatIndex].SampleLevel(DefaultSampler, anInput.myTextureCoordinates, 0);

		float2 rUV			= (anInput.myTextureCoordinates * (TerrainSize / SplatTextureSizes[splatIndex * 4 + 0]));
		float2 gUV			= (anInput.myTextureCoordinates * (TerrainSize / SplatTextureSizes[splatIndex * 4 + 1]));
		float2 bUV			= (anInput.myTextureCoordinates * (TerrainSize / SplatTextureSizes[splatIndex * 4 + 2]));
		float2 aUV			= (anInput.myTextureCoordinates * (TerrainSize / SplatTextureSizes[splatIndex * 4 + 3]));

		float3 rAlbedo		= SplatAlbedo	[splatIndex * 4 + 0].Sample(DefaultSampler, rUV).xyz;
		float3 gAlbedo		= SplatAlbedo	[splatIndex * 4 + 1].Sample(DefaultSampler, gUV).xyz;
		float3 bAlbedo		= SplatAlbedo	[splatIndex * 4 + 2].Sample(DefaultSampler, bUV).xyz;
		float3 aAlbedo		= SplatAlbedo	[splatIndex * 4 + 3].Sample(DefaultSampler, aUV).xyz;

		float3 rNormal		= SplatNormal	[splatIndex * 4 + 0].Sample(DefaultSampler, rUV).xyz;
		float3 gNormal		= SplatNormal	[splatIndex * 4 + 1].Sample(DefaultSampler, gUV).xyz;
		float3 bNormal		= SplatNormal	[splatIndex * 4 + 2].Sample(DefaultSampler, bUV).xyz;
		float3 aNormal		= SplatNormal	[splatIndex * 4 + 3].Sample(DefaultSampler, aUV).xyz;

		float3 rMetalness	= SplatMetalness[splatIndex * 4 + 0].Sample(DefaultSampler, rUV).xyz;
		float3 gMetalness	= SplatMetalness[splatIndex * 4 + 1].Sample(DefaultSampler, gUV).xyz;
		float3 bMetalness	= SplatMetalness[splatIndex * 4 + 2].Sample(DefaultSampler, bUV).xyz;
		float3 aMetalness	= SplatMetalness[splatIndex * 4 + 3].Sample(DefaultSampler, aUV).xyz;

		float3 rAO			= SplatAO		[splatIndex * 4 + 0].Sample(DefaultSampler, rUV).xyz;
		float3 gAO			= SplatAO		[splatIndex * 4 + 1].Sample(DefaultSampler, gUV).xyz;
		float3 bAO			= SplatAO		[splatIndex * 4 + 2].Sample(DefaultSampler, bUV).xyz;
		float3 aAO			= SplatAO		[splatIndex * 4 + 3].Sample(DefaultSampler, aUV).xyz;

		float3 rRoughness	= SplatRoughness[splatIndex * 4 + 0].Sample(DefaultSampler, rUV).xyz;
		float3 gRoughness	= SplatRoughness[splatIndex * 4 + 1].Sample(DefaultSampler, gUV).xyz;
		float3 bRoughness	= SplatRoughness[splatIndex * 4 + 2].Sample(DefaultSampler, bUV).xyz;
		float3 aRoughness	= SplatRoughness[splatIndex * 4 + 3].Sample(DefaultSampler, aUV).xyz;

		float3 rEmissive	= SplatEmissive	[splatIndex * 4 + 0].Sample(DefaultSampler, rUV).xyz;
		float3 gEmissive	= SplatEmissive	[splatIndex * 4 + 1].Sample(DefaultSampler, gUV).xyz;
		float3 bEmissive	= SplatEmissive	[splatIndex * 4 + 2].Sample(DefaultSampler, bUV).xyz;
		float3 aEmissive	= SplatEmissive	[splatIndex * 4 + 3].Sample(DefaultSampler, aUV).xyz;

		pblIn.myAlbedoColor += (rAlbedo * splatMap.r) + (gAlbedo * splatMap.g) + (bAlbedo * splatMap.b) + (aAlbedo * splatMap.a);
		rNormal = Slerp(float3(0, 0, 0.5), rNormal, splatMap.r);
		gNormal = Slerp(rNormal, gNormal, splatMap.g);
		bNormal = Slerp(gNormal, bNormal, splatMap.b);
		aNormal = Slerp(bNormal, aNormal, splatMap.a);

		textureNormal = normalize(lerp(textureNormal, aNormal, rest));
		rest = 1 - (splatMap.r + splatMap.g + splatMap.b + splatMap.a);
		pblIn.myMetalness += (rMetalness.r * splatMap.r) + (gMetalness.r * splatMap.g) + (bMetalness.r * splatMap.b) + (aMetalness.r * splatMap.a);
		pblIn.myEmissiveColor += (rEmissive * splatMap.r) + (gEmissive * splatMap.g) + (bEmissive * splatMap.b) + (aEmissive * splatMap.a);
		pblIn.myRoughness += (rRoughness.r * splatMap.r) + (gRoughness.r * splatMap.g) + (bRoughness.r * splatMap.b) + (aRoughness.r * splatMap.a);
		pblIn.myAmbientOcclusion += (rAO.r * splatMap.r) + (gAO.r * splatMap.g) + (bAO.r * splatMap.b) + (aAO.r * splatMap.a);
	}

	// Correct normal and change it into world-space.
	float3 normalizedNormals = normalize((textureNormal.xyz * 2) - 1);
	//return float4(abs(normalizedNormals), 1);
	float3 tangent = normalize(cross(normalize(anInput.myNormal - float3(0, 0, 1)), anInput.myNormal));
	float3 biNormal = normalize(cross(anInput.myNormal, tangent));
	float3x3 tbnMatrix = (float3x3(normalize(tangent), normalize(biNormal), normalize(anInput.myNormal)));
	pblIn.myWorldNormalDirection = normalize(mul(normalizedNormals, tbnMatrix));

	//return float4(abs(pblIn.myWorldNormalDirection), 1);

	pblIn.myLightData = myLightData;

	float4 pblColor = float4(0, 0, 0, 1);
	pblColor = float4(PhysicallyBasedLighting(pblIn, abs(-1 * fogOfWar.r)), 1);

	PixelInput pixelInput;
	pixelInput.myWorldPosition = anInput.myWorldPosition;
	float4 outColor = 0;
	outColor.xyz = DistanceFog(pblColor.xyz, pblIn.myPixelWorldPosition, float3(0.13, 0.08, 0.76), pblIn.myCameraPosition);
	outColor.xyz *= FogOfWarValue(pixelInput);

	return outColor;
}

technique11 RenderTerrainUnlit
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VertexShaderFunction()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PixelShaderFunctionPBL()));
		SetRasterizerState(Solid);
		SetDepthStencilState(EnableDepth, 0);
		SetBlendState(NoBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
	}
};

technique11 Render
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VertexShaderFunction()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PixelShaderFunctionPBL()));
		SetRasterizerState(Solid);
		SetDepthStencilState(EnableDepth, 0);
		SetBlendState(NoBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
	}
};

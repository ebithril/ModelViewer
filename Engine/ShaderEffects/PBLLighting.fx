///////////////////////////////////////////////////////////////////////////
// * PBLLighting.fx
///////////////////////////////////////////////////////////////////////////
// If you need to include something here, you're probably doing it wrong. =)

//-------------------------------------------------------------------------
// * Generic Lighting Functions
//-------------------------------------------------------------------------
#ifndef PI
#define PI 3.14159265358f
#endif

#ifndef NUMBER_OF_POINT_AND_SPOT_LIGHTS
#define NUMBER_OF_POINT_AND_SPOT_LIGHTS 8
#endif


static const float globalK0 = 0.00098f;
static const float globalK1 = 0.9921f;
static const float globalFakeLysMaxSpecularPower = (2.f / (0.0014f * 0.0014f)) - 2.f;
static const float globalFMaxT = (exp2(-10.f / sqrt((2.f / (0.0014f * 0.0014f)) - 2.f)) - 0.00098f) / 0.9921f;

float AngleAttenuation(float3 aLightPosition, float aLightAngle, float3 aLightDirection, float3 aWorldPosition)
{
	float maxLightAngle = aLightAngle * 0.5;

	float3 lightDirection = normalize(aWorldPosition - aLightPosition);
	float lightAngle = acos(dot(lightDirection, aLightDirection));
	float attenuation = max(maxLightAngle * maxLightAngle - lightAngle * lightAngle, 0);
	attenuation = attenuation / maxLightAngle;

	return attenuation;
}
float RangeAttenuation(float3 aLightPos, float aLightRange, float3 aWorldPosition)
{
	float3 lightDir = aLightPos - aWorldPosition;
	float lengthToLight = length(lightDir);
	float b = lengthToLight / (aLightRange / 2.f);
	float attenuation = 1 - saturate(b);
	//attenuation -= 1;
	//attenuation = abs(attenuation);

	return attenuation;
}

float3 Fresnel(const float3 substance, const float3 lightDir, const float3 halfVec)
{
	float3 oneMinusSubstance = 1.f - substance;
		float LdotH = saturate(dot(lightDir, halfVec));
	float oneMinusLdotH = (1.f - LdotH);
	float oneMinusLdotHPow5 = pow(oneMinusLdotH, 5);
	float3 oneMinusSubstanceTimesOneMinusLdotHPow5 = oneMinusSubstance * oneMinusLdotHPow5;
		float3 substancePlusOneMinusSubstanceTimesOneMinusLdotHPow5 = substance + oneMinusSubstanceTimesOneMinusLdotHPow5;
		return (substancePlusOneMinusSubstanceTimesOneMinusLdotHPow5);
}

float3 ReflectionFresnel(const float3 aSubstance, const float3 aLightDirection, const float3 aHalfVector, float aRoughness)
{
	return (aSubstance + (1.f - aSubstance) * pow(1.f - saturate(dot(aLightDirection, aHalfVector)), 5) / (4 - 3 * aRoughness));
}

float RoughToSpecularPower(float aRoughness)
{
	return (2.f / (aRoughness * aRoughness)) - 2.f;
}

float GetSpecularPowerToMip(float aSpecularPower, int aNumberOfMipsLevels)
{
	float fSmulMaxT = (exp2(-10.0 / sqrt(aSpecularPower)) - globalK0) / globalK1;

	return float(aNumberOfMipsLevels - 1) * (1.0 - clamp(fSmulMaxT / globalFMaxT, 0.0, 1.0));
}

float D_GGX(float aHalfVectorDotNormal, float aRoughness)
{
	float m = aRoughness * aRoughness;
	float m2 = m*m;
	float Denominator = aHalfVectorDotNormal * aHalfVectorDotNormal * (m2 - 1.f) + 1.f;
	float D = m2 / (PI * Denominator * Denominator);
	return D;
}

float V_Schlick_GGX(float aRoughness, float NdotV, float NdotL)
{
	float k = aRoughness * aRoughness * 0.5f;
	float G1V = NdotV * (1.f - k) + k;
	float G1L = NdotL * (1.f - k) + k;
	return (0.25f / (G1V * G1L));
}

//-------------------------------------------------------------------------
// * Physically Based Lighting
//     Standard PBL calculations
//-------------------------------------------------------------------------

// * PBL Configuration

// Ambient settings - If none is specified, there is no ambient lighting.
//#define PBL_AMBIENT_USECUBEMAP // Not implemented
//#define PBL_AMBIENT_COLOR float3(0.6f, 0.6f, 0.6f)
#define PBL_TRILIGHT_SKY float3(88.f / 255.f, 138.f / 255.f, 233.f / 255.f)
#define PBL_TRILIGHT_EQU float3(160.f / 255.f, 160.f / 255.f, 160.f / 255.f)
#define PBL_TRILIGHT_GRN float3(95.f / 255.f, 104.f / 255.f, 95.f / 255.f)

// Limit settings
#define PBL_DIRECTIONAL_LIGHT_AMOUNT 1
#define PBL_MAX_MIPMAP 12

struct DirectionalLight
{
	float3 myDirection;
	float3 myColor;
	float myShadowGradient;
};

struct LightData
{
	float3 myColor;
	float myRange;
	float3 myDirection;
	int myLightType;
	float3 myPosition;
	float myAngle;
};


struct PBLInput
{
	float3 myAlbedoColor;
	float3 myEmissiveColor;
	float myMetalness;
	float myRoughness;
	float myAmbientOcclusion;

	float3 myPixelWorldPosition;
	float3 myWorldNormalDirection;

	float3 myCameraPosition;
	float3 myCameraDirection;

	DirectionalLight myDirectionalLights[PBL_DIRECTIONAL_LIGHT_AMOUNT];
	LightData myLightData[NUMBER_OF_POINT_AND_SPOT_LIGHTS];
};

const float3 CalculateLight(PBLInput anInput, float3 aLightColor, float3 aLightDirection,float anAttenuation)
{
	float3 color = (0,0,0);

	float3 pixelMetalColor = anInput.myAlbedoColor - (anInput.myAlbedoColor * anInput.myMetalness);
	float3 pixelNonMetalColor = (0.04f - 0.04f * anInput.myMetalness) + anInput.myAlbedoColor * anInput.myMetalness;

	float3 eyeDirection = normalize(anInput.myPixelWorldPosition - anInput.myCameraPosition);
	float NdotV = saturate(dot(anInput.myWorldNormalDirection, eyeDirection));

	float3 negLightDir = -normalize(aLightDirection);
	float3 halfVector = normalize(negLightDir - eyeDirection);

	float NdotL = saturate(dot(anInput.myWorldNormalDirection, negLightDir));
	float HdotN = saturate(dot(halfVector, anInput.myWorldNormalDirection));

	float3 F = saturate(Fresnel(pixelNonMetalColor, negLightDir, halfVector));
	float D = saturate(D_GGX(HdotN, (anInput.myRoughness + 1) / 2));
	float V = saturate(V_Schlick_GGX((anInput.myRoughness + 1) / 2, NdotV, NdotL));

	float3 spec = ((D * V * F) * NdotL);
	float3 diffuse = (1 - F) * NdotL * pixelMetalColor * anInput.myAmbientOcclusion;

	color = ((spec + diffuse) / PI) * aLightColor;
	color *= anAttenuation * 3;
	return color;
}

const float3 CalculateDirectionalLight(PBLInput anInput, DirectionalLight aLight)
{
	return aLight.myShadowGradient * CalculateLight(anInput, aLight.myColor , aLight.myDirection, 1); 
}

const float3 CalculatePointLight(PBLInput anInput, LightData aLight)
{
	float attenuation = RangeAttenuation(aLight.myPosition, aLight.myRange, anInput.myPixelWorldPosition);
	return CalculateLight(anInput, aLight.myColor, normalize(anInput.myPixelWorldPosition - aLight.myPosition), attenuation);
}

const float3 CalculateSpotLight(PBLInput anInput, LightData aLight)
{
	float3 lightDirection = normalize(anInput.myPixelWorldPosition - aLight.myPosition);
	float attenuation = RangeAttenuation(aLight.myPosition, aLight.myRange, anInput.myPixelWorldPosition);
	attenuation *= AngleAttenuation(aLight.myPosition, aLight.myAngle, aLight.myDirection, anInput.myPixelWorldPosition);
	return CalculateLight(anInput, aLight.myColor, lightDirection, attenuation);
}

float3 GetSkyColor(float3 direction)
{
#if defined(PBL_AMBIENT_COLOR)
	return PBL_AMBIENT_COLOR;
	//#elif defined(PBL_AMBIENT_USECUBEMAP)
	//return AmbientCubeMap.Sample(DefaultSampler, direction).xyz;
#elif defined(PBL_TRILIGHT_SKY)
	float NdotL = dot(direction, float3(0, 1, 0));// * rsqrt(dot(pixelNormal, pixelNormal));
	float3 trilight = 0;
		trilight += saturate(NdotL) * PBL_TRILIGHT_SKY;
	trilight += (1 - abs(NdotL)) * PBL_TRILIGHT_EQU;
	trilight += saturate(-NdotL) * PBL_TRILIGHT_GRN;

	return trilight;
#else
	return float3(0, 0, 0);
#endif
}

float3 GetSkyColor(float3 direction, int mipLevel)
{
#if defined(PBL_AMBIENT_COLOR)
	return PBL_AMBIENT_COLOR;
//#elif defined(PBL_AMBIENT_USECUBEMAP)
	//return AmbientCubeMap.SampleLevel(DefaultSampler, direction, mipLevel).xyz;
#elif defined(PBL_TRILIGHT_SKY)
	float NdotL = dot(direction, float3(0, 1, 0));// * rsqrt(dot(pixelNormal, pixelNormal));
	float3 trilight = 0;
		trilight += saturate(NdotL) * PBL_TRILIGHT_SKY;
	trilight += (1 - abs(NdotL)) * PBL_TRILIGHT_EQU;
	trilight += saturate(-NdotL) * PBL_TRILIGHT_GRN;

	float3 blurredTri = saturate((PBL_TRILIGHT_SKY + PBL_TRILIGHT_EQU + PBL_TRILIGHT_GRN) / 3.f);

	return lerp(trilight, blurredTri, mipLevel / PBL_MAX_MIPMAP);
#else
	return float3(0, 0, 0);
#endif
}

float3 GetReflectedSkyColor(float3 pixelPosition, float3 pixelNormal, float3 aCameraPosition)
{
	float3 eyeDir = normalize(pixelPosition - aCameraPosition);
	float3 reflectionVector = reflect(eyeDir, pixelNormal);

	return GetSkyColor(reflectionVector);
}

float3 GetReflectedSkyColor(float3 pixelPosition, float3 pixelNormal, int mipLevel, float3 aCameraPosition)
{
	float3 eyeDir = normalize(pixelPosition - aCameraPosition);
	float3 reflectionVector = reflect(eyeDir, pixelNormal);

	return GetSkyColor(reflectionVector, mipLevel);
}

const float3 CalculateAmbientColor(PBLInput anInput)
{
	float3 pixelMetalColor = anInput.myAlbedoColor - (anInput.myAlbedoColor * anInput.myMetalness);
	float3 pixelNonMetalColor = (0.04f - 0.04f * anInput.myMetalness) + anInput.myAlbedoColor * anInput.myMetalness;

	float3 eyeDirection = normalize(anInput.myPixelWorldPosition - anInput.myCameraPosition).xyz;
	float3 reflectionFresnel = ReflectionFresnel(pixelNonMetalColor, anInput.myWorldNormalDirection, -eyeDirection, 1 - pow(8192, anInput.myRoughness)).xyz;

	float3 reflectedSkyColor = GetReflectedSkyColor(anInput.myPixelWorldPosition.xyz, anInput.myWorldNormalDirection, 7, anInput.myCameraPosition);
	float3 ambientDiffuse = (reflectedSkyColor * (1 - reflectionFresnel) * anInput.myAmbientOcclusion * pixelMetalColor).xyz;

	float fakeLysSpecularPower = RoughToSpecularPower(anInput.myRoughness);
	float lysMipMap = GetSpecularPowerToMip(fakeLysSpecularPower, PBL_MAX_MIPMAP);

	float3 ambientSpecular = (GetReflectedSkyColor(anInput.myPixelWorldPosition.xyz, anInput.myWorldNormalDirection, lysMipMap, anInput.myCameraPosition).xyz * anInput.myAmbientOcclusion * (reflectionFresnel));//* (1 - anInput.myRoughness)).xyz;
	float3 ambientLighting = ambientDiffuse + ambientSpecular;

	return ambientLighting;
}

const float3 CalculateDirectLight(PBLInput anInput)
{
	float3 finalLightColor = float3(0, 0, 0);

	[unroll] for (uint i = 0; i < PBL_DIRECTIONAL_LIGHT_AMOUNT; ++i)
	{
		finalLightColor += CalculateDirectionalLight(anInput, anInput.myDirectionalLights[i]);
	}

	for (uint j = 0; j< NUMBER_OF_POINT_AND_SPOT_LIGHTS; ++j)
	{
		if (anInput.myLightData[j].myLightType == 0)
		{
			break;
		}
		/*else if(anInput.myLightData[j].myLightType == 1)
		{
			finalLightColor += CalculateSpotLight(anInput, anInput.myLightData[j]);
		}*/
		else
		{
			finalLightColor += CalculatePointLight(anInput, anInput.myLightData[j]);
		}
	}
	return finalLightColor;
}

const float3 CalculateEmissiveLight(PBLInput anInput)
{
	return anInput.myEmissiveColor;
}

const float3 PhysicallyBasedLighting(PBLInput anInput, float aDirectLightModifier)
{
	anInput.myWorldNormalDirection = normalize(anInput.myWorldNormalDirection);

	float3 ambientLight = CalculateAmbientColor(anInput);
	float3 directLight = 0;

		directLight = CalculateDirectLight(anInput) * aDirectLightModifier;
	float3 emissiveLight = CalculateEmissiveLight(anInput) * 2;

	float3 totalColor = ambientLight + directLight + emissiveLight;

	return totalColor;
}



//float3 CalculateSpotLights(PixelInput aPSInput, PixelData somePixelData)
//{
//	float3 totalColor = 0;
//
//	for (uint i = 0; i < NUMBER_OF_SPOT_LIGHTS; i++)
//	{
//		float3 color = 0;
//			float visibility = 1.0;
//
//
//		float2 samplePoint = aPSInput.mySpotLightShadowCoordinates[i].xy;
//			float sampleValue = SpotLightShadowMaps[i].Sample(ClampSampler, samplePoint).x;
//		float compareValue = aPSInput.mySpotLightShadowCoordinates[i].z;
//		if (sampleValue < compareValue)
//		{
//			continue;
//		}
//
//			//float color1 = SpotLightShadowMaps[i].Sample(DefaultSampler, aPSInput.mySpotLightShadowCoordinates[i].xy).r;
//			////float3 color = saturate(float3(255, 8, 127));
//			////color = float3(color1, color1, color1);
//			// color.x = aPSInput.mySpotLightShadowCoordinates[i].z;
//
//			//return color;
//			//return color1;
//
//			//float compareDepth = 1.f - 1.f / aPSInput.mySpotLightShadowCoordinates[i].z;
//
//
//		float3 eyeDirection = normalize(aPSInput.myWorldPosition - myCameraPosition);
//			float NdotV = saturate(dot(somePixelData.myNormal, eyeDirection));
//
//		float attenuation = RangeAttenuation(mySpotLightPositionAndRange[i].xyz, mySpotLightPositionAndRange[i].w, aPSInput);
//		attenuation *= AngleAttenuation(mySpotLightPositionAndRange[i].xyz, mySpotLightDirectionAndAngle[i].w * 0.5f, mySpotLightDirectionAndAngle[i].xyz, aPSInput);
//
//		float3 lightDir = normalize(mySpotLightPositionAndRange[i].xyz - aPSInput.myWorldPosition);
//
//			float3 halfVec = normalize(lightDir - eyeDirection);
//
//			float NdotL = saturate(dot(somePixelData.myNormal, lightDir));
//		float HdotN = saturate(dot(halfVec, somePixelData.myNormal));
//
//		float3 F = saturate(Fresnel(somePixelData.mySubstance, lightDir, halfVec));
//			float D = saturate(D_GGX(HdotN, (somePixelData.myRoughness + 1) / 2));
//		float V = saturate(V_Schlick_GGX((somePixelData.myRoughness + 1) / 2, NdotV, NdotL));
//
//		float3 spec = (D * V * F) * NdotL;
//		float3 diffuse = (1 - F) * NdotL* somePixelData.myMetalnessAlbedo * somePixelData.myAmbientOcclusion;
//
//		color = ((spec + diffuse) / 3.14159265358f) * mySpotLightColor[i];
//		color *= min(attenuation, 1);
//
//		totalColor += color * saturate(visibility);
//	}
//		return totalColor;
//}
//

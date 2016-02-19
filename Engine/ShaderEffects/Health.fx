#include "Default.fx"

float HealthPercentage;
float HealthPercentageDelayed;

BlendState HP_BLENDING
{
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
	DestBlend = INV_SRC_ALPHA;
	BlendOp = ADD;
	SrcBlendAlpha = ONE;
	DestBlendAlpha = ONE;
	BlendOpAlpha = ADD;
	RenderTargetWriteMask[0] = 0x0F;

	/*AlphaToCoverageEnable = FALSE;
	BlendEnable[0] = TRUE;*/
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PixelInput VertexShaderFunction(VertexInput aVertexInput)
{
	PixelInput vertexOutput;
	float4 worldPosition;

	myToViewMatrix;
	myToWorldMatrix;
	myProjectionMatrix;


	vertexOutput.myPosition = mul(myWorldViewProjection, float4(aVertexInput.myPosition, 1.0f));
	vertexOutput.myPixelPositionInProjectionSpace = mul(myWorldViewProjection, float4(aVertexInput.myPosition, 1.0f));
	vertexOutput.myPixelPositionInProjectionSpace.z = 0;

	vertexOutput.myTexturePosition = aVertexInput.myTexturePosition;


	vertexOutput.myNormal = mul((float3x3)myToWorldMatrix, aVertexInput.myNormal);
	vertexOutput.myNormal = normalize(vertexOutput.myNormal);

	vertexOutput.myWorldPosition = mul(myToWorldMatrix,float4(aVertexInput.myPosition, 1.0f));
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

	float animTheta = -(myTotalTime * 0.5) + aPixelInput.myTexturePosition.y;
	float animFunction = (sin(animTheta * 4) * 6) - 5;
	animFunction *= 0.08;
	animFunction = saturate(animFunction);

	float2 texPos = aPixelInput.myTexturePosition + float2((animFunction * animFunction) * 0.17, 0);

	float4 falloff = AOTexture.Sample(DefaultSampler, texPos);

	float cutoffTex = saturate(abs(aPixelInput.myPixelPositionInProjectionSpace.x));
	float4 col = DiffuseTexture.Sample(DefaultSampler, texPos);

	if (falloff.x >= HealthPercentage)
	{
		if (falloff.x >= HealthPercentageDelayed)
		{
				discard;
		}

		col.xyz = float3(1.0, 0.2, 0.2);
	}

	col += float4(animFunction * 1, animFunction, animFunction, 0);
	return  saturate(col);
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
		SetBlendState(HP_BLENDING, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
	}
}

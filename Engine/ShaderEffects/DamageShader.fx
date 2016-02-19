#include "Default.fx"

float fadeTime;
float startTime = 0.5;

BlendState Damage_Blend
{
	BlendEnable[0] = TRUE;
	SrcBlend = ONE;
	DestBlend = ONE;
	BlendOp = ADD;
	SrcBlendAlpha = ONE;
	DestBlendAlpha = ONE;
	BlendOpAlpha = ADD;
	RenderTargetWriteMask[0] = 0x0F;

	/*AlphaToCoverageEnable = FALSE;
	BlendEnable[0] = TRUE;*/
};

PixelInput vertexShaderFunction(VertexInput aVertexInput)
{
	PixelInput vertexOutput = (PixelInput)0;

	vertexOutput.myWorldPosition = mul(myToWorldMatrix, float4(aVertexInput.myPosition, 1));

	vertexOutput.myPosition = mul(myWorldViewProjection, float4(aVertexInput.myPosition, 1.0f));

	vertexOutput.myTexturePosition = aVertexInput.myTexturePosition;

	vertexOutput.myNormal = mul(aVertexInput.myNormal, (float3x3) myToWorldMatrix);
	vertexOutput.myNormal = normalize(vertexOutput.myNormal);

	vertexOutput.myViewDirection = myCameraPosition.xyz - vertexOutput.myWorldPosition.xyz;
	vertexOutput.myViewDirection = normalize(vertexOutput.myViewDirection);

	vertexOutput.myBiNormal = mul((float3x3) myToWorldMatrix, aVertexInput.myBiNormal);

	vertexOutput.myTangent = mul((float3x3) myToWorldMatrix, aVertexInput.myTangent);

	return vertexOutput;
}

float attenuation(float aRange, float aLength)
{
	float b = 1 / max(aRange, 0.00001);

	return 1 - saturate(aLength * b);
}

float4 pixelShaderFunction(PixelInput input) : SV_TARGET
{
    float range = fadeTime / startTime;
	range *= 500;

	float4 playerPos = myToWorldMatrix._m30_m31_m32_m33;
		float4 move = mul(float4(0, 0, -10, 0), myToWorldMatrix);
		float4 positions[2] = { playerPos + move, playerPos - move };

	float4 color = DiffuseTexture.Sample(DefaultSampler, input.myTexturePosition);
		color.a *= saturate(max(attenuation(range, length(float4(input.myWorldPosition + positions[0]).xyz)), attenuation(range, length(float4(input.myWorldPosition + positions[0]).xyz))));

	color.xyz *= color.a * (((cos(input.myWorldPosition.x + myTotalTime * 10) + 1) / 2) + ((sin(input.myWorldPosition.y + myTotalTime * 10) + 1) / 2));

    return color;
}


technique11 Render
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, vertexShaderFunction()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, pixelShaderFunction()));

		SetDepthStencilState(EnableDepth, 0);
		SetBlendState(Damage_Blend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
	}
};
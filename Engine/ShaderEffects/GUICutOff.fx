#include "ShaderFunctions.fx"

RasterizerState NoCulling
{
	CULLMODE = 1;
	FILLMODE = 3;
};


BlendState GUI_Blend
{
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
	DestBlend = INV_SRC_ALPHA;
	BlendOp = ADD;
	SrcBlendAlpha = ZERO;
	DestBlendAlpha = ZERO;
	BlendOpAlpha = ADD;
	RenderTargetWriteMask[0] = 0x0F;

	/*AlphaToCoverageEnable = FALSE;
	BlendEnable[0] = TRUE;*/
};
Texture2D TextTexture;

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PixelInput VertexShaderFunction(VertexInput aVertexInput)
{
	PixelInput vertexOutput = (PixelInput)0;

	vertexOutput.myPosition = mul(myWorldViewProjection, float4(aVertexInput.myPosition, 1.0f));
	vertexOutput.myTexturePosition = aVertexInput.myTexturePosition;

	vertexOutput.myWorldPosition = mul(myToWorldMatrix, float4(aVertexInput.myPosition, 1.0f));

	return vertexOutput;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
ReturnData PixelShaderFunction(PixelInput aPixelInput) : SV_Target
{
	ReturnData returnData;
	returnData.myVelocity = float2(0, 0);
	if (abs(aPixelInput.myTexturePosition.x) > myCutOffValue.x)
	{
		discard;
	}
	if (abs(aPixelInput.myTexturePosition.y) > myCutOffValue.y)
	{
		discard;
	}
	float animTheta = -(myTotalTime * 0.5) + aPixelInput.myTexturePosition.y;

	float animFunction = (sin(animTheta * 4) * 6) - 5;
	animFunction *= 0.08;
	animFunction = saturate(animFunction);

	returnData.myColor = DiffuseTexture.Sample(DefaultSampler, aPixelInput.myTexturePosition + float2((animFunction * animFunction) * 0.17, 0));
	returnData.myColor += float4(animFunction * 0.75, animFunction, animFunction, 0);
	returnData.myColor = returnData.myColor;
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
		SetBlendState(GUI_Blend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetRasterizerState(NoCulling);
	}
}


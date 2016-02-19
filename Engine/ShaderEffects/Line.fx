matrix Camera;
matrix Projection;

float4 VS(float4 pos : POSITION) : SV_POSITION
{
	float4 position = mul(pos, Camera);
	position = mul(position, Projection);

	return position;
}

float4 PS(float4 position : SV_POSITION) : SV_TARGET
{
    return float4( 1,0,1,1 );
}

technique11 Render
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
}

cbuffer cbSettings
{
	float gWeights[17] =
	{
		0.0102015, 0.0178574, 0.0290103, 0.0437388, 0.0612012, 0.0794753, 0.0957820, 0.01071309, 0.01112052, 0.01071309, 0.0957820, 0.0794753, 0.0612012, 0.0437388, 0.0290103, 0.0178574, 0.0102015
	};
};

cbuffer cbFixed
{
	static const int gBlurRadius = 8;
};

Texture2D inputData;
RWTexture2D<float4> outputData;

#define N 256
#define CacheSize (N + 2*gBlurRadius)
groupshared float4 gCache[CacheSize];

[numthreads(N, 1, 1)]
void HorzBlurCS(int3 groupThreadID : SV_GroupThreadID,
	int3 dispatchThreadID : SV_DispatchThreadID)
{
	//
	// Fill local thread storage to reduce bandwidth.  To blur
	// N pixels, we will need to load N + 2*BlurRadius pixels
	// due to the blur radius.
	//

	// This thread group runs N threads.  To get the extra 2*BlurRadius pixels,
	// have 2*BlurRadius threads sample an extra pixel.
	if (groupThreadID.x < gBlurRadius)
	{
		// Clamp out of bound samples that occur at image borders.
		int x = max(dispatchThreadID.x - gBlurRadius, 0);
		gCache[groupThreadID.x] = inputData[int2(x, dispatchThreadID.y)];
	}
	if (groupThreadID.x >= N - gBlurRadius)
	{
		// Clamp out of bound samples that occur at image borders.
		int x = min(dispatchThreadID.x + gBlurRadius, inputData.Length.x - 1);
		gCache[groupThreadID.x + 2 * gBlurRadius] = inputData[int2(x, dispatchThreadID.y)];
	}

	// Clamp out of bound samples that occur at image borders.
	gCache[groupThreadID.x + gBlurRadius] = inputData[min(dispatchThreadID.xy, inputData.Length.xy - 1)];

	// Wait for all threads to finish.
	GroupMemoryBarrierWithGroupSync();

	//
	// Now blur each pixel.
	//

	float4 blurColor = float4(0, 0, 0, 0);

		[unroll]
	for (int i = -gBlurRadius; i <= gBlurRadius; ++i)
	{
		int k = groupThreadID.x + gBlurRadius + i;

		blurColor += gWeights[i + gBlurRadius] * gCache[k];
	}

	outputData[dispatchThreadID.xy] = blurColor;
}

[numthreads(1, N, 1)]
void VertBlurCS(int3 groupThreadID : SV_GroupThreadID,
	int3 dispatchThreadID : SV_DispatchThreadID)
{

	//
	// Fill local thread storage to reduce bandwidth.  To blur
	// N pixels, we will need to load N + 2*BlurRadius pixels
	// due to the blur radius.
	//

	// This thread group runs N threads.  To get the extra 2*BlurRadius pixels,
	// have 2*BlurRadius threads sample an extra pixel.
	if (groupThreadID.y < gBlurRadius)
	{
		// Clamp out of bound samples that occur at image borders.
		int y = max(dispatchThreadID.y - gBlurRadius, 0);
		gCache[groupThreadID.y] = inputData[int2(dispatchThreadID.x, y)];
	}
	if (groupThreadID.y >= N - gBlurRadius)
	{
		// Clamp out of bound samples that occur at image borders.
		int y = min(dispatchThreadID.y + gBlurRadius, inputData.Length.y - 1);
		gCache[groupThreadID.y + 2 * gBlurRadius] = inputData[int2(dispatchThreadID.x, y)];
	}

	// Clamp out of bound samples that occur at image borders.
	gCache[groupThreadID.y + gBlurRadius] = inputData[min(dispatchThreadID.xy, inputData.Length.xy - 1)];


	// Wait for all threads to finish.
	GroupMemoryBarrierWithGroupSync();

	//
	// Now blur each pixel.
	//

	float4 blurColor = float4(0, 0, 0, 0);

		[unroll]
	for (int i = -gBlurRadius; i <= gBlurRadius; ++i)
	{
		int k = groupThreadID.y + gBlurRadius + i;

		blurColor += gWeights[i + gBlurRadius] * gCache[k];
	}

	outputData[dispatchThreadID.xy] = blurColor;
}


[numthreads(32, 32, 1)]
void DownSample2X2CS(int3 groupThreadID : SV_GroupThreadID,
	int3 dispatchThreadID : SV_DispatchThreadID)
{
	outputData[dispatchThreadID.xy] = (inputData[dispatchThreadID.xy * 2] + inputData[dispatchThreadID.xy * 2 + int2(1, 0)] + inputData[dispatchThreadID.xy * 2 + int2(0, 1)] + inputData[dispatchThreadID.xy * 2 + int2(1, 1)]) / 4.0;
}

[numthreads(32, 32, 1)]
void DownSample2X2CSLog(int3 groupThreadID : SV_GroupThreadID,
	int3 dispatchThreadID : SV_DispatchThreadID)
{
	float data = 0;
	for (int x = 0; x<2; x++)
	{
		for (int y = 0; y<2; y++)
		{
			data += log(max(0.0001f, inputData[dispatchThreadID.xy * 2 + int2(x, y)]));
		}
	}

	outputData[dispatchThreadID.xy] = exp(data / 4.0);
}

float LinearToLum(float4 aColor)
{
	return(aColor.r*0.3 + aColor.g*0.59 + aColor.b*0.11);
}

float bloomLevel = 0.9f;

[numthreads(32, 32, 1)]
void DownSample2X2andHiPassCS(int3 groupThreadID : SV_GroupThreadID,
	int3 dispatchThreadID : SV_DispatchThreadID)
{
	float4 data = 0;
		for (int x = 0; x<2; x++)
		{
		for (int y = 0; y<2; y++)
		{
			if (LinearToLum(inputData[dispatchThreadID.xy * 2 + int2(x, y)])>bloomLevel)
			{
				data += inputData[dispatchThreadID.xy * 2 + int2(x, y)];
			}
		}
		}
	outputData[dispatchThreadID.xy] = data / 4.0;
}


technique11 DownSample2x2
{
	pass P0
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, DownSample2X2CS()));
	}
}

technique11 DownSample2x2Log
{
	pass P0
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, DownSample2X2CSLog()));
	}
}

technique11 DownSample2x2AndHiPass
{
	pass P0
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, DownSample2X2andHiPassCS()));
	}
}

technique11 HorzBlur
{
	pass P0
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, HorzBlurCS()));
	}
}

technique11 VertBlur
{
	pass P0
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, VertBlurCS()));
	}
}

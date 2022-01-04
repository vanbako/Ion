#ifndef ObjectConstantBuffer_HLSLI
#define ObjectConstantBuffer_HLSLI

cbuffer ObjectConstantBuffer : register(b1)
{
	float4x4 gWorld;
	float4x4 gWorldViewProj;
	float gShininess;
	float3 gPadding0;
	float4 gPadding1[7];
}

#endif
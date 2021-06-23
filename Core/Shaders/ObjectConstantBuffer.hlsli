#ifndef ObjectConstantBuffer_HLSLI
#define ObjectConstantBuffer_HLSLI

cbuffer ObjectConstantBuffer : register(b1)
{
	float4x4 gWorld;
	float4x4 gWorldViewProj;
	float4 padding0[8];
}

#endif
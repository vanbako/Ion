#ifndef CanvasConstantBuffer_HLSLI
#define CanvasConstantBuffer_HLSLI

cbuffer CanvasConstantBuffer : register(b0)
{
	float3 gLightDirection;
	float padding0;
	float4 gColorDiffuse;
	float4 gColorAmbient;
	float gAmbientIntensity;
	float padding1;
	float padding2;
	float padding3;
	float4 padding4[12];
};

#endif
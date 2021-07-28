#ifndef CanvasConstantBuffer_HLSLI
#define CanvasConstantBuffer_HLSLI

cbuffer CanvasConstantBuffer : register(b0)
{
	float4x4 gView;
	float4x4 gViewProj;
	float3 gLightDirection;
	float gAmbientIntensity;
	float4 gColorDiffuse;
	float4 gColorAmbient;
	float4 cPadding4[5];
};

#endif
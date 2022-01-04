#ifndef CanvasConstantBuffer_HLSLI
#define CanvasConstantBuffer_HLSLI

cbuffer CanvasConstantBuffer : register(b0)
{
	float4x4 gView;
	float4x4 gViewProj;
	float4x4 gViewInverse;
	float4 gViewPos;
	float3 gLightDirection;
	float gAmbientIntensity;
	float4 gColorDiffuse;
	float4 gColorAmbient;
	// Total 256 bytes
};

#endif
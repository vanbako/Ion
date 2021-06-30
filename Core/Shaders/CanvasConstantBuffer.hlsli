#ifndef CanvasConstantBuffer_HLSLI
#define CanvasConstantBuffer_HLSLI

cbuffer CanvasConstantBuffer : register(b0)
{
	float4x4 gViewProj;
	float3 gLightDirection;
	float cPadding0;
	float4 gColorDiffuse;
	float4 gColorAmbient;
	float gAmbientIntensity;
	float cPadding1;
	float cPadding2;
	float cPadding3;
	float4 cPadding4[8];
};

#endif
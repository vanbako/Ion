#ifndef PosNormTanTex_PSInput_HLSLI
#define PosNormTanTex_PSInput_HLSLI

struct PSInput
{
	float4 position: SV_POSITION;
	float4 worldposition: COLOR0;
	float3 normal: NORMAL;
	float3 tangent: TANGENT;
	float2 texcoord: TEXCOORD;
};

#endif
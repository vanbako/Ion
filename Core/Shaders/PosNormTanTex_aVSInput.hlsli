#ifndef PosNormTanTex_VSInput_HLSLI
#define PosNormTanTex_VSInput_HLSLI

struct VSInput
{
	float3 position: POSITION;
	float3 normal: NORMAL;
	float3 tangent: TANGENT;
	float2 texcoord: TEXCOORD;
	int4 boneindices : BLENDINDICES;
	float4 boneweights : BLENDWEIGHT;
};

#endif
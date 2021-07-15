#ifndef BonesBuffer_HLSLI
#define BonesBuffer_HLSLI

cbuffer BonesBuffer : register(b2)
{
	float4x4 gBones[128];
}

#endif
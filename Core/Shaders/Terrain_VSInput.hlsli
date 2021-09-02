#ifndef Terrain_VSInput_HLSLI
#define Terrain_VSInput_HLSLI

struct VSInput
{
	float3 position: POSITION;
	float3 normal: NORMAL;
	float2 texcoord: TEXCOORD;
};

#endif
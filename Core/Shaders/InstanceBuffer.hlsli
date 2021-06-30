#ifndef InstanceBuffer_HLSLI
#define InstanceBuffer_HLSLI

struct InstanceData
{
	float4x4 world;
};

StructuredBuffer<InstanceData> gInstanceData : register(t9);

#endif
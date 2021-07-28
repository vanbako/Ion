#ifndef InstanceBuffer_HLSLI
#define InstanceBuffer_HLSLI

struct InstanceData
{
	float4x4 local;
};

StructuredBuffer<InstanceData> gInstanceData : register(t9);

#endif
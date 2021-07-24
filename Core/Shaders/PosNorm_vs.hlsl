#include "../../Core/Shaders/ObjectConstantBuffer.hlsli"

struct VSInput
{
	float3 position : POSITION;
	float3 normal : NORMAL;
};

struct VSOutput
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
};

VSOutput main(VSInput input)
{
	VSOutput output;
	output.position = mul(gWorldViewProj, float4(input.position, 1.0f));
	output.normal = normalize(mul(input.normal, (float3x3)gWorld));
	return output;
}

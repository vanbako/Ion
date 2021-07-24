#include "../../Core/Shaders/CanvasConstantBuffer.hlsli"

struct PSInput {
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
};

float4 main(PSInput input) : SV_TARGET
{
	float3 color_rgb = float3(1.0f, 1.0f, 1.0f);
	float diffuseStrength = dot(input.normal, -gLightDirection);
	diffuseStrength = diffuseStrength * 0.5 + 0.5;
	diffuseStrength = saturate(diffuseStrength);
	color_rgb = color_rgb * diffuseStrength;
	return float4(color_rgb , 1.0f);
}

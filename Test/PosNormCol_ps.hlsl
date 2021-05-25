cbuffer CanvasConstantBuffer : register(b0)
{
	float3 gLightDirection;
	float padding0;
	float4 padding1[15];
};

struct PSInput {
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float4 color : COLOR;
};

float4 main(PSInput input) : SV_TARGET
{
	float3 color_rgb = input.color.rgb;
	float color_a = input.color.a;
	float diffuseStrength = dot(input.normal, -gLightDirection);
	diffuseStrength = diffuseStrength * 0.5 + 0.5;
	diffuseStrength = saturate(diffuseStrength);
	color_rgb = color_rgb * diffuseStrength;
	return float4(color_rgb , color_a);
}

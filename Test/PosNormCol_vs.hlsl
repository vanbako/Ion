cbuffer ObjectConstantBuffer : register(b1)
{
	float4x4 gWorld;
	float4x4 gWorldViewProj;
	float4 padding3[8];
}

struct VSInput {
	float3 position : POSITION;
	float3 normal : NORMAL;
	float4 color : COLOR;
};

struct VSOutput {
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float4 color : COLOR;
};

VSOutput main(VSInput input)
{
	VSOutput output;
	output.position = mul(gWorldViewProj, float4(input.position, 1.0f));
	output.normal = normalize(mul(input.normal, (float3x3)gWorld));
	output.color = input.color;
	return output;
}

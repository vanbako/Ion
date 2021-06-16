cbuffer ObjectConstantBuffer : register(b1)
{
	float4x4 gWorld;
	float4x4 gWorldViewProj;
	float4 padding3[8];
}

struct VSInput
{
	float3 position: POSITION;
	float3 normal: NORMAL;
	//float3 Tangent: TANGENT;
	//float2 TexCoord: TEXCOORD;
};

struct VSOutput
{
	float4 position: SV_POSITION;
	float3 normal: NORMAL;
	//float3 Tangent: TANGENT;
	//float2 TexCoord: TEXCOORD;
};

VSOutput main(VSInput input)
{
	VSOutput output = (VSOutput)0;
	
	output.position = mul(gWorldViewProj, float4(input.position, 1.0f));
	output.normal = normalize(mul(input.normal, (float3x3)gWorld));
	//output.Tangent = mul(input.Tangent, (float3x3)gWorld);
	//output.TexCoord = input.TexCoord;
	
	return output;
}

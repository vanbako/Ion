cbuffer CanvasConstantBuffer : register(b0)
{
	float3 gLightDirection;
	float4 gColorDiffuse;
	float4 gColorAmbient;
	float gAmbientIntensity;
	float4 padding1[13];
};

Texture2D gTextureAlbedo : register(t0);
Texture2D gTextureNormal : register(t1);

SamplerState gTextureSampler : register(s0);

struct PSInput
{
	float4 position: SV_POSITION;
	float3 normal: NORMAL;
	float3 tangent: TANGENT;
	float2 texCoord: TEXCOORD;
};

float3 CalculateDiffuse(float3 normal, float2 texCoord)
{
	float3 diffuseColor = (gTextureAlbedo.Sample( gTextureSampler, texCoord ) * gColorDiffuse).xyz;
	float diffuseStrength = saturate(dot(normal, -gLightDirection));

	return diffuseColor * diffuseStrength;
}

float3 CalculateNormal(float3 tangent, float3 normal, float2 texCoord)
{
	float3 binormal = normalize(cross(tangent, normal));
	float3x3 localAxis = transpose( float3x3( tangent, binormal, normal )) ;
	float3 sampledNormal = gTextureNormal.Sample(gTextureSampler, texCoord).xyz;
	sampledNormal = (sampledNormal * 2.0f) - 1.0f;

	return normalize(mul(localAxis, sampledNormal));
}

float4 main(PSInput input) : SV_TARGET
{
	input.normal = normalize(input.normal);
	input.tangent = normalize(input.tangent);
	
	float3 newNormal = CalculateNormal(input.tangent, input.normal, input.texCoord);
	//float3 newNormal = input.normal;
	float3 diffColor = CalculateDiffuse(newNormal, input.texCoord);
	float3 ambientColor = gColorAmbient.xyz * gAmbientIntensity;
	float3 finalColor = diffColor + ambientColor;
	
	return float4(finalColor, 1.0f);

	//float3 color_rgb = float3( 1.0f, 1.0f, 1.0f);
	//float color_a = 1.0f;
	//float diffuseStrength = dot(input.normal, -gLightDirection);
	//diffuseStrength = diffuseStrength * 0.5 + 0.5;
	//diffuseStrength = saturate(diffuseStrength);
	//color_rgb = color_rgb * diffuseStrength;
	//return float4(color_rgb, color_a);
}

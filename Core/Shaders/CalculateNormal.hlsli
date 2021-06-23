#ifndef CalculateNormal_HLSLI
#define CalculateNormal_HLSLI

Texture2D gTextureNormal : register(t1);

float3 CalculateNormal(float3 tangent, float3 normal, float2 texCoord)
{
	float3 binormal = normalize(cross(tangent, normal));
	float3x3 localAxis = transpose( float3x3( tangent, binormal, normal )) ;
	float3 sampledNormal = gTextureNormal.Sample(gTextureSampler, texCoord).xyz;
	sampledNormal = (sampledNormal * 2.0f) - 1.0f;

	return normalize(mul(localAxis, sampledNormal));
}

#endif
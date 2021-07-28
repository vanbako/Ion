#ifndef CalculateAlbedo_HLSLI
#define CalculateAlbedo_HLSLI

Texture2D gTextureAlbedo : register(t0);

float3 CalculateAlbedo(float3 normal, float2 texCoord)
{
	float diffuseStrength = saturate(dot(normal, -gLightDirection));
	float3 diffuseColor = gTextureAlbedo.Sample(gTextureSampler, texCoord).xyz;
	return diffuseStrength * diffuseColor * gColorDiffuse.xyz;
}

#endif
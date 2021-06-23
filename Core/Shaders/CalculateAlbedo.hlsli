#ifndef CalculateAlbedo_HLSLI
#define CalculateAlbedo_HLSLI

Texture2D gTextureAlbedo : register(t0);

float3 CalculateAlbedo(float3 normal, float2 texCoord)
{
	float3 diffuseColor = (gTextureAlbedo.Sample( gTextureSampler, texCoord ) * gColorDiffuse).xyz;
	float diffuseStrength = saturate(dot(normal, -gLightDirection));

	return diffuseColor * diffuseStrength;
}

#endif
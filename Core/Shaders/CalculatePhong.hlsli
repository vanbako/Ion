#ifndef CalculatePhong_HLSLI
#define CalculatePhong_HLSLI

//float3 CalculatePhong(float3 viewDirection, float3 normal, float2 texCoord)
float3 CalculatePhong(float3 viewDirection, float3 normal)
{
	//float3 specularColor = gColorSpecular;
	float3 specularColor = float3(1.0, 1.0, 1.0);
	float3 reflectedLightDirection = reflect(gLightDirection, normal);
	float specularStrength = dot(-viewDirection, reflectedLightDirection);
	specularStrength = saturate(specularStrength);
	specularStrength = pow(specularStrength, gShininess);
	//specularStrength = pow(specularStrength, 20.0);

	//if (gUseTextureSpecularIntensity)
	//{
	//	specularStrength = specularStrength * gTextureSpecularIntensity.Sample(gTextureSampler, texCoord);
	//}

	return specularColor * specularStrength;
}

#endif
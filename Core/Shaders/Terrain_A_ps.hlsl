#include "../../Core/Shaders/CanvasConstantBuffer.hlsli"
#include "../../Core/Shaders/ObjectConstantBuffer.hlsli"

SamplerState gTextureSampler : register(s0);

#include "../../Core/Shaders/Terrain_PSInput.hlsli"

#include "../../Core/Shaders/CalculateAlbedo.hlsli"
#include "../../Core/Shaders/CalculatePhong.hlsli"

float4 main(PSInput input) : SV_TARGET
{
	input.normal = normalize(input.normal);
	
	float3 newNormal = input.normal;
	float3 viewDirection = normalize(input.worldposition.xyz - gViewPos.xyz);
	float3 albedoColor = CalculateAlbedo(newNormal, input.texcoord);
	float3 ambientColor = gColorAmbient.xyz * gAmbientIntensity;
	float3 finalColor;
	if (gShininess != 0.f)
	{
		//float3 phongColor = CalculatePhong(viewDirection, newNormal, input.TexCoord);
		float3 phongColor = CalculatePhong(viewDirection, newNormal);
		finalColor = saturate(albedoColor + ambientColor + phongColor);
	}
	else
		finalColor = saturate(albedoColor + ambientColor);

	return float4(finalColor, 1.0f);
}

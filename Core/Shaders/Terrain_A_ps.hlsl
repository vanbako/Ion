#include "../../Core/Shaders/CanvasConstantBuffer.hlsli"

SamplerState gTextureSampler : register(s0);

#include "../../Core/Shaders/Terrain_PSInput.hlsli"

#include "../../Core/Shaders/CalculateAlbedo.hlsli"

float4 main(PSInput input) : SV_TARGET
{
	input.normal = normalize(input.normal);
	
	float3 newNormal = input.normal;
	float3 albedoColor = CalculateAlbedo(newNormal, input.texcoord);
	float3 ambientColor = gColorAmbient.xyz * gAmbientIntensity;
	float3 finalColor = saturate(albedoColor + ambientColor);
	
	return float4(finalColor, 1.0f);
}

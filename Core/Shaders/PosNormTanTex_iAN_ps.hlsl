#include "../../Core/Shaders/CanvasConstantBuffer.hlsli"

SamplerState gTextureSampler : register(s0);

#include "../../Core/Shaders/PosNormTanTex_PSInput.hlsli"

#include "../../Core/Shaders/CalculateAlbedo.hlsli"
#include "../../Core/Shaders/CalculateNormal.hlsli"

float4 main(PSInput input) : SV_TARGET
{
	input.normal = normalize(input.normal);
	input.tangent = normalize(input.tangent);
	
	float3 newNormal = CalculateNormal(input.tangent, input.normal, input.texcoord);
	float3 albedoColor = CalculateAlbedo(newNormal, input.texcoord);
	float3 ambientColor = gColorAmbient.xyz * gAmbientIntensity;
	float3 finalColor = saturate(albedoColor + ambientColor);
	
	return float4(finalColor, 1.0f);
}

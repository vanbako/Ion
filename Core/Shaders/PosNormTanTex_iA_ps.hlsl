#include "../../Core/Shaders/CanvasConstantBuffer.hlsli"
#include "../../Core/Shaders/ObjectConstantBuffer.hlsli"

SamplerState gTextureSampler : register(s0);

#include "../../Core/Shaders/PosNormTanTex_PSInput.hlsli"

#include "../../Core/Shaders/CalculateAlbedo.hlsli"
#include "../../Core/Shaders/CalculatePhong.hlsli"

float4 main(PSInput input) : SV_TARGET
{
	input.normal = normalize(input.normal);
	input.tangent = normalize(input.tangent);
	
	float3 albedoColor = CalculateAlbedo(input.normal, input.texcoord);
	float3 viewDirection = normalize(input.worldposition.xyz - gViewPos.xyz);
	float3 ambientColor = gColorAmbient.xyz * gAmbientIntensity;
	float3 phongColor = float3(0.f, 0.f, 0.f);
	if (gShininess != 0.f)
		phongColor = CalculatePhong(viewDirection, input.normal);
	float4 finalColor = float4(saturate(albedoColor + ambientColor + phongColor), 1.f);

	return finalColor;
}

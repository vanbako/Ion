#include "../../Core/Shaders/CanvasConstantBuffer.hlsli"
#include "../../Core/Shaders/ObjectConstantBuffer.hlsli"

SamplerState gTextureSampler : register(s0);

#include "../../Core/Shaders/PosNormTanTex_PSInput.hlsli"

//#include "../../Core/Shaders/CalculateAlbedo.hlsli"
Texture2D gTextureAlbedo : register(t0);
#include "../../Core/Shaders/CalculatePhong.hlsli"

float4 main(PSInput input) : SV_TARGET
{
	input.normal = normalize(input.normal);
	input.tangent = normalize(input.tangent);
	

	float diffuseStrength = saturate(dot(input.normal, -gLightDirection));
	float3 diffuseColor = gTextureAlbedo.Sample(gTextureSampler, input.texcoord).xyz;
	float3 albedoColor = diffuseStrength * diffuseColor * gColorDiffuse.xyz;

	//float3 albedoColor = CalculateAlbedo(input.normal, input.texcoord);
	float3 viewDirection = normalize(input.worldposition.xyz - gViewPos.xyz);
	float3 ambientColor = gColorAmbient.xyz * gAmbientIntensity;
	float3 phongColor = float3(0.f, 0.f, 0.f);
	if (gShininess != 0.f)
		phongColor = CalculatePhong(viewDirection, input.normal);
	float4 finalColor = float4(saturate(albedoColor + ambientColor + phongColor), 1.f);

	return finalColor;
}

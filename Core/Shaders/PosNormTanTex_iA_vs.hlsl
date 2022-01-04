#include "../../Core/Shaders/CanvasConstantBuffer.hlsli"
#include "../../Core/Shaders/ObjectConstantBuffer.hlsli"
#include "../../Core/Shaders/InstanceBuffer.hlsli"
#include "../../Core/Shaders/PosNormTanTex_VSInput.hlsli"
#include "../../Core/Shaders/PosNormTanTex_PSInput.hlsli"

PSInput main(VSInput input, uint instanceID : SV_InstanceID)
{
	PSInput output = (PSInput)0;
	
	float4x4 world = transpose(mul(gWorld, gInstanceData[instanceID].local));
	float4x4 vp = transpose(gViewProj);
	float4x4 wvp = transpose(mul(world, vp));
	output.position = mul(wvp, float4(input.position, 1.0f));
	output.worldposition = mul(float4(input.position, 1.0f), world);
	output.normal = normalize(mul(input.normal, (float3x3)world));
	output.tangent = normalize(mul(input.tangent, (float3x3)world));
	output.texcoord = input.texcoord;
	
	return output;
}

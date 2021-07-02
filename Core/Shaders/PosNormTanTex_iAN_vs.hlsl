#include "../../Core/Shaders/CanvasConstantBuffer.hlsli"
#include "../../Core/Shaders/InstanceBuffer.hlsli"
#include "../../Core/Shaders/PosNormTanTex_VSInput.hlsli"
#include "../../Core/Shaders/PosNormTanTex_PSInput.hlsli"

PSInput main(VSInput input, uint instanceID : SV_InstanceID)
{
	PSInput output = (PSInput)0;
	
	float4x4 world = transpose(gInstanceData[instanceID].world);
	float4x4 vp = transpose(gViewProj);
	float4x4 wvp = transpose(mul(world, vp));
	output.position = mul(wvp, float4(input.position, 1.0f));
	output.normal = normalize(mul((float3x3)world, input.normal));
	output.tangent = mul(input.tangent, (float3x3)world);
	output.texcoord = input.texcoord;
	
	return output;
}

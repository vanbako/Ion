#include "../../Core/Shaders/ObjectConstantBuffer.hlsli"
#include "../../Core/Shaders/PosNormTanTex_VSInput.hlsli"
#include "../../Core/Shaders/PosNormTanTex_PSInput.hlsli"

PSInput main(VSInput input)
{
	PSInput output = (PSInput)0;
	
	output.position = mul(gWorldViewProj, float4(input.position, 1.0f));
	output.worldposition = mul(float4(input.position, 1.0f), gWorld);
	output.normal = normalize(mul((float3x3)gWorld, input.normal));
	output.tangent = mul((float3x3)gWorld, input.tangent);
	output.texcoord = input.texcoord;
	
	return output;
}

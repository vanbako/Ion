#include "../../Core/Shaders/ObjectConstantBuffer.hlsli"
#include "../../Core/Shaders/PosNormTanTex_VSInput.hlsli"
#include "../../Core/Shaders/PosNormTanTex_PSInput.hlsli"

PSInput main(VSInput input)
{
	PSInput output = (PSInput)0;
	
	output.position = mul(gWorldViewProj, float4(input.position, 1.0f));
	//output.position = mul(float4(input.position, 1.0f), gWorldViewProj);
	output.normal = normalize(mul((float3x3)gWorld, input.normal));
	//output.normal = normalize(mul(input.normal, (float3x3)gWorld));
	output.tangent = mul(input.tangent, (float3x3)gWorld);
	output.texcoord = input.texcoord;
	
	return output;
}

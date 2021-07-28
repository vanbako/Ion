#include "../../Core/Shaders/CanvasConstantBuffer.hlsli"
#include "../../Core/Shaders/ObjectConstantBuffer.hlsli"
#include "../../Core/Shaders/BonesBuffer.hlsli"
#include "../../Core/Shaders/InstanceBuffer.hlsli"
#include "../../Core/Shaders/PosNormTanTex_aVSInput.hlsli"
#include "../../Core/Shaders/PosNormTanTex_PSInput.hlsli"

PSInput main(VSInput input, uint instanceID : SV_InstanceID)
{
	PSInput output = (PSInput)0;
	
	float4 originalPosition = float4(input.position, 1.0f);
	float4 transformedPosition = 0;
	float3 transformedNormal = 0;
	float3 transformedTangent = 0;
	int index = 0;
	for (int i = 0; i < 4; ++i)
	{
		index = input.boneindices[i];
		if (index >= 0)
		{
			transformedPosition += input.boneweights[i] * mul(gBones[index], originalPosition);
			transformedNormal += input.boneweights[i] * mul((float3x3)gBones[index], input.normal);
			transformedTangent += input.boneweights[i] * mul((float3x3)gBones[index], input.tangent);
		}
	}
	transformedPosition.w = 1.0f;

	float4x4 world = transpose(mul(gWorld, gInstanceData[instanceID].local));
	float4x4 vp = transpose(gViewProj);
	float4x4 wvp = transpose(mul(world, vp));
	output.position = mul(wvp, transformedPosition);
	output.normal = normalize(mul(transformedNormal, (float3x3)world));
	output.tangent = normalize(mul(transformedTangent, (float3x3)world));
	output.texcoord = input.texcoord;
	
	return output;
}

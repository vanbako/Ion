#include "../../Core/Shaders/ObjectConstantBuffer.hlsli"
#include "../../Core/Shaders/BonesBuffer.hlsli"
#include "../../Core/Shaders/PosNormTanTex_aVSInput.hlsli"
#include "../../Core/Shaders/PosNormTanTex_PSInput.hlsli"

PSInput main(VSInput input)
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
			transformedNormal += input.boneweights[i] * mul((float3x3)gBones[index], input.normal );
			transformedTangent += input.boneweights[i] * mul((float3x3)gBones[index], input.tangent);
		}
	}
	transformedPosition.w = 1.0f;

	output.position = mul(gWorldViewProj, transformedPosition);
	output.worldposition = mul(transformedPosition, gWorld);
	output.normal = normalize(mul((float3x3)gWorld, transformedNormal));
	output.tangent = mul((float3x3)gWorld, transformedTangent);
	output.texcoord = input.texcoord;
	
	return output;
}

#pragma once

enum class InputSemantic {
	Position = 0, // float4
	Position0,
	Position1,
	Position2,
	Normal, // float4
	Normal0,
	Normal1,
	Normal2,
	Tangent, // float4
	Tangent0,
	Tangent1,
	Tangent2,
	Binormal, // float4
	Binormal0,
	Binormal1,
	Binormal2,
	TexCoord, // float4
	TexCoord0,
	TexCoord1,
	TexCoord2,
	Color, // float4
	Color0,
	Color1,
	Color2,
	BlendIndices, // uint
	BlendIndices0,
	BlendIndices1,
	BlendIndices2,
	BlendWeight, // float
	BlendWeight0,
	BlendWeight1,
	BlendWeight2,
	Count
};
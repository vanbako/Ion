#pragma once

namespace Ion
{
	namespace Core
	{
		enum class InputSemantic {
			Position = 0, // float4
			Normal, // float4
			Tangent, // float4
			Binormal, // float4
			TexCoord, // float4
			Color, // float4
			BlendIndices, // uint
			BlendWeight, // float
			World,
			Count
		};
	}
}

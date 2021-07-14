#pragma once

namespace Ion
{
	namespace Core
	{
		enum class TextureType
		{
			Albedo = 0,
			Normal = 1,
			Roughness = 2,
			Metalness = 3,
			Specular = 4,
			Height = 5,
			Opacity = 6,
			AmbientOcclusion = 7,
			Refraction = 8,
			SelfIllumination = 9,
			Count
		};
	}
}

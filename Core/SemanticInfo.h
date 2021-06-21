#pragma once
#include "../Core/InputSemantic.h"

namespace Ion
{
	namespace Core
	{
		struct SemanticInfo
		{
			InputSemantic inputSemantic;
			DXGI_FORMAT format;
			UINT offset;
		};
	}
}
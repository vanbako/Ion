#pragma once
#include "InputSemantic.h"

namespace Ion
{
	namespace Core
	{
		struct SemanticInfo
		{
			Core::InputSemantic inputSemantic;
			DXGI_FORMAT format;
			UINT offset;
			D3D12_INPUT_CLASSIFICATION inputClassification;
		};
	}
}
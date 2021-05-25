#pragma once

namespace Ion
{
	namespace Core
	{
		struct CanvasConstantBuffer final
		{
			DirectX::XMFLOAT3 mLightDirection;
			float mPadding[61]; // Total 256 bytes
		};
	}
}

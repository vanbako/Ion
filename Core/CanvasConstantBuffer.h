#pragma once

namespace Ion
{
	namespace Core
	{
		struct CanvasConstantBuffer final
		{
			DirectX::XMFLOAT3 mLightDirection;
			DirectX::XMFLOAT4 mColorDiffuse;
			DirectX::XMFLOAT4 mColorAmbient;
			float mAmbientIntensity;
			float mPadding[52]; // Total 256 bytes
		};
	}
}

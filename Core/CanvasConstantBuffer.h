#pragma once

namespace Ion
{
	namespace Core
	{
		struct CanvasConstantBuffer final
		{
			DirectX::XMFLOAT3 mLightDirection;
			float mPadding0;
			DirectX::XMFLOAT4 mColorDiffuse;
			DirectX::XMFLOAT4 mColorAmbient;
			float mAmbientIntensity;
			float mPadding1;
			float mPadding2;
			float mPadding3;
			DirectX::XMFLOAT4 mPadding4[12]; // Total 256 bytes
		};
	}
}

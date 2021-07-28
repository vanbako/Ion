#pragma once

namespace Ion
{
	namespace Core
	{
		struct CanvasConstantBuffer
		{
			DirectX::XMFLOAT4X4 mView;
			DirectX::XMFLOAT4X4 mViewProj;
			DirectX::XMFLOAT3 mLightDirection;
			float mAmbientIntensity;
			DirectX::XMFLOAT4 mColorDiffuse;
			DirectX::XMFLOAT4 mColorAmbient;
			DirectX::XMFLOAT4 mPadding4[5]; // Total 256 bytes
		};
	}
}

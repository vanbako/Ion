#pragma once

namespace Ion
{
	namespace Core
	{
		struct MeshVCConstantBuffer
		{
			DirectX::XMFLOAT4X4 mWorld;
			DirectX::XMFLOAT4X4 mWorldViewProj;
			float mShininess;
			DirectX::XMFLOAT3 mPadding0;
			DirectX::XMFLOAT4 mPadding1[7];
			// Total 256 bytes
		};
	}
}

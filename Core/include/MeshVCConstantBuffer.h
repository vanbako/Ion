#pragma once

namespace Ion
{
	namespace Core
	{
		struct MeshVCConstantBuffer
		{
			DirectX::XMFLOAT4X4 mWorld;
			DirectX::XMFLOAT4X4 mWorldViewProj;
			DirectX::XMFLOAT4 mPadding[8]; // Total 256 bytes
		};
	}
}

#pragma once

namespace Ion
{
	namespace Core
	{
		struct MeshVCConstantBuffer final
		{
			DirectX::XMFLOAT4X4 mWorld;
			DirectX::XMFLOAT4X4 mWorldViewProj;
			float mPadding[32]; // Total 256 bytes
		};
	}
}

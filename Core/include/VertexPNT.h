#pragma once

// Vertex Position Normal UV

namespace Ion
{
	namespace Core
	{
		struct VertexPNT
		{
			DirectX::XMFLOAT3 mPosition;
			DirectX::XMFLOAT3 mNormal;
			DirectX::XMFLOAT2 mUV;
		};
	}
}

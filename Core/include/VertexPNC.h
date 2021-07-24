#pragma once

// Vertex Position Normal Color

namespace Ion
{
	namespace Core
	{
		struct VertexPNC
		{
			DirectX::XMFLOAT3 mPosition;
			DirectX::XMFLOAT3 mNormal;
			DirectX::XMFLOAT4 mColor;
		};
	}
}

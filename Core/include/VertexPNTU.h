#pragma once

// Vertex Position Normal Tangent UV

namespace Ion
{
	namespace Core
	{
		struct VertexPNTU
		{
			DirectX::XMFLOAT3 mPosition;
			DirectX::XMFLOAT3 mNormal;
			DirectX::XMFLOAT3 mTangent;
			DirectX::XMFLOAT2 mUV;
		};
	}
}

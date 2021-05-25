#pragma once

// Vertex Position Normal Color

namespace Ion
{
	namespace Core
	{
		struct VertexPNC
		{
		//public:
		//	explicit VertexPNC();
		//	explicit VertexPNC(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& normal, const DirectX::XMFLOAT4& color);
		//	virtual ~VertexPNC() = default;
		//	VertexPNC(const VertexPNC& other) = default;
		//	VertexPNC(VertexPNC&& other) noexcept = default;
		//	VertexPNC& operator=(const VertexPNC& other) = default;
		//	VertexPNC& operator=(VertexPNC&& other) noexcept = default;
		//private:
			DirectX::XMFLOAT3 mPosition;
			DirectX::XMFLOAT3 mNormal;
			DirectX::XMFLOAT4 mColor;
		};
	}
}

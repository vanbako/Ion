#pragma once

namespace Ion
{
	namespace Core
	{
		struct Transform final
		{
			DirectX::XMFLOAT4 mPosition;
			DirectX::XMFLOAT4 mScale;
			DirectX::XMFLOAT4 mRotation;
		};
	}
}

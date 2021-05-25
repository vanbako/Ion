#pragma once

namespace Ion
{
	namespace Core
	{
		class Helpers final
		{
		public:
			explicit Helpers() = delete;
			virtual ~Helpers() = delete;
			Helpers(const Helpers& other) = delete;
			Helpers(Helpers&& other) noexcept = delete;
			Helpers& operator=(const Helpers& other) = delete;
			Helpers& operator=(Helpers&& other) noexcept = delete;

			static UINT CalcConstantBufferByteSize(UINT byteSize);
		};
	}
}

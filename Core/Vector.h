#pragma once

namespace Ion
{
	namespace Core
	{
		template<class T>
		class Vector final
		{
		public:
			explicit Vector()
				: Vector(T{}, T{}, T{})
			{}
			explicit Vector(T a, T b, T c)
				: mA{ a }
				, mB{ b }
				, mC{ c }
			{}
			~Vector() = default;
			Vector(const Vector& other) = default;
			Vector(Vector&& other) noexcept = default;
			Vector& operator=(const Vector& other) = default;
			Vector& operator=(Vector&& other) noexcept = default;
		private:
			T mA;
			T mB;
			T mC;
		};
	}
}

#pragma once

// Triangle

namespace Ion
{
	namespace Core
	{
		template<class T>
		class Triangle
		{
		public:
			Triangle()
				: Triangle(T{}, T{}, T{})
			{}
			explicit Triangle(T a, T b, T c)
				: mA{ a }
				, mB{ b }
				, mC{ c }
			{}
			virtual ~Triangle() = default;
			Triangle(const Triangle& other) = default;
			Triangle(Triangle&& other) noexcept = default;
			Triangle& operator=(const Triangle& other) = default;
			Triangle& operator=(Triangle&& other) noexcept = default;
		private:
			T mA;
			T mB;
			T mC;
		};
	}
}

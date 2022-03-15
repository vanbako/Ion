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

			T& GetA() { return mA; }
			T& GetB() { return mB; }
			T& GetC() { return mC; }
			void SetA(T a) { mA = a; }
			void SetB(T b) { mB = b; }
			void SetC(T c) { mC = c; }
		private:
			T mA;
			T mB;
			T mC;
		};
		template<class T>
		bool operator==(Vector<T>& lhs, Vector<T>& rhs)
		{
			return ((lhs.GetA() == rhs.GetA()) && (lhs.GetB() == rhs.GetB()) && (lhs.GetC() == rhs.GetC()));
		}
	}
}

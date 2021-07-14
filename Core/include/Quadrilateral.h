#pragma once

// Quadrilateral

namespace Ion
{
	namespace Core
	{
		template<class T>
		class Quadrilateral
		{
		public:
			explicit Quadrilateral()
				: Quadrilateral(T{}, T{}, T{}, T{})
			{}
			explicit Quadrilateral(T a, T b, T c, T d)
				: mA{ a }
				, mB{ b }
				, mC{ c }
				, mD{ d }
			{}
			virtual ~Quadrilateral() = default;
			Quadrilateral(const Quadrilateral& other) = default;
			Quadrilateral(Quadrilateral&& other) noexcept = default;
			Quadrilateral& operator=(const Quadrilateral& other) = default;
			Quadrilateral& operator=(Quadrilateral&& other) noexcept = default;

			const T& GetA() const { return mA; };
			const T& GetB() const { return mB; };
			const T& GetC() const { return mC; };
			const T& GetD() const { return mD; };
		private:
			T mA;
			T mB;
			T mC;
			T mD;
		};
	}
}

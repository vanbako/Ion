#pragma once
#include "Vector.h"

namespace Ion
{
	namespace Core
	{

		class Cube
		{
		public:
			explicit Cube(const Core::Vector<long long>& location);
			explicit Cube();
			virtual ~Cube() = default;
			Cube(const Cube& other) = delete;
			Cube(Cube&& other) noexcept = delete;
			Cube& operator=(const Cube& other) = delete;
			Cube& operator=(Cube&& other) noexcept = delete;

			Core::Vector<long long>& GetLocation();
			void SetLocation(const Core::Vector<long long>& loc);
		protected:
			Core::Vector<long long> mLocation;
		};
	}
}

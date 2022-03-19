#include "pch.h"
#include "Cube.h"

using namespace Ion;

Core::Cube::Cube(const Core::Vector<long long>& location)
	: mLocation{ location }
{
}

Core::Cube::Cube()
	: mLocation{}
{
}

Core::Vector<long long>& Core::Cube::GetLocation()
{
	return mLocation;
}

void Core::Cube::SetLocation(const Core::Vector<long long>& loc)
{
	mLocation = loc;
}

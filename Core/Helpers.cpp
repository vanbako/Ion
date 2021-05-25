#include "../Core/pch.h"
#include "../Core/Helpers.h"

using namespace Ion::Core;

UINT Helpers::CalcConstantBufferByteSize(UINT byteSize)
{
	return ((byteSize + 255) & ~255);
}

#include "pch.h"
#include "PxIonErrorCallback.h"
#include <iostream>

using namespace physx;

void PxIonErrorCallback::reportError(PxErrorCode::Enum code, const char* message, const char* file, int line)
{
	//(code);
	//(message);
	(file);
	(line);
	std::cout << "Error code: " << code << ", msg: " << message << std::endl;
	throw std::runtime_error(message);
}

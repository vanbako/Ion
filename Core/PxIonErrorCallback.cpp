#include "pch.h"
#include "PxIonErrorCallback.h"

void physx::PxIonErrorCallback::reportError(PxErrorCode::Enum code, const char* message, const char* file, int line)
{
	//(code);
	//(message);
	(file);
	(line);
	std::cout << "Error code: " << code << ", msg: " << message << std::endl;
	throw std::runtime_error(message);
}

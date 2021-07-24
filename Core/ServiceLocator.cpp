#include "pch.h"
#include "ServiceLocator.h"

using namespace Ion;

Core::ServiceLocator::ServiceLocator()
	: mpLoggerService{ nullptr }
	, mNullLoggerService{}
	, mpAudioService{ nullptr }
	, mNullAudioService{}
{
	mpAudioService = &mNullAudioService;
	mpLoggerService = &mNullLoggerService;
}

Core::Audio* Core::ServiceLocator::GetAudio()
{
	return mpAudioService;
}

void Core::ServiceLocator::RegisterAudioService(Core::Audio* pAudioService)
{
	if (pAudioService == nullptr)
		mpAudioService = &mNullAudioService;
	else
		mpAudioService = pAudioService;
}

Core::Logger* Core::ServiceLocator::GetLogger()
{
	return mpLoggerService;
}

void Core::ServiceLocator::RegisterLoggerService(Core::Logger* pLoggerService)
{
	if (pLoggerService == nullptr)
		mpLoggerService = &mNullLoggerService;
	else
		mpLoggerService = pLoggerService;
}

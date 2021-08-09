#include "pch.h"
#include "ServiceLocator.h"

using namespace Ion;

Core::ServiceLocator::ServiceLocator()
	: mpAudioService{ nullptr }
	, mNullAudioService{}
#ifdef ION_LOGGER
	, mpLoggerService{ nullptr }
	, mNullLoggerService{}
#endif
{
	mpAudioService = &mNullAudioService;
#ifdef ION_LOGGER
	mpLoggerService = &mNullLoggerService;
#endif
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

#ifdef ION_LOGGER

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

#endif
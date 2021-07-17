#include "pch.h"
#include "RegularAudio.h"

using namespace Ion::Core;

RegularAudio::RegularAudio()
	: mpFmodSystem{ nullptr }
	, mpChannelGroup{ nullptr }
	, mpSounds{}
{
	if (FMOD::System_Create(&mpFmodSystem) != FMOD_OK)
	{
		mpFmodSystem = nullptr;
		return;
	}
	int driverCnt;
	if (mpFmodSystem->getNumDrivers(&driverCnt) != FMOD_OK)
	{
		mpFmodSystem->release();
		mpFmodSystem = nullptr;
		return;
	}
	if (driverCnt == 0)
	{
		mpFmodSystem->release();
		mpFmodSystem = nullptr;
		return;
	}
	if (mpFmodSystem->init(32, FMOD_INIT_NORMAL, nullptr) != FMOD_OK)
	{
		mpFmodSystem->release();
		mpFmodSystem = nullptr;
		return;
	}
	if (mpFmodSystem->createChannelGroup("CG", &mpChannelGroup) != FMOD_OK)
	{
		mpFmodSystem->release();
		mpFmodSystem = nullptr;
		return;
	}
}

RegularAudio::~RegularAudio()
{
	if (mpFmodSystem != nullptr)
		mpFmodSystem->release();
}

int RegularAudio::AddSound(const std::string& filename, bool loop)
{
	FMOD_MODE mode{ FMOD_DEFAULT };
	if (loop)
		mode |= FMOD_LOOP_NORMAL;
	FMOD::Sound* pSound{ nullptr };
	int soundId{ int(mpSounds.size()) };
	if (mpFmodSystem->createSound(filename.c_str(), mode, nullptr, &pSound) == FMOD_OK)
		mpSounds.push_back(pSound);
	return soundId;
}

void RegularAudio::PlaySound(int soundId)
{
	if (mpFmodSystem != nullptr && soundId < int(mpSounds.size()))
	{
		FMOD::Channel* pChannel{ nullptr };
		mpFmodSystem->playSound(mpSounds[soundId], mpChannelGroup, false, &pChannel);
	}
}

void RegularAudio::StopSound(int soundId)
{
	if (mpFmodSystem != nullptr && soundId < int(mpSounds.size()))
		mpChannelGroup->stop();
}

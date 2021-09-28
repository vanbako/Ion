#pragma once
#include "../../Core/include/Audio.h"
//#include "Audio.h"
#include "fmod.hpp" //Precompiled Header
#pragma warning(push)
#pragma warning(disable: 4505) //Unreferenced local function removed (FMOD_ErrorString)
#include "fmod_errors.h"
#pragma warning(pop)

class FmodAudio final
	: public Ion::Core::Audio
{
public:
	explicit FmodAudio();
	~FmodAudio();
	FmodAudio(const FmodAudio&) = delete;
	FmodAudio(FmodAudio&&) = delete;
	FmodAudio& operator=(const FmodAudio&) = delete;
	FmodAudio& operator=(FmodAudio&&) = delete;

	virtual int AddSound(const std::string& filename, bool loop) override;
	virtual void PlaySound(int soundId) override;
	virtual void StopSound(int soundId) override;
private:
	FMOD::System* mpFmodSystem;
	FMOD::ChannelGroup* mpChannelGroup;
	std::vector<FMOD::Sound*> mpSounds;
};

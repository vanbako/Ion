#pragma once
#include "Audio.h"
#include "fmod.hpp" //Precompiled Header
#pragma warning(push)
#pragma warning(disable: 4505) //Unreferenced local function removed (FMOD_ErrorString)
#include "fmod_errors.h"
#pragma warning(pop)

namespace Ion
{
	namespace Core
	{
		class RegularAudio final
			: public Audio
		{
		public:
			explicit RegularAudio();
			~RegularAudio();
			RegularAudio(const RegularAudio&) = delete;
			RegularAudio(RegularAudio&&) = delete;
			RegularAudio& operator=(const RegularAudio&) = delete;
			RegularAudio& operator=(RegularAudio&&) = delete;

			virtual int AddSound(const std::string& filename, bool loop) override;
			virtual void PlaySound(int soundId) override;
			virtual void StopSound(int soundId) override;
		private:
			FMOD::System* mpFmodSystem;
			FMOD::ChannelGroup* mpChannelGroup;
			std::vector<FMOD::Sound*> mpSounds;
		};
	}
}

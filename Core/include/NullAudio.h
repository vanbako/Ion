#pragma once
#include "Audio.h"

namespace Ion
{
	namespace Core
	{
		class NullAudio final
			: public Core::Audio
		{
		public:
			explicit NullAudio() = default;
			~NullAudio() = default;
			NullAudio(const NullAudio&) = delete;
			NullAudio(NullAudio&&) = delete;
			NullAudio& operator=(const NullAudio&) = delete;
			NullAudio& operator=(const NullAudio&&) = delete;

			virtual int AddSound(const std::string& filename, bool loop) override;
			virtual void PlaySound(int soundId) override { (soundId); };
			virtual void StopSound(int soundId) override { (soundId); };
		};
	}
}

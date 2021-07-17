#pragma once

namespace Ion
{
	namespace Core
	{
		class Audio
		{
		public:
			explicit Audio() = default;
			virtual ~Audio() = default;
			Audio(const Audio&) = delete;
			Audio(Audio&&) = delete;
			Audio& operator=(const Audio&) = delete;
			Audio& operator=(Audio&&) = delete;

			virtual int AddSound(const std::string& filename, bool loop) = 0;
			virtual void PlaySound(int soundId) = 0;
			virtual void StopSound(int soundId) = 0;
		};
	}
}

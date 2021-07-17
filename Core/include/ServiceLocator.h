#pragma once
#include "NullLogger.h"
#include "NullAudio.h"

namespace Ion
{
	namespace Core
	{
		class Audio;
		class NullAudio;
		class Logger;
		class NullLogger;

		class ServiceLocator final
		{
		public:
			explicit ServiceLocator();
			~ServiceLocator() = default;
			ServiceLocator(const ServiceLocator&) = delete;
			ServiceLocator(ServiceLocator&&) = delete;
			ServiceLocator& operator=(const ServiceLocator&) = delete;
			ServiceLocator& operator=(ServiceLocator&&) = delete;

			Audio* GetAudio();
			void RegisterAudioService(Audio* pAudioService);
			Logger* GetLogger();
			void RegisterLoggerService(Logger* pLoggerService);
		private:
			Audio* mpAudioService;
			NullAudio mNullAudioService;
			Logger* mpLoggerService;
			NullLogger mNullLoggerService;
		};
	}
}

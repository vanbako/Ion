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

			Core::Audio* GetAudio();
			void RegisterAudioService(Core::Audio* pAudioService);
			Core::Logger* GetLogger();
			void RegisterLoggerService(Core::Logger* pLoggerService);
		private:
			Core::Audio* mpAudioService;
			Core::NullAudio mNullAudioService;
			Core::Logger* mpLoggerService;
			Core::NullLogger mNullLoggerService;
		};
	}
}

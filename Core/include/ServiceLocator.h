#pragma once
#include "NullAudio.h"
#ifdef ION_LOGGER
#include "NullLogger.h"
#endif
#ifdef ION_STATS
#include "NullStatsWriter.h"
#endif

namespace Ion
{
	namespace Core
	{
		class Audio;
#ifdef ION_LOGGER
		class Logger;
#endif
#ifdef ION_STATS
		class StatsWriter;
#endif

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
#ifdef ION_LOGGER
			Core::Logger* GetLogger();
			void RegisterLoggerService(Core::Logger* pLoggerService);
#endif
#ifdef ION_STATS
			Core::StatsWriter* GetStatsWriter();
			void RegisterStatsWriterService(Core::StatsWriter* pStatsWriterService);
#endif
		private:
			Core::Audio* mpAudioService;
			Core::NullAudio mNullAudioService;
#ifdef ION_LOGGER
			Core::Logger* mpLoggerService;
			Core::NullLogger mNullLoggerService;
#endif
#ifdef ION_STATS
			Core::StatsWriter* mpStatsWriterService;
			Core::NullStatsWriter mNullStatsWriterService;
#endif
		};
	}
}

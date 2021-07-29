#pragma once

namespace Ion
{
	namespace Core
	{
		class ReceiverMC;

		class Command
		{
		public:
			virtual ~Command() = default;
			Command(const Command& other) = default;
			Command(Command&& other) noexcept = default;
			Command& operator=(const Command& other) = default;
			Command& operator=(Command&& other) noexcept = default;

			virtual Core::Command* Duplicate() = 0;
			virtual void Execute() = 0;
			virtual void SetValue(long long value);
		protected:
			explicit Command();

			long long int mValue;
		};
	}
}

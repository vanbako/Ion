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

			virtual Command* Duplicate() = 0;
			virtual void Execute() = 0;
		protected:
			explicit Command(ReceiverMC* pReceiver);

			ReceiverMC* mpReceiver;
		};
	}
}

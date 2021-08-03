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
			virtual void SetValue(int value);
			virtual void SetValue(long long value);
			virtual void SetValue(float value);
			virtual void SetValue(bool value);
		protected:
			explicit Command();

			bool mBoolValue;
			int mIntValue;
			long long mLongLongValue;
			float mFloatValue;
		};
	}
}

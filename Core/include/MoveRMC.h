#pragma once
#include "ReceiverMC.h"
#include "KeyboardState.h"

// Receiver Model Component

namespace Ion
{
	namespace Core
	{
		class Command;

		enum class MoveType
		{
			Forward,
			Back,
			Left,
			Right,
			Up,
			Down,
			RotateLeft,
			RotateRight,
			CursorRotateLeftRight,
			CursorUpDown,
			Count
		};

		class MoveRMC
			: public Core::ReceiverMC
		{
		public:
			explicit MoveRMC(bool isActive, Core::Object* pObject);
			virtual ~MoveRMC() = default;
			MoveRMC(const MoveRMC& other) = default;
			MoveRMC(MoveRMC&& other) noexcept = default;
			MoveRMC& operator=(const MoveRMC& other) = default;
			MoveRMC& operator=(MoveRMC&& other) noexcept = default;

			virtual void Switch() override;
			virtual const std::vector<std::pair<std::string, Core::Command*>>& GetCommands() const = 0;
			virtual const std::string& GetName() const = 0;

			virtual void MoveForward(long long value);
			virtual void MoveBack(long long value);
			virtual void MoveLeft(long long value);
			virtual void MoveRight(long long value);
			virtual void MoveUp(long long value);
			virtual void MoveDown(long long value);
			virtual void RotateLeft(long long value);
			virtual void RotateRight(long long value);
			virtual void CursorRotateLeftRight(int value);
			virtual void CursorUpDown(int value);
		protected:
			// My best guess is that boolean operations are thread-safe
			// Implement std::atomic if needed
			bool mMoveBool[size_t(Core::MoveType::Count)];
			static const std::chrono::milliseconds mMoveActionsMutexDuration;
			std::timed_mutex mMoveActionsMutex;
			std::vector<std::pair<Core::MoveType, int>> mMoveActions;

			virtual void SetMoveState(Core::MoveType moveType, long long value);
			virtual void AddMoveAction(Core::MoveType moveType, int value);
		};
	}
}

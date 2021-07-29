#pragma once
#include "ReceiverMC.h"

// Receiver Model Component

namespace Ion
{
	namespace Core
	{
		class Command;

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

			virtual void Update(float) override = 0;
			virtual void Switch() override = 0;
			virtual const std::vector<std::pair<std::string, Core::Command*>>& GetCommands() const = 0;
			virtual const std::string& GetName() const = 0;

			virtual void MoveForward(long long value) { (value); };
			virtual void MoveBack(long long value) { (value); };
			virtual void MoveLeft(long long value) { (value); };
			virtual void MoveRight(long long value) { (value); };
			virtual void MoveUp(long long value) { (value); };
			virtual void MoveDown(long long value) { (value); };
			virtual void RotateLeft(long long value) { (value); };
			virtual void RotateRight(long long value) { (value); };
			virtual void NextObject(long long value) { (value); };
			virtual void PrevObject(long long value) { (value); };
			virtual void ShowControls(long long value) { (value); };
		};
	}
}

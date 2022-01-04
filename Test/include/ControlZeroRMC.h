#pragma once
#include "../Core/include/ReceiverMC.h"

// Control(Scene)Zero Receiver Model Component

namespace Ion
{
	namespace Core
	{
		class Canvas;
		class TransformMC;

		class ControlZeroRMC final
			: public Core::ReceiverMC
		{
		public:
			explicit ControlZeroRMC(bool isActive, Core::Object* pObject);
			virtual ~ControlZeroRMC();
			ControlZeroRMC(const ControlZeroRMC& other) = default;
			ControlZeroRMC(ControlZeroRMC&& other) noexcept = default;
			ControlZeroRMC& operator=(const ControlZeroRMC& other) = default;
			ControlZeroRMC& operator=(ControlZeroRMC&& other) noexcept = default;

			virtual void Initialize() override;
			virtual void Update(float delta) override;
			virtual void Switch() override;
			virtual const std::vector<std::pair<std::string, Core::Command*>>& GetCommands() const override;
			virtual const std::string& GetName() const override;

			virtual void ToggleSceneOne(long long value);
			virtual void ToggleSceneTwo(long long value);
		private:
			static const std::string mName;
			std::vector<std::pair<std::string, Core::Command*>> mCommands;

			// My best guess is that boolean operations are thread-safe
			// Implement std::atomic if needed
			bool
				mToggleSceneOne[2],
				mToggleSceneTwo[2];
		};
	}
}

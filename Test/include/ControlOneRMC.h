#pragma once
#include "../Core/include/ReceiverMC.h"

// Control(Scene)One Receiver Model Component

namespace Ion
{
	namespace Core
	{
		class Canvas;
		class TransformMC;

		class ControlOneRMC final
			: public Core::ReceiverMC
		{
		public:
			explicit ControlOneRMC(bool isActive, Core::Object* pObject);
			virtual ~ControlOneRMC();
			ControlOneRMC(const ControlOneRMC& other) = default;
			ControlOneRMC(ControlOneRMC&& other) noexcept = default;
			ControlOneRMC& operator=(const ControlOneRMC& other) = default;
			ControlOneRMC& operator=(ControlOneRMC&& other) noexcept = default;

			virtual void Initialize() override;
			virtual void Update(float delta) override;
			virtual void Switch() override;
			virtual const std::vector<std::pair<std::string, Core::Command*>>& GetCommands() const override;
			virtual const std::string& GetName() const override;

			void AddObject(Core::Object* pObject);

			virtual void NextObject(long long value);
			virtual void PrevObject(long long value);
			virtual void ShowControls(long long value);
		private:
			static const std::string mName;
			std::vector<std::pair<std::string, Core::Command*>> mCommands;
			std::vector<Core::Object*> mpObjects;
			std::size_t mCurrObject;

			// My best guess is that boolean operations are thread-safe
			// Implement std::atomic if needed
			bool
				mNextObject[2],
				mPrevObject[2],
				mShowControls[2];
		};
	}
}

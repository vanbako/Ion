#pragma once
#include "ReceiverMC.h"

// Camera Model Component

namespace Ion
{
	namespace Core
	{
		class Canvas;
		class TransformMC;

		class ControlRMC final
			: public Core::ReceiverMC
		{
		public:
			explicit ControlRMC(bool isActive, Core::Object* pObject);
			virtual ~ControlRMC();
			ControlRMC(const ControlRMC& other) = default;
			ControlRMC(ControlRMC&& other) noexcept = default;
			ControlRMC& operator=(const ControlRMC& other) = default;
			ControlRMC& operator=(ControlRMC&& other) noexcept = default;

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
			size_t mCurrObject;

			// My best guess is that boolean operations are thread-safe
			// Implement std::atomic if needed
			bool
				mNextObject[2],
				mPrevObject[2],
				mShowControls[2];
		};
	}
}

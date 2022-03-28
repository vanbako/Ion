#pragma once
#include "SceneThread.h"

// IC = IonCore
#define IC_CURSOR_LEFTRIGHT -2
#define IC_CURSOR_UPDOWN -1

namespace Ion
{
	namespace Core
	{
		class Command;
		class InputCC;

		class ControllerST final
			: public Core::SceneThread
		{
		public:
			explicit ControllerST(Core::Scene* pScene, std::chrono::milliseconds updateTime);
			virtual ~ControllerST() = default;
			ControllerST(const ControllerST& other) = delete;
			ControllerST(ControllerST&& other) noexcept = delete;
			ControllerST& operator=(const ControllerST& other) = delete;
			ControllerST& operator=(ControllerST&& other) noexcept = delete;

			void Register(Core::InputCC* pInvoker, const std::string& name, const std::vector<std::pair<std::string, Core::Command*>>& commands);
		protected:
			virtual void Inner(float delta) override;
		private:
			static const std::unordered_map<std::string, int> mKeyNames;
			BYTE mKeyboard[512];
			PBYTE
				mpCurrKeyboard,
				mpLastKeyboard;

			void Input();

			std::unordered_map<int, std::vector<std::pair<Core::Command*, Core::InputCC*>>> mKeyboardCommands;
		};
	}
}

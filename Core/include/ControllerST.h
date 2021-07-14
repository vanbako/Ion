#pragma once
#include "SceneThread.h"

namespace Ion
{
	namespace Core
	{
		class Command;
		class InputCC;

		class ControllerST final
			: public SceneThread
		{
		public:
			explicit ControllerST(Scene* pScene, std::chrono::microseconds updateTime);
			virtual ~ControllerST() = default;
			ControllerST(const ControllerST& other) = delete;
			ControllerST(ControllerST&& other) noexcept = delete;
			ControllerST& operator=(const ControllerST& other) = delete;
			ControllerST& operator=(ControllerST&& other) noexcept = delete;

			void Register(InputCC* pInvoker, const std::string& name, const std::vector<std::pair<std::string, Command*>>& commands);
		protected:
			virtual void Inner(float delta) override;
		private:
			static const std::map<std::string, int> mKeyNames;

			void Input();

			std::map<int, std::vector<std::pair<Command*, InputCC*>>> mCommands;
		};
	}
}

#pragma once
#include "SceneThread.h"

namespace Ion
{
	namespace Core
	{
		class ModelST final
			: public Core::SceneThread
		{
		public:
			explicit ModelST(Core::Scene* pScene, std::chrono::milliseconds updateTime);
			virtual ~ModelST() = default;
			ModelST(const ModelST& other) = delete;
			ModelST(ModelST&& other) noexcept = delete;
			ModelST& operator=(const ModelST& other) = delete;
			ModelST& operator=(ModelST&& other) noexcept = delete;
		protected:
			virtual void Inner(float delta) override;
		};
	}
}

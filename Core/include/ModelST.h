#pragma once
#include "SceneThread.h"

namespace Ion
{
	namespace Core
	{
		class ModelST final
			: public SceneThread
		{
		public:
			explicit ModelST(Scene* pScene, std::chrono::microseconds updateTime);
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

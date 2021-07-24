#pragma once
#include "SceneThread.h"

namespace Ion
{
	namespace Core
	{
		class ViewST final
			: public SceneThread
		{
		public:
			explicit ViewST(Core::Scene* pScene, std::chrono::microseconds updateTime);
			virtual ~ViewST() = default;
			ViewST(const ViewST& other) = delete;
			ViewST(ViewST&& other) noexcept = delete;
			ViewST& operator=(const ViewST& other) = delete;
			ViewST& operator=(ViewST&& other) noexcept = delete;
		protected:
			virtual void Inner(float delta) override;
		};
	}
}

#pragma once

#include "../Core/AnimationKey.h"

namespace Ion
{
	namespace Core
	{
		class AnimationClip final
		{
		public:
			explicit AnimationClip();
			~AnimationClip() = default;
			AnimationClip(const AnimationClip& other) = default;
			AnimationClip(AnimationClip&& other) noexcept = default;
			AnimationClip& operator=(const AnimationClip& other) = default;
			AnimationClip& operator=(AnimationClip&& other) noexcept = default;

			void SetName(const std::wstring& name);
			void SetDuration(float duration);
			void SetTicksPerSecond(float ticksPerSecond);
			std::vector<AnimationKey>& GetKeys();
		private:
			std::wstring mName;
			float mDuration;
			float mTicksPerSecond;
			std::vector<AnimationKey> mKeys;
		};
	}
}

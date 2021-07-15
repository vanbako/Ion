#pragma once

#include "AnimationKey.h"

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

			const float GetDuration() const;
			const float GetTicksPerSecond() const;
			std::vector<AnimationKey>& GetKeys();
		private:
			std::wstring mName;
			float mDuration;
			float mTicksPerSecond;
			std::vector<AnimationKey> mKeys;
		};
	}
}

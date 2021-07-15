#pragma once

namespace Ion
{
	namespace Core
	{
		class AnimationKey final
		{
		public:
			explicit AnimationKey();
			~AnimationKey() = default;
			AnimationKey(const AnimationKey& other) = default;
			AnimationKey(AnimationKey&& other) noexcept = default;
			AnimationKey& operator=(const AnimationKey& other) = default;
			AnimationKey& operator=(AnimationKey&& other) noexcept = default;

			void SetTick(float tick);

			const float GetTick() const;
			std::vector<DirectX::XMFLOAT4X4>& GetBoneTransforms();
		private:
			float mTick;
			std::vector<DirectX::XMFLOAT4X4> mBoneTransforms;
		};
	}
}

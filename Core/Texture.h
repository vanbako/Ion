#pragma once
#include "../Core/ResourceUploadBatch.h"

namespace Ion
{
	namespace Core
	{
		class Application;

		class Texture final
		{
		public:
			explicit Texture(Application* pApplication, const std::string& name);
			~Texture() = default;
			Texture(const Texture& other) = delete;
			Texture(Texture&& other) noexcept = delete;
			Texture& operator=(const Texture& other) = delete;
			Texture& operator=(Texture&& other) noexcept = delete;

			const Microsoft::WRL::ComPtr<ID3D12Resource>& GetTexture() const;
		private:
			bool mIsInitialized;
			Application* mpApplication;
			Microsoft::WRL::ComPtr<ID3D12Resource> mpTexture;
			DirectX::ResourceUploadBatch mUploadBatch;
		};
	}
}

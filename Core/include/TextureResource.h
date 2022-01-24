#pragma once

#include "Resource.h"

namespace Ion
{
	namespace Core
	{
		class Application;
		class Texture;

		class TextureResource final
			: public Resource
		{
		public:
			explicit TextureResource(Core::ResourceManager* pResourceManager);
			virtual ~TextureResource();
			TextureResource(const TextureResource& other) = delete;
			TextureResource(TextureResource&& other) noexcept = delete;
			TextureResource& operator=(const TextureResource& other) = delete;
			TextureResource& operator=(TextureResource&& other) noexcept = delete;

			Core::Texture* AddTexture(const std::string& name);
			void RemoveTexture(const std::string& name);
		private:
			std::map<std::string, Core::Texture*> mpResources;

			virtual void Clear() override;
		};
	}
}

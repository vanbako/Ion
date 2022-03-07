#pragma once

#include "Resource.h"

namespace Ion
{
	namespace Core
	{
		class Material2D;

		class Material2DResource final
			: public Resource
		{
		public:
			explicit Material2DResource(Core::ResourceManager* pResourceManager);
			virtual ~Material2DResource();
			Material2DResource(const Material2DResource& other) = delete;
			Material2DResource(Material2DResource&& other) noexcept = delete;
			Material2DResource& operator=(const Material2DResource& other) = delete;
			Material2DResource& operator=(Material2DResource&& other) noexcept = delete;

			Core::Material2D* AddMaterial2D(const std::string& name);
			void RemoveMaterial2D(const std::string& name);
		private:
			std::unordered_map<std::string, Core::Material2D*> mpMaterial2Ds;

			virtual void Clear() override;
		};
	}
}

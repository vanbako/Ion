#pragma once

#include "Resource.h"

namespace Ion
{
	namespace Core
	{
		class Application;
		class Material3D;

		class Material3DResource final
			: public Resource
		{
		public:
			explicit Material3DResource(Core::ResourceManager* pResourceManager);
			virtual ~Material3DResource();
			Material3DResource(const Material3DResource& other) = delete;
			Material3DResource(Material3DResource&& other) noexcept = delete;
			Material3DResource& operator=(const Material3DResource& other) = delete;
			Material3DResource& operator=(Material3DResource&& other) noexcept = delete;

			Core::Material3D* AddMaterial3D(const std::string& name);
			void RemoveMaterial3D(const std::string& name);
		private:
			std::unordered_map<std::string, Core::Material3D*> mpMaterial3Ds;

			virtual void Clear() override;
		};
	}
}

#pragma once
#include "Resource.h"
#include "Winding.h"
#include "CoordSystem.h"

namespace Ion
{
	namespace Core
	{
		class Application;
		class MeshModel;

		class MeshModelResource final
			: public Resource
		{
		public:
			explicit MeshModelResource(Core::ResourceManager* pResourceManager);
			virtual ~MeshModelResource();
			MeshModelResource(const MeshModelResource& other) = delete;
			MeshModelResource(MeshModelResource&& other) noexcept = delete;
			MeshModelResource& operator=(const MeshModelResource& other) = delete;
			MeshModelResource& operator=(MeshModelResource&& other) noexcept = delete;

			Core::MeshModel* AddMeshModel(const std::string& name, const std::string& fileExtension, Core::Winding winding, Core::CoordSystem coordSystem);
			void RemoveModel(const std::string& name);
		private:
			std::map<std::string, Core::MeshModel*> mpResources;

			virtual void Clear() override;
		};
	}
}

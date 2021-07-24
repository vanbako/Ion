#pragma once
#include "ModelVC.h"
#include "InstanceBuffer.h"
#include "TransformMC.h"

// Instanced Model View Component

namespace Ion
{
	namespace Core
	{
		class Object;
		class Material3D;
		class Canvas3D;
		class Model;
		class Texture;

		class InstancedModelVC
			: public Core::ModelVC
		{
		public:
			explicit InstancedModelVC(const std::string& modelName, const std::string& modelExtension, const std::string& materialName, bool isActive, Core::Winding winding, Core::CoordSystem coordSystem, Core::Object* pObject);
			virtual ~InstancedModelVC();
			InstancedModelVC(const InstancedModelVC& other) = default;
			InstancedModelVC(InstancedModelVC&& other) noexcept = default;
			InstancedModelVC& operator=(const InstancedModelVC& other) = default;
			InstancedModelVC& operator=(InstancedModelVC&& other) noexcept = default;

			void AddInstance(const Core::TransformMC& transform);
			void AddInstances(const std::vector<Core::TransformMC>& transforms);
			void ReadInstances();
			std::vector<Core::TransformMC>& GetInstances();

			virtual void Initialize() override;
			virtual void Update(float delta) override;
			virtual void Render(Core::Canvas* pCanvas, Core::Material3D* pMaterial) override;
			virtual void Render(Core::Canvas* pCanvas, Core::Material2D* pMaterial) override { (pCanvas); (pMaterial); };
		private:
			static const size_t mMaxInstances;
			std::vector<Core::TransformMC> mTransforms;

			Microsoft::WRL::ComPtr<ID3D12Resource> mpInstanceBuffer;
			std::vector<Core::InstanceBuffer> mInstanceBufferData;
			UINT8* mpInstanceDataBegin;
		};
	}
}

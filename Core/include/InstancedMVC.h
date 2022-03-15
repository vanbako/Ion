#pragma once
#include "MeshModelVC.h"
#include "InstanceBuffer.h"

// Instanced Model View Component

namespace Ion
{
	namespace Core
	{
		class Object;
		class Material3D;
		class Canvas3D;
		class MeshModel;
		class Texture;
		class InstancedTransformMC;

		class InstancedMVC final
			: public Core::MeshModelVC
		{
		public:
			explicit InstancedMVC(const std::string& modelName, const std::string& modelExtension, const std::string& materialName, bool isActive, Core::Winding winding, Core::CoordSystem coordSystem, Core::Object* pObject);
			virtual ~InstancedMVC() = default;
			InstancedMVC(const InstancedMVC& other) = default;
			InstancedMVC(InstancedMVC&& other) noexcept = default;
			InstancedMVC& operator=(const InstancedMVC& other) = default;
			InstancedMVC& operator=(InstancedMVC&& other) noexcept = default;

			//void AddInstance(const Core::TransformMC& transform);
			//void AddInstances(const std::vector<Core::TransformMC>& transforms);
			void SetInstancedTransform(InstancedTransformMC* pInstancedTransform);

			virtual void Initialize() override;
			virtual void Update(float delta) override;
			virtual bool Render(Core::Canvas* pCanvas, Core::Material3D* pMaterial, float distSq) override;
			virtual bool Render(Core::Canvas* pCanvas, Core::Material2D* pMaterial) override { (pCanvas); (pMaterial); return false; };
		private:
			InstancedTransformMC* mpInstancedTransform;
			Microsoft::WRL::ComPtr<ID3D12Resource> mpInstanceBuffer;
			std::vector<Core::InstanceBuffer> mInstanceBufferData;
			UINT8* mpInstanceDataBegin;
		};
	}
}

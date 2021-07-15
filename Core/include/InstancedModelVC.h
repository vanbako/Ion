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
			: public ModelVC
		{
		public:
			explicit InstancedModelVC(const std::string& modelName, const std::string& materialName, bool isActive, Winding winding, CoordSystem coordSystem, Object* pObject);
			virtual ~InstancedModelVC();
			InstancedModelVC(const InstancedModelVC& other) = default;
			InstancedModelVC(InstancedModelVC&& other) noexcept = default;
			InstancedModelVC& operator=(const InstancedModelVC& other) = default;
			InstancedModelVC& operator=(InstancedModelVC&& other) noexcept = default;

			void AddInstance(const TransformMC& transform);
			void AddInstances(const std::vector<TransformMC>& transforms);
			void ReadInstances();
			std::vector<TransformMC>& GetInstances();

			virtual void Initialize() override;
			virtual void Update(float delta) override;
			virtual void Render(Canvas* pCanvas, Material3D* pMaterial) override;
			virtual void Render(Canvas* pCanvas, Material2D* pMaterial) override { (pCanvas); (pMaterial); };
		private:
			static const size_t mMaxInstances;
			std::vector<TransformMC> mTransforms;

			Microsoft::WRL::ComPtr<ID3D12Resource> mpInstanceBuffer;
			std::vector<InstanceBuffer> mInstanceBufferData;
			UINT8* mpInstanceDataBegin;
		};
	}
}

#pragma once

namespace Ion
{
	namespace Core
	{
		class Scene;
		class Object;

		class Factory final
		{
		public:
			explicit Factory() = delete;
			~Factory() = delete;
			Factory(const Factory& other) = delete;
			Factory(Factory&& other) noexcept = delete;
			Factory& operator=(const Factory& other) = delete;
			Factory& operator=(Factory&& other) noexcept = delete;

			static Core::Object* AddCamera(Core::Scene* pScene);
			static Core::Object* AddCube(
				Core::Scene* pScene,
				float width = 1.f,
				float height = 1.f,
				float depth = 1.f,
				DirectX::XMFLOAT4 color = (DirectX::XMFLOAT4)DirectX::Colors::White);
			static void CreatePhysicsPlane(physx::PxPhysics* pPxPhysics, physx::PxScene* pPxScene);
		};
	}
}

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

			static Object* AddCamera(Scene* pScene);
			static Object* AddCube(
				Scene* pScene,
				float width = 1.f,
				float height = 1.f,
				float depth = 1.f,
				DirectX::XMFLOAT4 color = (DirectX::XMFLOAT4)DirectX::Colors::White);
			static void CreatePhysicsPlane(physx::PxPhysics* pPxPhysics, physx::PxScene* pPxScene);
		};
	}
}

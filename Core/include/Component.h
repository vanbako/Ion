#pragma once

namespace Ion
{
	namespace Core
	{
		class Object;

		class Component
		{
		public:
			explicit Component(bool isActive, Core::Object* pObject);
			virtual ~Component() = default;
			Component(const Component& other) = default;
			Component(Component&& other) noexcept = default;
			Component& operator=(const Component& other) = default;
			Component& operator=(Component&& other) noexcept = default;

			virtual void Initialize();
			virtual void Update(float) = 0;

			void SetIsActive(bool isActive);
			const bool GetIsActive() const;
			void SetObject(Core::Object* pObject);
			Core::Object* GetObject();
		protected:
			bool mIsInitialized;
			bool mIsActive;
			Core::Object* mpObject;
		};
	}
}

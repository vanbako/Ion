#pragma once
#include "../Core/TransformMC.h"
#include "../Core/ModelVC.h"
#include "../Core/InstancedModelVC.h"
#include "../Core/Winding.h"

namespace Ion
{
	namespace Core
	{
		class Scene;
		class ModelC;
		class ControllerC;
		class ViewC;

		class Object final
		{
		public:
			explicit Object(bool isActive, Scene* pScene, Object* pParent = nullptr);
			~Object();
			Object(const Object& other) = delete;
			Object(Object&& other) noexcept = delete;
			Object& operator=(const Object& other) = delete;
			Object& operator=(Object&& other) noexcept = delete;

			void SetIsActive(bool isActive, bool includeComponents = false);
			const bool GetIsActive() const;
			Scene* GetScene();

			Object* GetParent();
			Object* AddChildObject(bool isActive);

			template<class T>
			T* AddModelC(bool isActive)
			{
				mpModelCs.emplace_back(new T{ isActive, this });
				return (T*)mpModelCs.back();
			}
			template<>
			TransformMC* AddModelC(bool isActive)
			{
				if (mpTransformMC == nullptr)
					mpTransformMC = new TransformMC{ isActive, this };
				mpModelCs.emplace_back(mpTransformMC);
				return mpTransformMC;
			}
			template<class T>
			T* GetModelC()
			{
				for (auto pModelC : mpModelCs)
					if (typeid(*pModelC).hash_code() == typeid(T).hash_code())
						return (T*)pModelC;
				return nullptr;
			}
			template<>
			TransformMC* GetModelC()
			{
				return mpTransformMC;
			}
			template<class T>
			void GetModelCs(std::vector<T*>& modelCs)
			{
				for (auto pModelC : mpModelCs)
					if (typeid(*pModelC).hash_code() == typeid(T).hash_code())
						modelCs.push_back((T*)pModelC);
			}
			template<class T>
			T* AddControllerC(bool isActive)
			{
				mpControllerCs.emplace_back(new T{ isActive, this });
				return (T*)mpControllerCs.back();
			}
			template<class T>
			T* GetControllerC()
			{
				for (auto pControllerC : mpControllerCs)
					if (typeid(*pControllerC).hash_code() == typeid(T).hash_code())
						return (T*)pControllerC;
				return nullptr;
			}
			template<class T>
			void GetControllerCs(std::vector<T*>& controllerCs)
			{
				for (auto pControllerC : mpControllerCs)
					if (typeid(*pControllerC).hash_code() == typeid(T).hash_code())
						controllerCs.push_back((T*)pControllerC);
			}
			void ModelCInitialize();
			void ModelCUpdate(float delta);
			void ModelCSwitch();
			void ControllerCInitialize();
			void ControllerCUpdate(float delta);
			template<class T>
			T* AddViewC(bool isActive)
			{
				mpViewCs.emplace_back(new T{ isActive, this });
				return (T*)mpViewCs.back();
			}
			template<class T>
			T* AddViewC(const std::string& modelName, const std::string& materialName, bool isActive, Winding winding = Winding::CW)
			{
			}
			template<>
			ModelVC* AddViewC(const std::string& modelName, const std::string& materialName, bool isActive, Winding winding)
			{
				mpViewCs.emplace_back(new ModelVC{ modelName, materialName, isActive, winding, this });
				return (ModelVC*)mpViewCs.back();
			}
			template<>
			InstancedModelVC* AddViewC(const std::string& modelName, const std::string& materialName, bool isActive, Winding winding)
			{
				mpViewCs.emplace_back(new InstancedModelVC{ modelName, materialName, isActive, winding, this });
				return (InstancedModelVC*)mpViewCs.back();
			}
			template<class T>
			T* GetViewC()
			{
				for (auto pViewC : mpViewCs)
					if (typeid(*pViewC).hash_code() == typeid(T).hash_code())
						return (T*)pViewC;
				return nullptr;
			}
			template<class T>
			void GetViewCs(std::vector<T*>& viewCs)
			{
				for (auto pViewC : mpViewCs)
					if (typeid(*pViewC).hash_code() == typeid(T).hash_code())
						viewCs.push_back((T*)pViewC);
			}
			void ViewCInitialize();
			void ViewCUpdate(float delta);
		private:
			bool mIsActive;
			Scene* mpScene;
			Object* mpParentObject;
			std::list<Object> mChildObjects;
			TransformMC* mpTransformMC;
			std::list<ModelC*> mpModelCs;
			std::list<ControllerC*> mpControllerCs;
			std::list<ViewC*> mpViewCs;
		};
	}
}

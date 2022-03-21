#pragma once
#include "TransformMC.h"
#include "MeshModelVC.h"
#include "InstancedMVC.h"
#include "Winding.h"

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
			explicit Object(bool isActive, Core::Scene* pScene, Core::Object* pParent = nullptr);
			~Object();
			Object(const Object& other) = delete;
			Object(Object&& other) noexcept = delete;
			Object& operator=(const Object& other) = delete;
			Object& operator=(Object&& other) noexcept = delete;

			void SetIsActive(bool isActive, bool includeComponents = false);
			const bool GetIsActive() const;
			Core::Scene* GetScene();

			Core::Object* GetParent();
			Core::Object* AddChildObject(bool isActive);

			template<class T>
			T* AddModelC(bool isActive)
			{
				return (T*)mpModelCs.emplace_back(new T{ isActive, this });
			}
			template<>
			Core::TransformMC* AddModelC(bool isActive)
			{
				if (mpTransformMC == nullptr)
					mpTransformMC = new Core::TransformMC{ isActive, this };
				mpModelCs.emplace_back(mpTransformMC);
				return mpTransformMC;
			}
			template<class T>
			void AttachModelC(T* pModelC)
			{
				pModelC->SetObject(this);
				mpModelCs.emplace_back(pModelC);
			}
			template<class T>
			T* GetModelC()
			{
				for (auto pModelC : mpModelCs)
					if (typeid(*pModelC).hash_code() == typeid(T).hash_code())
						return (T*)pModelC;
				return nullptr;
			}
			template<class T>
			bool HasModelC()
			{
				for (auto pModelC : mpModelCs)
					if (typeid(*pModelC).hash_code() == typeid(T).hash_code())
						return true;
				return false;
			}
			template<class T>
			T* DetachModelC()
			{
				for (auto it{ mpModelCs.begin() }; it != mpModelCs.end(); ++it)
					if (typeid(**it).hash_code() == typeid(T).hash_code())
					{
						auto ret{ *it };
						mpModelCs.erase(it);
						return (T*)ret;
					}
				return nullptr;
			}
			template<>
			Core::TransformMC* GetModelC()
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
				return (T*)mpControllerCs.emplace_back(new T{ isActive, this });
			}
			template<class T>
			void AttachControllerC(T* pControllerC)
			{
				pControllerC->SetObject(this);
				mpControllerCs.emplace_back(pControllerC);
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
			bool HasControllerC()
			{
				for (auto pControllerC : mpControllerCs)
					if (typeid(*pControllerC).hash_code() == typeid(T).hash_code())
						return true;
				return false;
			}
			template<class T>
			T* DetachControllerC()
			{
				for (auto it{ mpControllerCs.begin() }; it != mpControllerCs.end(); ++it)
					if (typeid(**it).hash_code() == typeid(T).hash_code())
					{
						auto ret{ *it };
						mpControllerCs.erase(it);
						return (T*)ret;
					}
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
			//void ModelCUpdate(float delta);
			//void ModelCSwitch();
			void ControllerCInitialize();
			void ControllerCUpdate(float delta);
			template<class T>
			T* AddViewC(bool isActive)
			{
				return (T*)mpViewCs.emplace_back(new T{ isActive, this });
			}
			template<class T>
			T* AddViewC(const std::string& filename, float width, float depth, std::size_t rowCount, std::size_t colCount, bool isActive)
			{
				return (T*)mpViewCs.emplace_back(new T{ filename, width, depth, rowCount, colCount, isActive, this });
			}
			template<class T>
			T* AddViewC(const std::string& materialName, bool isActive)
			{
				return (T*)mpViewCs.emplace_back(new T{ materialName, isActive, this });
			}
			template<class T>
			T* AddViewC(const std::string& modelName, const std::string& modelExtension, const std::string& materialName, bool isActive, Core::Winding winding = Core::Winding::CW, Core::CoordSystem coordSystem = Core::CoordSystem::LeftHanded)
			{
				return (T*)mpViewCs.emplace_back(new T{ modelName, modelExtension, materialName, isActive, winding, coordSystem , this });
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
			Core::Scene* mpScene;
			Core::Object* mpParentObject;
			std::list<Core::Object> mChildObjects;
			Core::TransformMC* mpTransformMC;
			std::list<Core::ModelC*> mpModelCs;
			std::list<Core::ControllerC*> mpControllerCs;
			std::list<Core::ViewC*> mpViewCs;
		};
	}
}

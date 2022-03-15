#pragma once
#include "ViewC.h"
#include "Triangle.h"

// Triangle View Component

namespace Ion
{
	namespace Core
	{
		class Object;
		class Material2D;
		class Canvas2D;

		class TextVC
			: public Core::ViewC
		{
		public:
			explicit TextVC(bool isActive, Core::Object* pObject);
			virtual ~TextVC() = default;
			TextVC(const TextVC& other) = default;
			TextVC(TextVC&& other) noexcept = default;
			TextVC& operator=(const TextVC& other) = default;
			TextVC& operator=(TextVC&& other) noexcept = default;

			virtual void Initialize() override;
			virtual void Update(float delta) override;
			virtual bool Render(Core::Canvas* pCanvas, Core::Material3D* pMaterial, float distSq) override;
			virtual bool Render(Core::Canvas* pCanvas, Core::Material2D* pMaterial) override;

			void SetText(const std::wstring& text);
			void SetRect(const D2D1_RECT_F& rect);
		private:
			Microsoft::WRL::ComPtr<ID2D1PathGeometry> mpPathGeometry;
			std::wstring mText;
			D2D1_RECT_F mRect;
		};
	}
}

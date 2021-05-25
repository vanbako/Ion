#pragma once

namespace Ion
{
	namespace Core
	{
		template<class T>
		class Rectangle final
		{
		public:
			explicit Rectangle()
				: Rectangle(T{}, T{}, T{}, T{})
			{}
			explicit Rectangle(T left, T top, T width, T height)
				: mLeft{ left }
				, mTop{ top }
				, mWidth{ width }
				, mHeight{ height }
			{}
			~Rectangle() = default;
			Rectangle(const Rectangle& other) = default;
			Rectangle(Rectangle&& other) noexcept = default;
			Rectangle& operator=(const Rectangle& other) = default;
			Rectangle& operator=(Rectangle&& other) noexcept = default;

			const T& GetLeft() const { return mLeft; }
			const T& GetTop() const { return mTop; }
			const T& GetWidth() const { return mWidth; }
			const T& GetHeight() const { return mHeight; }
		private:
			T mLeft;
			T mTop;
			T mWidth;
			T mHeight;
		};
	}
}

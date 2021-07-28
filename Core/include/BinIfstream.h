#pragma once

#include "Logger.h"

namespace Ion
{
	namespace Core
	{
		class Application;

		class BinIfstream final
		{
		public:
			explicit BinIfstream(Core::Application* pApplication, const std::string& name);
			~BinIfstream();
			BinIfstream(const BinIfstream& other) = delete;
			BinIfstream(BinIfstream&& other) noexcept = delete;
			BinIfstream& operator=(const BinIfstream& other) = delete;
			BinIfstream& operator=(BinIfstream&& other) noexcept = delete;

			template<class T>
			T Read()
			{
#ifdef _DEBUG
				if ((size_t(mFile.tellg()) + sizeof(T)) > mSize)
				{
					Fatal();
					return T{};
				}
#endif
				T value;
				mFile.read((char*)&value, sizeof(T));
				return value;
			}

			std::wstring ReadString();
			std::wstring ReadLongString();
			std::wstring ReadNullString();
			std::streampos GetPosition();
			void SetPosition(std::streampos pos);
			void MovePosition(size_t move);
			size_t GetSize();
		private:
			Core::Application* mpApplication;
			const std::string mName;
			std::ifstream mFile;
			size_t mSize;

#ifdef _DEBUG
			void Fatal();
#endif
		};
	}
}

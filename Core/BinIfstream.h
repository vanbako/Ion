#pragma once

namespace Ion
{
	namespace Core
	{
		class BinIfstream final
		{
		public:
			explicit BinIfstream(const std::string& name);
			~BinIfstream();
			BinIfstream(const BinIfstream& other) = delete;
			BinIfstream(BinIfstream&& other) noexcept = delete;
			BinIfstream& operator=(const BinIfstream& other) = delete;
			BinIfstream& operator=(BinIfstream&& other) noexcept = delete;

			template<class T>
			T Read()
			{
				T value;
				mFile.read((char*)&value, sizeof(T));
				return value;
			}

			std::wstring ReadString();
			std::wstring ReadLongString();
			std::wstring ReadNullString();
			std::streampos GetPosition();
			void SetPosition(std::streampos pos);
			void MovePosition(int move);
		private:
			std::ifstream mFile;
		};
	}
}

#pragma once

namespace Ion
{
	namespace Core
	{
		class BinOfstream final
		{
		public:
			explicit BinOfstream(const std::string& name);
			~BinOfstream();
			BinOfstream(const BinOfstream& other) = delete;
			BinOfstream(BinOfstream&& other) noexcept = delete;
			BinOfstream& operator=(const BinOfstream& other) = delete;
			BinOfstream& operator=(BinOfstream&& other) noexcept = delete;

			template<typename T>
			bool Write(const T& toWrite)
			{
				if (!std::is_pod<T>::value) return false;
				mFile.write((const char*)&toWrite, sizeof(T));
				return true;
			}
			void WriteString(const std::string& toWrite);
			void WriteString(const std::wstring& toWrite);
			void WriteByteLenString(const std::string& toWrite);
			void WriteByteLenString(const std::wstring& toWrite);
			void WriteNullString(const std::string toWrite);
			void WriteNullString(const std::wstring toWrite);
		private:
			std::ofstream mFile;
		};
	}
}

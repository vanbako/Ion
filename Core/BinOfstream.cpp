#include "pch.h"
#include "BinOfstream.h"

using namespace Ion;

Core::BinOfstream::BinOfstream(const std::string& name)
	: mFile{ name, std::ios::out | std::ios::binary }
{
}

Core::BinOfstream::~BinOfstream(void)
{
	mFile.close();
}

void Core::BinOfstream::WriteString(const std::string& toWrite)
{
	std::size_t length{ toWrite.length() };
	mFile.write((const char*)&length, sizeof(std::size_t));
	mFile.write((const char*)(toWrite.c_str()), length);
}

void Core::BinOfstream::WriteString(const std::wstring& toWrite)
{
	std::size_t length{ toWrite.length() * sizeof(wchar_t) };
	mFile.write((const char*)&length, sizeof(std::size_t));
	mFile.write((const char*)(toWrite.c_str()), length);
}

void Core::BinOfstream::WriteByteLenString(const std::string& toWrite)
{
	unsigned char length{ unsigned char(toWrite.length()) };
	mFile.write((const char*)&length, sizeof(unsigned char));
	mFile.write((const char*)(toWrite.c_str()), length);
}

void Core::BinOfstream::WriteByteLenString(const std::wstring& toWrite)
{
	unsigned char length{ unsigned char(toWrite.length() * sizeof(wchar_t)) };
	mFile.write((const char*)&length, sizeof(unsigned char));
	mFile.write((const char*)(toWrite.c_str()), length);
}

void Core::BinOfstream::WriteNullString(const std::string toWrite)
{
	mFile.write((const char*)(toWrite.c_str()), toWrite.length());
	const char nullChar{ '\0' };
	mFile.write(&nullChar, 1);
}

void Core::BinOfstream::WriteNullString(const std::wstring toWrite)
{
	mFile.write((const char*)(toWrite.c_str()), toWrite.length() * sizeof(wchar_t));
	const wchar_t nullChar{ L'\0' };
	mFile.write((const char*)&nullChar, 2);
}

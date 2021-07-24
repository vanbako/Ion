#include "pch.h"
#include "BinIfstream.h"

using namespace Ion;

Core::BinIfstream::BinIfstream(const std::string& name)
	: mFile{ name, std::ios::in | std::ios::binary }
{
}

Core::BinIfstream::~BinIfstream(void)
{
	mFile.close();
}

std::wstring Core::BinIfstream::ReadLongString()
{
	UINT stringLength{ Read<UINT>() };
	std::wstringstream ss{};
	for (UINT i{ 0 }; i < stringLength; ++i)
		ss << Read<wchar_t>();
	return (std::wstring)ss.str();
}

std::wstring Core::BinIfstream::ReadNullString()
{
	std::string buff{};
	std::getline(mFile, buff, '\0');
	return std::wstring(buff.begin(), buff.end());
}

std::wstring Core::BinIfstream::ReadString()
{
	int stringLength{ int(Read<char>()) };
	std::wstringstream ss{};
	for (int i{ 0 }; i < stringLength; ++i)
		ss << Read<char>();
	return (std::wstring)ss.str();
}

std::streampos Core::BinIfstream::GetPosition()
{
	return mFile.tellg();
}

void Core::BinIfstream::SetPosition(std::streampos pos)
{
	mFile.seekg(pos);
}

void Core::BinIfstream::MovePosition(int move)
{
	std::streampos currPos{ GetPosition() };
	if (currPos > 0)
		SetPosition(currPos + std::streampos(move));
}

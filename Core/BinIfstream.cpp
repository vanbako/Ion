#include "pch.h"
#include "BinIfstream.h"
#include "Application.h"

using namespace Ion;

Core::BinIfstream::BinIfstream(Core::Application* pApplication, const std::string& name)
	: mpApplication{ pApplication }
	, mName{ name }
	, mFile{ name, std::ios::in | std::ios::binary | std::ios::ate }
	, mSize{}
{
	mSize = mFile.tellg();
	mFile.seekg(0);
}

Core::BinIfstream::~BinIfstream(void)
{
	mFile.close();
}

std::wstring Core::BinIfstream::ReadLongString()
{
	UINT stringLength{ Read<UINT>() };
	std::wstringstream ss{};
#ifdef _DEBUG
	if ((std::size_t(mFile.tellg()) + std::size_t(stringLength)) > mSize)
	{
#ifdef ION_LOGGER
		mpApplication->GetServiceLocator().GetLogger()->Message(typeid(this).name(), Core::MsgType::Fatal, "BinIfstream reading past eof " + mName);
#endif
		return (std::wstring)ss.str();
	}
#endif
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
#ifdef _DEBUG
	if ((std::size_t(mFile.tellg()) + std::size_t(stringLength)) > mSize)
	{
#ifdef ION_LOGGER
		mpApplication->GetServiceLocator().GetLogger()->Message(typeid(this).name(), Core::MsgType::Fatal, "BinIfstream reading past eof " + mName);
#endif
		return (std::wstring)ss.str();
	}
#endif
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
#ifdef _DEBUG
	if (std::size_t(pos) > mSize)
	{
#ifdef ION_LOGGER
		mpApplication->GetServiceLocator().GetLogger()->Message(typeid(this).name(), Core::MsgType::Fatal, "BinIfstream position past eof " + mName);
#endif
		return;
	}
#endif
	mFile.seekg(pos);
}

void Core::BinIfstream::MovePosition(std::size_t move)
{
	std::streampos currPos{ GetPosition() };
	if (currPos > 0)
		SetPosition(currPos + std::streampos(move));
}

std::size_t Core::BinIfstream::GetSize()
{
	return mSize;
}

#ifdef _DEBUG
void Core::BinIfstream::Fatal()
{
#ifdef ION_LOGGER
	mpApplication->GetServiceLocator().GetLogger()->Message(typeid(this).name(), Core::MsgType::Fatal, "BinIfstream reading past eof " + mName);
#endif
}
#endif

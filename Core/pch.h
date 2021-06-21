#pragma once

#define WIN32_LEAN_AND_MEAN
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include <typeindex>
#include <typeinfo>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <bitset>
#include <chrono>
#include <thread>
#include <shared_mutex>
#include <fstream>
#include <locale>
#include <codecvt>

#include <Windows.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

#include <wrl.h>
#include <dxgi.h>
#include <d3d12.h>
#include <d3d12shader.h>
#include <d3d12sdklayers.h>

#include "../Core/Vector.h"
#include "../Core/Rectangle.h"
#include "../Core/AngleUnit.h"

inline std::string HrToString(HRESULT hr)
{
    char s_str[64] = {};
    sprintf_s(s_str, "HRESULT of 0x%08X", static_cast<UINT>(hr));
    return std::string(s_str);
}

class HrException : public std::runtime_error
{
public:
    HrException(HRESULT hr) : std::runtime_error(HrToString(hr)), m_hr(hr) {}
    HRESULT Error() const { return m_hr; }
private:
    const HRESULT m_hr;
};

#define SAFE_RELEASE(p) if (p) (p)->Release()

inline void ThrowIfFailed(HRESULT hr)
{
    if (FAILED(hr))
    {
        throw HrException(hr);
    }
}

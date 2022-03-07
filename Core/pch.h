#pragma once

#define ION_LOGGER
#define ION_STATS

#define WIN32_LEAN_AND_MEAN
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

#include <typeindex>
#include <typeinfo>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <set>
#include <bitset>
#include <chrono>
#include <thread>
#include <shared_mutex>
#include <iostream>
#include <fstream>
#include <locale>
#include <codecvt>
#include <ctime>
//#include <random>
#define _USE_MATH_DEFINES
#include <cmath>

#include <Windows.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

#include "d3dx12.h"
#include <wrl.h>
#include <dxgi.h>
#include <dxgi1_2.h>
#include <dxgi1_3.h>
#include <dxgi1_4.h>
#include <dxgi1_5.h>
#include <d3d12.h>
#include <d3d12shader.h>
#include <d3d12sdklayers.h>

#include <d3d11on12.h>
#include <d2d1.h>
#include <d2d1_1.h>
#include <d2d1_2.h>
#include <d2d1_3.h>
#include <dwrite.h>

#include <PxPhysicsAPI.h>

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

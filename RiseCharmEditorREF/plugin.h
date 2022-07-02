#pragma once

#include "reframework/API.hpp"
#define RE_EXPORT __declspec(dllexport)

#include <atomic>

extern "C" lua_State * g_lua;
inline std::atomic_bool g_initialized = false;

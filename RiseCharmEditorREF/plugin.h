#pragma once

#include "reframework/API.hpp"
#define RE_EXPORT __declspec(dllexport)

extern "C" lua_State * g_lua;
inline bool g_initialized = false;

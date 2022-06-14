#include <vector>
#include <string>
#include "../repo/Trie.hpp"

#pragma once

#ifdef REDUCERDLL_EXPORTS
#define REDUCERDLL_API __declspec(dllexport)
#else
#define REDUCERDLL_API __declspec(dllimport)
#endif

extern "C" REDUCERDLL_API bool reduce();
extern "C" REDUCERDLL_API void heartbeat(int port);
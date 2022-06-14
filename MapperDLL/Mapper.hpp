#include <vector>
#include <string>

#pragma once

#ifdef MAPPERDLL_EXPORTS
#define MAPPERDLL_API __declspec(dllexport)
#else
#define MAPPERDLL_API __declspec(dllimport)
#endif

extern "C" MAPPERDLL_API void map(std::string fileName);
extern "C" MAPPERDLL_API bool test();
extern "C" MAPPERDLL_API void exporter(std::vector<std::string> vect);
extern "C" MAPPERDLL_API void heartbeat(int port);
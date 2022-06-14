#ifndef  _WINSOCKAPI_
#define _WINSOCKAPI_
#endif // ! _WINSOCKAPI_

#define CONTROLLER_LISTENING_PORT 10000
#define REDUCER_STUB_PORT 10001
#define BASE_STUB_PORT 10002

#include <string>
#include <iostream>
#include <boost/log/trivial.hpp>
#include "boost/algorithm/string.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <Windows.h>
#include "Trie.hpp"

#pragma once

class Workflow {
	// params: input unsorted string vector
	// returns: "sorted" trie of words
	void sort(Trie*, std::vector<std::string>);
public:
	// params: input dir, temp dir, output dir
	// returns: pass/fail bool
	bool run(std::string, std::string, std::string, std::string);
	static bool test(std::string);
	void statusListener();
};
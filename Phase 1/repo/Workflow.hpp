#include <string>
#include <iostream>
#include <sys/stat.h>
#include <boost/log/trivial.hpp>
#include <direct.h>
#include <stdlib.h>
#include <stdio.h>
#include <filesystem>
#include <string>

#include "Trie.hpp"

#pragma once

class Workflow {
	// params: input unsorted string vector
	// returns: "sorted" trie of words
	Trie* sort(std::vector<std::string>);
public:
	// params: input dir, temp dir, output dir
	// returns: pass/fail bool
	bool run(std::string, std::string, std::string);
};
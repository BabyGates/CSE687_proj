#include "framework.h"
#include "direct.h"
#include "Mapper.hpp"
#include <vector>
#include <string>
#include "../repo/FileManager.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>
#include "boost/algorithm/string.hpp"

MAPPERDLL_API void map(int key, std::string container, std::string dir) {
	// the vector to call export on
	std::vector<std::string> outVect;
	std::string temp;
	for (int i = 0; i < container.length(); i++) {
		if (isspace(container[i])) {
			// found end of a word. remove any punctuation and push it back
			temp.erase(std::remove_if(temp.begin(), temp.end(), ispunct), temp.end());
			// to lower case
			boost::to_lower(temp);
			outVect.push_back(temp);
			temp = "";
		}
		else {
			temp.push_back(container[i]);
		}
	}
	temp.erase(std::remove_if(temp.begin(), temp.end(), ispunct), temp.end());
	boost::to_lower(temp);
	outVect.push_back(temp);
	exporter(outVect, dir, key);
}

MAPPERDLL_API void exporter(std::vector<std::string> vect, std::string dir, int key) {
	std::string outStr;
	for (auto& itr : vect) {
		if (itr == "") {
			continue;
		}
		outStr += '\"' + itr + "\", 1\n";
	}
	FileManager::write(outStr, dir, key);
}

MAPPERDLL_API bool test() {
	// tests map and export
	try {
		std::string testStr = "test string test STRING\nTEST\nSTRING this is a test";
		_mkdir("test");
		map(0, testStr, "test");
		std::vector<std::string> outVect = FileManager::read("test", 0);
		for (auto& line : outVect) {
			if (strstr(line.c_str(), ", 1") == nullptr || line.at(0) != '\"') {
				std::filesystem::remove_all("test");
				return false;
			}
		}
		std::filesystem::remove_all("test");
		return true;
	}
	catch (...) {
		std::filesystem::remove_all("test");
		return false;
	}
}
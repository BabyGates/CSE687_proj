//FileManager.hpp
//definitions of FileManager class
#include <sys/stat.h>
#include <direct.h>

#include <string>
#include <vector>

#pragma once

class FileManager {
public:
	static std::vector<std::string> fetchLines(std::string input);
	static std::vector<std::string> read(std::string directory, int key = 0);
	static void write(std::string directory, int key, std::string content);
	static bool init(std::string, std::string, std::string);
	static void printFinal(bool pass, std::string dir);
};

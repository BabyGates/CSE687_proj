//FileManager.hpp
//definitions of FileManager class


#include <string>
#include <vector>

#pragma once

class FileManager {
public:
	std::vector<std::string> fetchLines(std::string input);
	std::vector<std::string> read(std::string directory, int key = 0);
	void write(std::string directory, int key, std::string content);
};

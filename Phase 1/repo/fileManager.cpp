//FileManager.cpp
//implementations of FileManager class



#include "FileManager.hpp"
#include <fstream>
#include <filesystem>
#include <boost/log/trivial.hpp>

std::vector<std::string> FileManager::fetchLines(std::string input) {
	std::vector<std::string> linesVector;
	std::string content;
	std::string directoryPath = input; //store input directory in directoryPath variable

	for (const auto & entry : std::filesystem::directory_iterator(directoryPath)) { //for each file in the directory
		std::ifstream file;
		file.open(directoryPath); //open the file

		while (file.good()) {
			file >> content; //push into content variable
			linesVector.push_back(content); //push onto the vector
		}
	}

	return linesVector; //return vector
}

std::vector<std::string> FileManager::read(std::string directory, int key) {
	std::vector<std::string> retVect;
	std::string filePathName = directory;
	filePathName += "/file";
	filePathName += std::to_string(key);
	filePathName += ".txt";
	BOOST_LOG_TRIVIAL(info) << "file path: " << filePathName;
	std::ifstream file(filePathName);
	std::string line;
	while (std::getline(file, line))
	{
		retVect.push_back(line);
	}
	return retVect;
}

void FileManager::write(std::string directory, int key, std::string content) {
	std::string filePath = directory; //store desired directory input into path variable
	filePath = "/file" + std::to_string(key) + ".txt"; //update path with file named based on specific key of file

	BOOST_LOG_TRIVIAL(info) << "file path: " << filePath;

	std::ofstream file(filePath); //specifies directory to be opened in
	file.open("file" + std::to_string(key) + ".txt", std::ios::app); //open file
	file << content; //write content into file
	file.close(); //close file
}
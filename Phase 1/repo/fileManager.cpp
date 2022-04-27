//FileManager.cpp
//implementations of FileManager class



#include "FileManager.hpp"
#include <fstream>
#include <filesystem>
#include <boost/log/trivial.hpp>
#include <sstream>

std::vector<std::string> FileManager::fetchLines(std::string input) {
	std::vector<std::string> linesVector;
	std::stringstream content;
	std::string directoryPath = input; //store input directory in directoryPath variable

	for (const auto & entry : std::filesystem::directory_iterator(directoryPath)) { //for each file in the directory
		std::ifstream file;
		file.open(entry); //open the file
		content << file.rdbuf(); //push into content variable
		linesVector.push_back(content.str()); //push onto the vector
	}

	return linesVector; //return vector
}

std::vector<std::string> FileManager::read(std::string directory, int key) {
	std::vector<std::string> retVect;
	std::string filePathName = directory;
	filePathName += "/file";
	filePathName += std::to_string(key);
	filePathName += ".txt";
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
	filePath += "/file" + std::to_string(key) + ".txt"; //update path with file named based on specific key of file


	std::ofstream file; //specifies directory to be opened in
	file.open(filePath, std::ios_base::app); //open file
	file << content; //write content into file
	file.close(); //close file
}

bool FileManager::init(std::string inDir, std::string tempDir, std::string outDir) {
	// check if input is a valid directory
	struct stat buffer;
	if (stat(inDir.c_str(), &buffer) != 0) {
		BOOST_LOG_TRIVIAL(error) << inDir << " directory does not exist!";
		return false;
	}
	else {
		// make new directories
		_mkdir(tempDir.c_str());
		_mkdir(outDir.c_str());
		// clean them in case they already exist
		for (const auto& entry : std::filesystem::directory_iterator(tempDir)) {
			std::filesystem::remove_all(entry.path());
		}
		for (const auto& entry : std::filesystem::directory_iterator(outDir)) {
			std::filesystem::remove_all(entry.path());
		}
		BOOST_LOG_TRIVIAL(info) << "FileSystem init success!";
		return true;
	}
}

void FileManager::printFinal(bool pass, std::string dir) {
	if (pass) {
		// print success message
		std::ofstream output(std::string(dir + "/SUCCESS"));
	}
	else {
		std::ofstream output(std::string(dir + "/FAILURE"));
	}
}
#include "framework.h"
#include "direct.h"
#include "Mapper.hpp"
#include <vector>
#include <string>
#include "../repo/FileManager.hpp"
#include "../repo/WinsockClient.cpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>
#include "boost/algorithm/string.hpp"
#include <thread>

std::atomic<bool> sendDoneMsg = false;

MAPPERDLL_API void map(std::string fileName) {
	sendDoneMsg = false;
	std::thread t([=] {
		std::ifstream file(fileName);
		std::stringstream str;
		str << file.rdbuf();
		std::string dirty = str.str();
		std::string cleaned;
		for (int i = 0; i < dirty.size(); i++) {
			if (dirty.at(i) < 0 || dirty.at(i) > 255) {
				continue;
			}
			cleaned += dirty.at(i);
		}
		// the vector to call export on
		std::vector<std::string> outVect;
		std::string temp;
		for (int i = 0; i < cleaned.length(); i++) {
			if (isspace(cleaned[i])) {
				// found end of a word. remove any punctuation and push it back
				temp.erase(std::remove_if(temp.begin(), temp.end(), ispunct), temp.end());
				// to lower case
				boost::to_lower(temp);
				outVect.push_back(temp);
				temp = "";
			}
			else {
				temp.push_back(cleaned[i]);
			}
		}
		temp.erase(std::remove_if(temp.begin(), temp.end(), ispunct), temp.end());
		boost::to_lower(temp);
		outVect.push_back(temp);
		exporter(outVect);
		sendDoneMsg = true;
		});
	t.detach();
}

MAPPERDLL_API void exporter(std::vector<std::string> vect) {
	std::string outStr;
	for (auto& itr : vect) {
		if (itr == "") {
			continue;
		}
		outStr += '\"' + itr + "\", 1\n";
	}
	// the key can be random here since everything is threaded out
	// and will eventually be merged all together
	FileManager::write(outStr, "temp", 1);
}

MAPPERDLL_API bool test() {
	// tests map and export
	try {
		std::string testStr = "test string test STRING\nTEST\nSTRING this is a test";
		_mkdir("test");
		std::ofstream outfile("test/test.txt");

		outfile << testStr << std::endl;

		outfile.close();
		map("test/test.txt");
		std::this_thread::sleep_for(std::chrono::seconds(3));
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
MAPPERDLL_API void heartbeat(int port) {
	// heartbeats are started before the mapper processes anything. Need to explicitely set doneMsg to false to reset unit tests
	sendDoneMsg = false;
	std::thread t([=]{
		while (true) {
			std::this_thread::sleep_for(std::chrono::seconds(3));
			WinsockClient client;
			if (sendDoneMsg) {
				client.publishMsg("mapper completed", port);
				sendDoneMsg = false;
			}
			else {
				client.publishMsg("good mapper status heartbeat", port);
			}
		}
		});
	t.detach();
	// sends a heartbeat message back to the controller
}
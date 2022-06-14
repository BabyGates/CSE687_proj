#include <chrono>
#include <thread>
#include <mutex>
#include <fstream>
#include <iostream>
#include <sstream>
#include "Workflow.hpp"
#include "FileManager.hpp"
#include "../MapperDLL/Mapper.hpp"
#include "WinsockClient.cpp"

std::atomic<int> keyCounter;
std::mutex mutex;
bool runMappers = false;
bool runReducers = false;
bool done = false;

bool Workflow::run(std::string inDir, std::string tempDir, std::string outDir, std::string dllDir) {

	BOOST_LOG_TRIVIAL(info) << "Starting workflow.";
	WinsockClient pub;
	int baseStubPort = BASE_STUB_PORT;
	std::vector<std::string> inVectFileNames = FileManager::readFileNames(inDir);
	for (int i = 0; i < inVectFileNames.size(); i++) {
		// create a stub and mapper for each file
		pub.publishMsg("create mapper", baseStubPort++);
	}
	// TODO
	pub.publishMsg("create reducer", REDUCER_STUB_PORT);
	while (!runMappers) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	// all mappers are ready to go!
	static Trie* trieHead = new Trie();
	WinsockClient client;
	try {
		for (int i = 0; i < inVectFileNames.size(); i++) {
			// for each file in the inDirectory
			client.publishMsg(inVectFileNames.at(i), BASE_STUB_PORT + i);
			// temp dir now has intermediate files in it. now read it back in
		}
		// wait here to kick off reducer until the mappers are all done
		while (!runReducers) {
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
		std::this_thread::sleep_for(std::chrono::seconds(3));
		client.publishMsg("start reducer", REDUCER_STUB_PORT);
	}
	catch (std::exception& e) {
		BOOST_LOG_TRIVIAL(info) << "ERROR: " << e.what();
	}
	while (!done) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	return true;
}

void Workflow::statusListener() {
	BOOST_LOG_TRIVIAL(info) << "Starting status listener";
	WinsockClient client;
	while (true) {
		std::string out = client.recvMsg(CONTROLLER_LISTENING_PORT);
		BOOST_LOG_TRIVIAL(info) << "Controller got stub status: " << out;
		if (std::strstr(out.c_str(), "good mapper status") != nullptr) {
			if (runMappers == false) {
				// workflow heard a valid heartbeat, start kick off run()
				runMappers = true;
			}
		}
		if (std::strstr(out.c_str(), "mapper completed") != nullptr) {
			std::this_thread::sleep_for(std::chrono::seconds(1));
			if (runReducers == false) {
				// workflow heard a valid heartbeat, start kick off run()
				runReducers = true;
			}
		}
		if (std::strstr(out.c_str(), "reducer completed") != nullptr) {
			// reducer is complete, return from run()
			std::this_thread::sleep_for(std::chrono::seconds(1));
			BOOST_LOG_TRIVIAL(info) << "Reducer has finished, exiting program!";
			done = true;
		}
	}
}

bool Workflow::test(std::string dllDir) {
	//
	// reducer dll -----------------
	//
	typedef bool (*funcReduce)();
	funcReduce reduce;
	{
		HINSTANCE hDLL;
		std::string dllPath = dllDir + "\\ReducerDLL";
		std::wstring wideStr = std::wstring(dllPath.begin(), dllPath.end());
		const wchar_t* libName = wideStr.c_str();
		hDLL = LoadLibraryEx(libName, NULL, NULL);   // Handle to DLL
		if (hDLL == NULL) {
			BOOST_LOG_TRIVIAL(error) << "Failed to load Reducer DLL!";
			return false;
		}
		reduce = (funcReduce)GetProcAddress(hDLL, "reduce");
	}
	//
	// tests sort, reduce, trie
	std::string testStr = "\"test\", 1\n\"string\", 1\n\"test\", 1";
	_mkdir("test");
	std::ofstream outfile("test/test.txt");

	outfile << testStr << std::endl;

	outfile.close();
	bool retVal = true;
	std::this_thread::sleep_for(std::chrono::seconds(3));
	return retVal;
}
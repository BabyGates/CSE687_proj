#include <chrono>
#include <thread>
#include <mutex>
#include "Workflow.hpp"
#include "FileManager.hpp"
#include "Mapper.hpp"

std::atomic<int> keyCounter;
std::mutex mutex;
typedef void (*funcMap)(int, std::string, std::string);
typedef bool (*funcReduce)(Trie*, std::string);

bool Workflow::run(std::string inDir, std::string tempDir, std::string outDir, std::string dllDir) {
	// load in dll's
	// mapper dll ------------------
	funcMap map;
	{
		HINSTANCE hDLL;
		std::string dllPath = dllDir + "\\MapperDLL";
		std::wstring wideStr = std::wstring(dllPath.begin(), dllPath.end());
		const wchar_t* libName = wideStr.c_str();
		hDLL = LoadLibraryEx(libName, NULL, NULL);   // Handle to DLL
		if (hDLL == NULL) {
			BOOST_LOG_TRIVIAL(error) << "Failed to load Map DLL!";
			return false;
		}
		map = (funcMap)GetProcAddress(hDLL, "map");
	}
	//
	// reducer dll -----------------
	//
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
	BOOST_LOG_TRIVIAL(info) << "Starting workflow.";
	std::vector<std::string> inVect = FileManager::read(inDir);
	std::vector<std::thread> threadVect;
	keyCounter = 0;
	static Trie* trieHead = new Trie();
	auto start = std::chrono::high_resolution_clock::now();
	try {
		for (int i = 0; i < inVect.size(); i++) {
			threadVect.emplace_back([=] {	
				// for each file in the inDirectory
				int threadKey = keyCounter++;
				map(threadKey, inVect.at(i), tempDir);
				BOOST_LOG_TRIVIAL(info) << "Wrote temp file" << threadKey;
				// temp dir now has intermediate files in it. now read it back in
				std::vector<std::string> tempVect = FileManager::read(tempDir, threadKey);
				sort(trieHead, tempVect);
			});
		}
		for (auto& thread : threadVect) {
			thread.join();
		}
		// trie is populated with string values. pump them out to the vector
		if (reduce(trieHead, outDir)) {
			BOOST_LOG_TRIVIAL(info) << "All files completed successfully!";
		}
		else {
			BOOST_LOG_TRIVIAL(info) << "Something bad happened in the trie. Exiting now.";
			return false;
		}
	}
	catch (std::exception& e) {
		BOOST_LOG_TRIVIAL(info) << "ERROR: " << e.what();
	}
	auto finish = std::chrono::high_resolution_clock::now();
	auto durationSec = std::chrono::duration_cast<std::chrono::seconds>(finish - start);
	BOOST_LOG_TRIVIAL(info) << "Workflow took: " << durationSec.count() << "s to complete.";
	return true;
}

void Workflow::sort(Trie* head, std::vector<std::string> inVect) {
	for (int i = 0; i < inVect.size(); i++) {
		std::string line = inVect.at(i);
		std::string::size_type start_pos = line.find("\"");
		std::string::size_type end_pos;
		if (start_pos != std::string::npos) {
			end_pos = line.find("\"", start_pos + 1);
			// get the word without quotes
			std::string word = line.substr(start_pos + 1, end_pos - 1);
			// populate the common trie. mutex lock to be thread safe
			mutex.lock();
			if (head->search(word) == 0) {
				// this is a new word
				head->insert(word);
			}
			else {
				// word already existed. increment the count
				head->increment(word);
			}
			mutex.unlock();
		}
		else {
			BOOST_LOG_TRIVIAL(error) << "Malformed temp directory found in sorted()";
		}
	}
}

bool Workflow::test(std::string dllDir) {
	//
	// reducer dll -----------------
	//
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
	bool retVal = false;
	_mkdir("test");
	Workflow wf;
	std::vector<std::string> tempVect;
	tempVect.push_back("\"test\", 1");
	tempVect.push_back("\"other\", 1");
	tempVect.push_back("\"a\", 1");
	tempVect.push_back("\"test\", 1");
	Trie* trieHead = new Trie();
	wf.sort(trieHead, tempVect);
	if (trieHead->search("test") == 0 ||
		trieHead->search("other") == 0 ||
		trieHead->search("a") == 0) {
		retVal = false;
		goto clean_up;
	}
	if (reduce(trieHead, "test")) {
		retVal = true;
	}
	else {
		retVal = false;
	}
clean_up:
	// clean up memory
	while (!trieHead->isLeaf) {
		trieHead->pop();
	}
	std::filesystem::remove_all("test");
	return retVal;
}
#include "Workflow.hpp"
#include "FileManager.hpp"
#include "Reducer.hpp"
#include "Mapper.hpp"

bool Workflow::run(std::string inDir, std::string tempDir, std::string outDir) {
	BOOST_LOG_TRIVIAL(info) << "Starting workflow.";
	
	int keyCounter = 0;
	// setup objects
	FileManager fileMgr;
	Mapper mapper;
	Reducer reducer(outDir);
	std::vector<std::string> inVect = fileMgr.read(inDir);
	try {
		for (int i = 0; i < inVect.size(); i++) {
			// for each file in the inDirectory
			mapper.map(keyCounter, inVect.at(i), tempDir);
			// temp dir now has intermediate files in it. now read it back in
			std::vector<std::string> tempVect = fileMgr.read(tempDir, keyCounter);
			Trie* trieHead = sort(tempVect);

			// trie is populated with string values. pump them out to the vector
			if (reducer.reduce(trieHead, outDir, keyCounter)) {
				BOOST_LOG_TRIVIAL(info) << "File: " << keyCounter << " SUCCESS";
			}
			else {
				BOOST_LOG_TRIVIAL(info) << "Something bad happened in the trie. Exiting now.";
				return false;
			}
			keyCounter++;
			tempVect.clear();
		}
	}
	catch (std::exception& e) {
		BOOST_LOG_TRIVIAL(info) << "ERROR: " << e.what();
	}
	return true;
}

Trie* Workflow::sort(std::vector<std::string> inVect) {
	std::vector<std::string> retVect;
	Trie* head = new Trie();

	for (int i = 0; i < inVect.size(); i++) {
		std::string line = inVect.at(i);
		std::string::size_type start_pos = line.find("\"");
		std::string::size_type end_pos;
		if (start_pos != std::string::npos) {
			end_pos = line.find("\"", start_pos + 1);
			// get the word without quotes
			std::string word = line.substr(start_pos + 1, end_pos - 1);
			// populate the trie
			if (head->search(word) == 0) {
				// this is a new word
				head->insert(word);
			}
			else {
				// word already existed. increment the count
				head->increment(word);
			}
		}
		else {
			BOOST_LOG_TRIVIAL(error) << "Malformed temp directory found in sorted()";
		}
	}
	return head;
}
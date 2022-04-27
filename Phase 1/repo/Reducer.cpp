#include "Reducer.hpp"
#include "FileManager.hpp"
#include "fstream"

Reducer::Reducer(std::string dir) {
	mOutDir = dir;
}

bool Reducer::reduce(Trie* head, std::string outPath, int key) {
	while (head->isLeaf == false) {
		// this cleans up the trie and pops off all the words and quantities
		std::tuple<std::string, int> val = head->pop();
		std::string outstring = '\"' + std::get<0>(val) + "\", " + std::to_string(std::get<1>(val)) + '\n';
		FileManager::write(outPath, key, outstring);
	}
	return true;
}
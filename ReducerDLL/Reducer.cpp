#include "Reducer.hpp"
#include "../repo/FileManager.hpp"
#include "fstream"

REDUCERDLL_API bool reduce(Trie* head, std::string path) {
	std::string outStr;
	while (head->isLeaf == false) {
		// this cleans up the trie and pops off all the words and quantities
		std::tuple<std::string, int> val = head->pop();
		outStr += '\"' + std::get<0>(val) + "\", " + std::to_string(std::get<1>(val)) + '\n';
	}
	FileManager::write(outStr, path, -1);
	return true;
}
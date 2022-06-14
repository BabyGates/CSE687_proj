#include <boost/log/trivial.hpp>
#include "Reducer.hpp"
#include "../repo/FileManager.hpp"
#include "fstream"
#include <iostream>
#include "../repo/WinsockClient.cpp"

std::atomic<bool> sendDoneMsg = false;
std::mutex mutx;

void sort(Trie* head, std::vector<std::string> inVect) {
	for (int i = 0; i < inVect.size(); i++) {
		std::string line = inVect.at(i);
		std::string::size_type start_pos = line.find("\"");
		std::string::size_type end_pos;
		if (start_pos != std::string::npos) {
			end_pos = line.find("\"", start_pos + 1);
			// get the word without quotes
			std::string word = line.substr(start_pos + 1, end_pos - 1);
			// populate the common trie. mutex lock to be thread safe
			mutx.lock();
			if (head->search(word) == 0) {
				// this is a new word
				head->insert(word);
			}
			else {
				// word already existed. increment the count
				head->increment(word);
			}
			mutx.unlock();
		}
		else {
			BOOST_LOG_TRIVIAL(error) << "Malformed temp directory found in sorted()";
		}
	}
}

// this serves and sort and reduce
REDUCERDLL_API bool reduce() {
	std::thread t([=] {
		// temp dir now has intermediate files in it. now read it back in
		static Trie* head = new Trie();
		std::vector<std::string> tempVect = FileManager::read("temp", 1);
		sort(head, tempVect);
		std::string outStr;
		while (head->isLeaf == false) {
			// this cleans up the trie and pops off all the words and quantities
			std::tuple<std::string, int> val = head->pop();
			outStr += '\"' + std::get<0>(val) + "\", " + std::to_string(std::get<1>(val)) + '\n';
		}
		FileManager::write(outStr, "../../output", -1);
		sendDoneMsg = true;
		});
	t.detach();
	return true;
}

REDUCERDLL_API void heartbeat(int port) {
	// heartbeats are started before the reducer processes anything. Need to explicitely set doneMsg to false to reset unit tests
	sendDoneMsg = false;
	std::thread t([=] {
		while (true) {
			std::this_thread::sleep_for(std::chrono::seconds(2));
			WinsockClient client;
			if (sendDoneMsg) {
				// this is okay to keep spamming, sometimes the udp messages get dropped.
				client.publishMsg("reducer completed", port);
			}
			else {
				client.publishMsg("good reducer status heartbeat", port);
			}
		}
		});
	t.detach();
	// sends a heartbeat message back to the controller
}
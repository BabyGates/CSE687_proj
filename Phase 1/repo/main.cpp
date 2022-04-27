#include "FileManager.hpp"
#include "Workflow.hpp"
#include <boost/test/unit_test.hpp>
int main(int argc, char* argv[]) {
	if (argc != 4) {
		BOOST_LOG_TRIVIAL(error) << "Malformed arguments! Please run <executable> <input dir> <temp dir> <output dir>";
		return -1;
	}
	std::string input = argv[1];
	std::string temp = argv[2];
	std::string output = argv[3];
	if (!FileManager::init(input, temp, output)) {
		BOOST_LOG_TRIVIAL(error) << "Filesystem init failed to complete!";
		return -1;
	}
	Workflow wf;
	// if checks pass, dirs are valid, pass them off to the workflow
	if (!wf.run(input, temp, output)) {
		BOOST_LOG_TRIVIAL(error) << "Workflow failed to complete!";
		FileManager::printFinal(false, output);
		return -1;
	}
	else {
		FileManager::printFinal(true, output);
	}
}
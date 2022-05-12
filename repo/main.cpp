#include "FileManager.hpp"
#include "Workflow.hpp"
#include "Tests.hpp"
#include <boost/log/trivial.hpp>

bool deploy = true;

int main(int argc, char* argv[]) {
	std::string input;
	std::string temp;
	std::string output;
	std::string dlls;
	if (deploy) {
		if (argc != 5) {
			BOOST_LOG_TRIVIAL(error) << "Malformed arguments! Please run <executable> <input dir> <temp dir> <output dir> <dll dir>";
			return -1;
		}
		input = argv[1];
		temp =argv[2];
		output = argv[3];
		dlls = argv[4];
	}
	else {
		input = "input";
		temp = "temp";
		output = "output";
		dlls = "x64/Debug";
	}
	if (!FileManager::init(input, temp, output, dlls)) {
		BOOST_LOG_TRIVIAL(error) << "Filesystem init failed to complete!";
		return -1;
	}
	BOOST_LOG_TRIVIAL(debug) << "Running unit tests...";
	if (Test::runTests(dlls)) {
		BOOST_LOG_TRIVIAL(debug) << "All unit tests passed!";
	}
	else {
		BOOST_LOG_TRIVIAL(warning) << "One or more unit tests failed!";
	}
	Workflow wf;
	// if checks pass, dirs are valid, pass them off to the workflow
	if (!wf.run(input, temp, output, dlls)) {
		BOOST_LOG_TRIVIAL(error) << "Workflow failed to complete!";
		FileManager::printFinal(false, output);
		return -1;
	}
	else {
		BOOST_LOG_TRIVIAL(info) << "Workflow completed successfully!";
		FileManager::printFinal(true, output);
	}
}
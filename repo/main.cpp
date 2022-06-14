#include "FileManager.hpp"
#include "Workflow.hpp"
#include "Tests.hpp"
#include "Stub.hpp"
#include <boost/log/trivial.hpp>
#include "WinsockServer.cpp"
#include "WinsockClient.cpp"
#include <csignal>

bool deploy = true;
#define TEMP_DIR "temp"
#define OUTPUT_DIR "../../output"
#define DLL_DIR "../../dlls"

int main(int argc, char* argv[]) {
	std::string input;
	std::string temp;
	std::string output;
	std::string dlls;
	if (deploy) {
		if (argc != 2) {
			BOOST_LOG_TRIVIAL(error) << "Malformed arguments! Please run <executable> <input dir>";
			return -1;
		}
		input = argv[1];
	}
	else {
		input = "input";
	}
	temp = TEMP_DIR;
	output = OUTPUT_DIR;
	dlls = DLL_DIR;
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
	BOOST_LOG_TRIVIAL(debug) << "------------------------------------------";
	std::this_thread::sleep_for(std::chrono::seconds(3));
	std::filesystem::remove_all("test");
	// hit another init to clear the files from the unit tests
	FileManager::init(input, temp, output, dlls);

	// create mapper stubs
	int externalPort = BASE_STUB_PORT;
	std::vector<std::string> inVectFileNames = FileManager::readFileNames(input);
	for (int i = 0; i < inVectFileNames.size(); i++) {
		int internalPort = externalPort++;
		Stub s(internalPort, dlls);
	}
	// create reducer stub
	Stub st(REDUCER_STUB_PORT, dlls);
	// allow the threads to start
	std::this_thread::sleep_for(std::chrono::seconds(2));
	// TODO test the stubs started

	auto start = std::chrono::high_resolution_clock::now();

	Workflow wf;
	std::thread t(&Workflow::statusListener, wf);
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

	auto finish = std::chrono::high_resolution_clock::now();
	auto durationSec = std::chrono::duration_cast<std::chrono::seconds>(finish - start);
	BOOST_LOG_TRIVIAL(info) << "Workflow took: " << durationSec.count() << "s to complete.";
	exit(0);
}
#include "Stub.hpp"
#include "WinsockClient.cpp"

Stub::Stub(int port, std::string dllDir) {
	std::thread t(&Stub::daemonLoop, this, port, dllDir);
	t.detach();
}

typedef void (*funcMap)(std::string);
typedef void (*funcMapHeartbeat)(int);
typedef bool (*funcReduce)();
typedef void (*funcReduceHeartbeat)(int);

void Stub::daemonLoop(int port, std::string dllDir) {
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
			return;
		}
		map = (funcMap)GetProcAddress(hDLL, "map");
	}
	funcMapHeartbeat mapHeartbeat;
	{
		HINSTANCE hDLL;
		std::string dllPath = dllDir + "\\MapperDLL";
		std::wstring wideStr = std::wstring(dllPath.begin(), dllPath.end());
		const wchar_t* libName = wideStr.c_str();
		hDLL = LoadLibraryEx(libName, NULL, NULL);   // Handle to DLL
		if (hDLL == NULL) {
			BOOST_LOG_TRIVIAL(error) << "Failed to load Map DLL!";
			return;
		}
		mapHeartbeat = (funcMapHeartbeat)GetProcAddress(hDLL, "heartbeat");
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
			return;
		}
		reduce = (funcReduce)GetProcAddress(hDLL, "reduce");
	}
	funcReduceHeartbeat reduceHeartbeat;
	{
		HINSTANCE hDLL;
		std::string dllPath = dllDir + "\\ReducerDLL";
		std::wstring wideStr = std::wstring(dllPath.begin(), dllPath.end());
		const wchar_t* libName = wideStr.c_str();
		hDLL = LoadLibraryEx(libName, NULL, NULL);   // Handle to DLL
		if (hDLL == NULL) {
			BOOST_LOG_TRIVIAL(error) << "Failed to load Reducer DLL!";
			return;
		}
		reduceHeartbeat = (funcReduceHeartbeat)GetProcAddress(hDLL, "heartbeat");
	}
	//

	BOOST_LOG_TRIVIAL(info) << "Creating stub on port: " << port;

	WinsockClient client;
	while (true) {
		std::string msg = client.recvMsg(port);
		BOOST_LOG_TRIVIAL (info) << "Stub heard message: " << msg << " on port: " << port << '\n';
		if (std::strstr(msg.c_str(), "create mapper") != nullptr) {

			// make a new mapper and detatch the threads
			// hard code 2 mappers per stub
			mapHeartbeat(CONTROLLER_LISTENING_PORT);
		}
		if (std::strstr(msg.c_str(), "create reducer") != nullptr) {
			// TODO make a new reducer
			reduceHeartbeat(CONTROLLER_LISTENING_PORT);
		}
		if (std::strstr(msg.c_str(), ".txt") != nullptr) {
			// got a path to map
			map(msg.c_str());
		}
		if (std::strstr(msg.c_str(), "start reducer") != nullptr) {
			// start the reducer with the pre defined temp file
			reduce();
		}
	}
}
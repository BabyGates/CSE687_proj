#include "Tests.hpp"


template <class T> bool Test::test(T actual, T expected, std::string testName) {
	if (expected == actual) {
		BOOST_LOG_TRIVIAL(debug) << "UNIT TEST: " << testName << " PASSED!";
		return true;
	}
	else {
		BOOST_LOG_TRIVIAL(warning) << "UNIT TEST: " << testName << " FAILED!";
		BOOST_LOG_TRIVIAL(warning) << "Expected: " << expected << " Actual: " << actual;
		return false;
	}
}
bool Test::runTests(std::string dllDir) {
	try {
		// list all the tests here
		// mapper dll------------------

		typedef bool (*funcMap)();
		// mapper dll ------------------
		funcMap mapTest;
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
			mapTest = (funcMap)GetProcAddress(hDLL, "test");
		}
		//
		if (
			test(0, 0, "Test class functional")
			&& test(FileManager::test(), true, "FileManager test")
			&& test(mapTest(), true, "Mapper test")
			&& test(Workflow::test(dllDir), true, "Workflow/Reducer/Trie test")
			) {
			return true;
		}
		else {
			return false;
		}
	}
	catch (std::exception& e) {
		BOOST_LOG_TRIVIAL(error) << "Unit tests crashed: " << e.what();
	}
}
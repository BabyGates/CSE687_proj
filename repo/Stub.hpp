#pragma once

#include "Workflow.hpp"

class Stub {
public:
	// true for reducer, false for mapper
	Stub(int port, std::string dlldir);
	void daemonLoop(int port, std::string dlldir);
};
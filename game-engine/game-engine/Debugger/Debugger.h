#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <string>
#include <iostream>
#include "../Logger/Logger.h"

class Debugger {
	public:
		static int debugLevel;
		static void UpdateDebugLevel(int argc, char* argv[]);
		static void Log(const std::string& msg);

	private:
		static void DisplayDebugLevel();
};

#endif
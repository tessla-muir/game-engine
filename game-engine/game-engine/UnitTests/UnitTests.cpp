#include "UnitTests.h"

int UnitTests::unitTest = 0;

void UnitTests::UpdateUnitTest(int argc, char* argv[]) {
	if (argc > 1) {
		// Get first argument
		std::string arg = argv[1];

		// See if has flag
		size_t pos = arg.find("/UT:");

		// Get the debug level if it has flag
		if (pos != std::string::npos) {
			try {
				if (unitTest >= 0 && unitTest <= 3) {
					unitTest = std::stoi(arg.substr(pos + 4));
					Logger::Debug("Running Unit Test " + std::to_string(unitTest));
				}
				else {
					Logger::Error("Debugger.cpp: Invalid Debugger Level");
				}
			}
			catch (const std::invalid_argument& e) {
				// Do nothing -- might be a unit test instaed
			}
		}
	}
}
#include "Debugger.h"

int Debugger::debugLevel = 0;

void Debugger::UpdateDebugLevel(int argc, char* argv[]) {
	if (argc > 1) {
		// Get first argument
		std::string arg = argv[1];

		// See if has flag
		size_t pos = arg.find("/GD:");
		
		// Get the debug level if it has flag
        if (pos != std::string::npos) {
            try {
				if (debugLevel >= 0 && debugLevel <= 9) {
					debugLevel = std::stoi(arg.substr(pos + 4));
					DisplayDebugLevel();
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

void Debugger::DisplayDebugLevel() {
	Logger::Debug("Debug level is " + std::to_string(debugLevel));

	switch (debugLevel) {
		case 1:
			Logger::Debug("Debugging ECS - Entities");
			break;
		case 2:
			Logger::Debug("Debugging ECS - Entites & Comopnents");
			break;
		case 3:
			Logger::Debug("Debugging Events");
			break;
		case 4:
			Logger::Debug("Debugging ProjectileDischarger");
			break;
		case 5:
			Logger::Debug("Debugging Collision System");
			break;
		case 6:
			Logger::Debug("Debugging Damage System");
			break;
		case 7:
			Logger::Debug("Debugging LifetimeSystem");
			break;
		case 8:
			Logger::Debug("Debugging KeyboardControlSystem");
			break;
		case 9:
			Logger::Debug("Debugging All");
			break;
	}
}

void Debugger::Log(const std::string& msg) {
	Logger::Debug(msg);
}
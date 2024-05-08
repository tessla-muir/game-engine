#ifndef UNITTESTS_H
#define UNITTESTS_H

#include <string>
#include <iostream>
#include "../Logger/Logger.h"

class UnitTests {
	public:
		static int unitTest;
		static void UpdateUnitTest(int argc, char* argv[]);
};

#endif
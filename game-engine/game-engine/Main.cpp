#include <iostream>
#include <SDL.h>
#include "Game.h"
#include "Debugger/Debugger.h"

int main(int argc, char* argv[]) {
	Game game;

	Debugger::UpdateDebugLevel(argc, argv);

	game.Initalize();
	game.Setup();
	game.Run();
	game.Destory();

	return 0;
}
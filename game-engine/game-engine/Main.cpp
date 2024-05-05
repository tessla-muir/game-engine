#include <iostream>
#include <SDL.h>
#include "Game.h"

int main(int argc, char* argv[]) {
	Game game;

	game.Initalize();
	game.Setup();
	game.Run();
	game.Destory();

	return 0;
}
#include "game.hpp"
#include "menustate.hpp"

int main() {
	//this is a test
	// create new game object
	// this object will hold the current gamestate and SFML objects
	Game game;

	// set the gamestate to the menu (represented by MenuState)
	game.setState(new MenuState(game));

	// tell the Game obj to run the main game loop
	game.run();

	return 0;
}
#include "game.hpp"
#include "state.hpp"

Game::Game():
	window(sf::VideoMode(800, 600), "CS179N Game"),
	curState(nullptr)
{}

Game::~Game() {
	window.close();
}

void Game::setState(State* newState) {
	curState = newState;
}

State* Game::getState() {
	return curState;
}

void Game::deleteState() {
	delete curState;
	curState = nullptr;
}

void Game::run() {
	// run while curState is not null
	for (;;) {
		// execute game logic
		curState->logic();

		// if curState == null, return
		if (!curState)
			return;

		// render the state
		curState->render();
	}
}
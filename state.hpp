#ifndef STATE_H_
#define STATE_H_

#include "game.hpp"

/* Abstract game state object
 * 
 * Game states define behavior for a given "scene" in the game
 * Ex: menu, story mode, endless mode, end screen, etc.
 * 
 * Game states load their resources on creation and free on deletion (RAII)
 * Changing game state can be done through Game::setState(State* newState)
 * 
 * To create a new state, create a child object of State and
 * define logic() and render() in that child
*/
class State {
public:
	State(Game& game): game(game) {};
	virtual ~State() {};

	/* defines the game logic of the gamestate */
	virtual void logic() = 0;

	/* defines how to render the gamestate */
	virtual void render() = 0;

protected:
	Game& game;
};

#endif
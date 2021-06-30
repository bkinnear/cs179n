#ifndef GAME_H_
#define GAME_H_

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/Audio.hpp>

class State;

/* Game object
 * contains SFML objects and handles the current gamestate
 * 
 * NOTE: this obj does NOT contain any assets or game logic
 * it is specifically for holding the SFML objects (window, renderer, etc.)
 * All game logic and rendering should be done in the current State obj (curState)
*/
class Game {
public:
	// initializes SFML
	Game();

	// dealloc SFML resources
	~Game();

	/* sets the current state to newState
	 * NOTE: does not automatically deallocate the old state. For that use deleteState() first
	 */
	void setState(State* newState);

	/* returns the current state
	 * Use this only for "saving" states for future use (i.e. returning to menu from settings)
	 */
	State* getState();

	/* deletes the current state and sets it to null
	 * This should be followed with setState()
	 */
	void deleteState();

	/* main game loop: runs the current state indefinitely */
	void run();

	// TODO: (low priority) we might want to consider implementing safe exiting for saving / loading

	/* SFML window and render target */
	sf::RenderWindow window;
private:
	/* current state being ran in run() */
	State* curState;
};

#endif
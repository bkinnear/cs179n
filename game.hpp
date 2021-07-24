#ifndef GAME_H_
#define GAME_H_

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/Audio.hpp>
#include <vector>

#include "mask.hpp"
#include "anim.hpp"

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
	 * this should only be used for quitting game
	 */
	void close();

	/* main game loop: runs the current state indefinitely */
	void run();

	// TODO: (low priority) we might want to consider implementing safe exiting for saving / loading

	/* SFML window and render target */
	sf::RenderWindow window;

	// set fullscreen mode
	void setFullscreen(bool fullscreen);

	// returns whether window is in fullscreen mode
	bool isFullscreen() const;

	// the default view port width
	float portWidth = 1366.f;
	// the default view port height
	float portHeight = 768.f;

	const std::string title;

private:
	/* current state being ran in run() */
	State* curState;

	bool fullscreen = true;
};

#endif
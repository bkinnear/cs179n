#ifndef GAME_H_
#define GAME_H_

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/Audio.hpp>
#include <vector>

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

/* Animated Sprite object
 * sprite index can be specified with setIndex
 * Parameters: texture, first (leftmost) subsprite source rect, number of subsprites
*/
class AnimSprite : public sf::Sprite {
public:
	AnimSprite() :
		Sprite(),
		subRect(0,0,0,0),
		curRect(0,0,0,0),
		nSubsprites(0),
		index(0)
	{};

	AnimSprite(sf::Texture& texture, sf::IntRect subRect, unsigned nSubsprites) :
		Sprite(texture),
		subRect(subRect),
		curRect(subRect),
		nSubsprites(nSubsprites),
		index(0)
	{};

	/* create animated sprite from texture, first subsprite rect, num of subsprites */
	void create(sf::Texture& texture, sf::IntRect subRect, unsigned nSubsprites) {
		setTexture(texture);
		this->subRect = subRect;
		this->curRect = subRect;
		this->nSubsprites = nSubsprites;
	}

	/* sets subsprite index to n
	 * if n >= nSubsprites, index = 0
	 */
	void setIndex(unsigned n) {
		if (n >= nSubsprites)
			index = 0;
		else
			index = n;
		updateSubsprite();
	};

	/* increments subsprite index by 1 
	 * loops back to 0 after index = nSubsprites
	 */
	void incIndex() {
		index = (index + 1) % nSubsprites;
		updateSubsprite();
	}

	/* returns subsprite index */
	unsigned getIndex() {
		return index;
	}

private:
	/* updates source rect as specified by subsprite index */
	void updateSubsprite() {
		curRect.left = subRect.width * index;
		setTextureRect(curRect);
	}

	// first subsprite src rect (index = 0)
	sf::IntRect subRect;
	
	// current subsprite src rect (index = n) 
	sf::IntRect curRect;

	// current subsprite index
	unsigned index;

	// number of subsprites in texture
	unsigned nSubsprites;
};

#endif
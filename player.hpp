#ifndef PLAYER_H_
#define PLAYER_H_

#include "game.hpp"
#include "state.hpp"

class Player: public AnimSprite {
public:

	bool movingLeft = false, movingUp = false, movingDown = false, movingRight = false;
	
	// sprite movement speed in px per frame
	int speed;
};

#endif
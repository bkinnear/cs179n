#ifndef PLAYER_H_
#define PLAYER_H_

#include "game.hpp"
#include "state.hpp"
#include "mask.hpp"

class Player: public AnimSprite {
public:	
	bool movingLeft = false, movingUp = false, movingDown = false, movingRight = false;
	
	// sprite movement speed in px per frame
	int speed;

	//player state
	int health = 100;
	bool alive = true;
};

#endif
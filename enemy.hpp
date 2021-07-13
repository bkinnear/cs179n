#ifndef ENEMY_H_
#define ENEMY_H_

#include "game.hpp"
#include "state.hpp"


class Enemy: public AnimSprite {
public:

	// sprite movement speed in px per frame
	int speed;//Endless Mode - 3 || Survival Mode = Dynamically Increasing
	int hitRate;//Endless Mode - 2 || Survival Mode = Dynamically increasing

	
};
#endif
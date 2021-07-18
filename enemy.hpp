#ifndef ENEMY_H_
#define ENEMY_H_

#include "game.hpp"
#include "state.hpp"
#include "mask.hpp"
#include <Windows.h>

class Enemy : public AnimSprite {
public:

	// sprite movement speed in px per frame
	int health = 100;
	int speed;//Endless Mode - 3 || Survival Mode = Dynamically Increasing from 3
	int hitRate;//Endless Mode - 15 || Survival Mode = Dynamically increasing from 15

	int attack = -1;

	void cooldown() {
		attack++;
		if (attack == 60) { // 60 frames/1 second, change the right-hand side to hitRate when hitRate is implemented
			attack = 0; //reset attack cooldown
		}
	}
};
#endif
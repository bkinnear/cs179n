#ifndef PLAYER_H_
#define PLAYER_H_

#include "character.hpp"
#include "classes.hpp"

class Player: public Character {
public:
	Player() : playerClass(PlayerClass::DEFAULT) {};
	Player(PlayerClass playerClass) : playerClass(playerClass) {};

	bool movingLeft = false, movingUp = false, movingDown = false, movingRight = false;
	bool lookingLeft = false, lookingRight = false;
	bool isDeadEye = false;
	bool isRage = false;
	bool isWarcry = false;
	bool isDash = false;

	const PlayerClass playerClass;
};

#endif
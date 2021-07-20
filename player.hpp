#ifndef PLAYER_H_
#define PLAYER_H_

#include "character.hpp"

class Player: public Character {
public:	
	bool movingLeft = false, movingUp = false, movingDown = false, movingRight = false;
};

#endif
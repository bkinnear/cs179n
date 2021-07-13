#ifndef PROJECTILE_H_
#define PROJECTILE_H_

#include "game.hpp"
#include "state.hpp"


class Projectile : public AnimSprite {
public:

	bool shoot = false;

	int speed = 15;
	int damage;
};

#endif
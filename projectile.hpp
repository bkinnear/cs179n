#ifndef PROJECTILE_H_
#define PROJECTILE_H_

#include "game.hpp"
#include "state.hpp"


class Projectile : public MSprite {
public:

	bool shoot = false;

	int speed = 15;
	float direction;
	int damage;
};

#endif
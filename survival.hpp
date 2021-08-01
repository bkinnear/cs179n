#ifndef SURVIVAL_H
#define SURVIVAL_H

#include "game.hpp"
#include "state.hpp"
#include "player.hpp"
#include "inventory.hpp"
#include "tilemap.hpp"
#include "projectile.hpp"
#include "enemy.hpp"
#include "mask.hpp"
#include "gamemode.hpp"

#include<list>

class SurvivalState : public GameMode {
public:
	SurvivalState(Game&, PlayerClass);
	~SurvivalState();
	

private:

	GameMode gameMode;

	int temp;
};

#endif
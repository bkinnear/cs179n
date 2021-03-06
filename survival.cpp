#include "survival.hpp"
#include "utils.hpp"

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <time.h>

// NOTE: we must call the original constructor and pass it the Game pointer
SurvivalState::SurvivalState(Game& game, PlayerClass playerClass) :
	GameMode(MODE_SURVIVAL, game, playerClass, gameMeta, npcSaveMeta, enemySaveMeta, inventorySaveMeta, false)
{}


SurvivalState::~SurvivalState() {
	// here we would deallocate any resources we use in this gamestate
}
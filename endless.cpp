#include "endless.hpp"
#include "utils.hpp"

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <time.h>


// NOTE: we must call the original constructor and pass it the Game pointer
EndlessState::EndlessState(Game& game, PlayerClass playerClass) :
	GameMode(1, game, playerClass)
{}

EndlessState::~EndlessState() {
	// here we would deallocate any resources we use in this gamestate
}


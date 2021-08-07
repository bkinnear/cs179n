#include "endless.hpp"

// the main game window
#define gwindow game.window

// NOTE: we must call the original constructor and pass it the Game pointer
EndlessState::EndlessState(Game& game, PlayerClass playerClass) :
	GameMode(1, game, playerClass, gameMeta, false)
{}

EndlessState::~EndlessState() {
}
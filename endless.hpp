#ifndef ENDLESS_H_
#define ENDLESS_H_

#include "gamemode.hpp"

class EndlessState : public GameMode {
public:
	EndlessState(Game&, PlayerClass);
	~EndlessState();
};

#endif
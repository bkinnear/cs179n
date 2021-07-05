#ifndef ENDLESS_H_
#define ENDLESS_H_

#include "game.hpp"
#include "state.hpp"
#include "player.hpp"

class EndlessState : public State {
public:
	EndlessState(Game&);
	~EndlessState();

	virtual void logic();
	virtual void render();

private:
	sf::View view;

	/* ==v== place any vars we need in this state in here ==v== */

	Player player;
	sf::Texture& texPlayerRight;
	sf::Texture& texPlayerLeft;
};

#endif
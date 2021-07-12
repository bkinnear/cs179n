#ifndef ENDLESS_H_
#define ENDLESS_H_

#include "game.hpp"
#include "state.hpp"
#include "player.hpp"
#include "tilemap.hpp"
#include "projectile.hpp"

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

	Projectile projectile;

	std::vector<Projectile>projectiles;
	
	TileMap tileMap;

	sf::Texture& texPlayerRight;
	sf::Texture& texPlayerLeft;

	sf::Texture& texProjectile;
};

#endif
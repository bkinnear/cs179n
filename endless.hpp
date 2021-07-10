#ifndef ENDLESS_H_
#define ENDLESS_H_

#include "game.hpp"
#include "state.hpp"
#include "player.hpp"
#include "tilemap.hpp"
#include "enemy.hpp"

#include<list>

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
	std::list<Enemy> enemies;
	const int totalEnemyCount = 60;
	int currentLevelEnemyCount = 3;//Default count

	int currentLevel = 1;//Starting Level
	TileMap tileMap;

	sf::Texture& texPlayerRight;
	sf::Texture& texPlayerLeft;

	sf::Texture& texEnemyRight;
	sf::Texture& texEnemyLeft;
};

#endif
#ifndef ENDLESS_H_
#define ENDLESS_H_

#include "game.hpp"
#include "state.hpp"
#include "player.hpp"
#include "inventory.hpp"
#include "tilemap.hpp"
#include "projectile.hpp"
#include "enemy.hpp"

#include<list>

class EndlessState : public State {
public:
	EndlessState(Game&);
	~EndlessState();

	virtual void logic();
	virtual void render();

private:
	sf::View mainView;
	sf::View guiView;

	/* ==v== place any vars we need in this state in here ==v== */


	// player
	Player player;

	// projectiles
	std::list<Projectile> projectiles;
  
	// inventory & inventory GUI
	Inventory inventory;
	bool showInventory = false;
	bool showItemDetails = false;
	sf::Text txtItemDetails;
	sf::RectangleShape shpItemDetails;

	// enemies
	std::list<Enemy> enemies;
	const int totalEnemyCount = 60;
	int currentLevelEnemyCount = 3;//Default count
	int currentLevel = 1;//Starting Level

	//spawned weapons
	std::list<std::pair<Item::type, sf::Sprite>> itemsOnMap;

	// tile map
	TileMap tileMap;

	// textures
	sf::Texture& texPlayerRight;
	sf::Texture& texPlayerLeft;
	sf::Texture& texProjectile;
	sf::Texture& texEnemyRight;
	sf::Texture& texEnemyLeft;
  
  // fonts
	sf::Font font;
};

#endif
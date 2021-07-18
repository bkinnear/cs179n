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

	void spawnEnemies(int);
	void renderEnemies(int);

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
	int maximumEnemyCount = 99;
	int defaultEnemySpawningCount = 3;//Default count

	//spawned weapons
	std::list<AnimSprite> weapons;

	// tile map
	TileMap tileMap;

	// textures
	sf::Texture& texPlayerRight;
	sf::Texture& texPlayerLeft;
	sf::Texture& texProjectile;
	sf::Texture& texEnemyRight;
	sf::Texture& texEnemyLeft;
	sf::Texture& texWeaponMP5;
  
  // fonts
	sf::Font font;
};

#endif
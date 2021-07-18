#ifndef SURVIVAL_H
#define SURVIVAL_H

#include "game.hpp"
#include "state.hpp"
#include "player.hpp"
#include "inventory.hpp"
#include "tilemap.hpp"
#include "projectile.hpp"
#include "enemy.hpp"

#include<list>

class SurvivalState : public State {
public:
	SurvivalState(Game&);
	~SurvivalState();

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
	int maxLevelCount = 20;
	int currentLevel = 1;//Starting Level
	int currentEnemySpawningCount = 3;//Default count for Level One
	int currentEnemyPresent = 3;//To keep track of enemies present in the game - Level Movement

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
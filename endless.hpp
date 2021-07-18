#ifndef ENDLESS_H_
#define ENDLESS_H_

#include "game.hpp"
#include "state.hpp"
#include "player.hpp"
#include "inventory.hpp"
#include "tilemap.hpp"
#include "projectile.hpp"
#include "enemy.hpp"
#include "npc.hpp"
#include "mask.hpp"

#include <list>

class EndlessState : public State {
public:
	EndlessState(Game&);
	~EndlessState();

	virtual void logic();
	virtual void render();

	// returns false if state exits
	bool handleEvents();

	void spawnEnemies(int);
	void updateEnemies();
	void renderEnemies();

	void updateAllies();
	void renderAllies();

	void updateProjectiles();

private:
	sf::View mainView;
	sf::View guiView;

	// mouse position in window (updated every logic tick)
	sf::Vector2i winMousePos;
	// mouse position in world (updated every logic tick)
	sf::Vector2f mousePos;

	/* ==v== place any vars we need in this state in here ==v== */

	// player
	Player player;

	// projectiles
	std::list<Projectile> projectiles;
  
	// inventory
	Inventory inventory;
	bool showInventory = false;
	bool showItemDetails = false;
	sf::Text txtItemDetails;
	sf::RectangleShape shpItemDetails;

	// allies
	std::list<NPC> allies;

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
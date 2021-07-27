#ifndef SURVIVAL_H
#define SURVIVAL_H

#include "game.hpp"
#include "state.hpp"
#include "player.hpp"
#include "inventory.hpp"
#include "tilemap.hpp"
#include "projectile.hpp"
#include "enemy.hpp"
#include "mask.hpp"
#include "gamemode.hpp"

#include<list>

class SurvivalState : public State {
public:
	SurvivalState(Game&, PlayerClass);
	~SurvivalState();

	virtual void logic();
	virtual void render();

	// returns false if state exits
	bool handleEvents();

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

	//PlayerGUI
	sf::RectangleShape playerHPBar;
	sf::RectangleShape playerHPBack;
	sf::Sprite playerIcon;
	sf::Sprite ammoIcon;
	sf::Text ammoCount;
	sf::Sprite grenadeIcon;
	sf::Text grenadesNum;

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

		//spawned weapons
	std::list<std::pair<Item::type, sf::Sprite>> itemsOnMap;
	
	// tile map
	TileMap tileMap;
	sf::Text pressToOpen;

	// textures
	sf::Texture& texPlayerRight;
	sf::Texture& texPlayerLeft;
	sf::Texture& texProjectile;
	sf::Texture& texEnemyRight;
	sf::Texture& texEnemyLeft;
	sf::Texture& texWeaponMP5;
	sf::Texture& texExplosionSmall;
	sf::Texture& playerDeath;
	// fonts
	sf::Font font;

	// effects
	Effect* explosionSmall;
};

#endif
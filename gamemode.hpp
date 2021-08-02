#ifndef GAMEMODE_H
#define GAMEMODE_H

#include "enemy.hpp"
#include "tilemap.hpp"
#include "inventory.hpp"
#include "npc.hpp"
#include "utils.hpp"
#include "player.hpp"
#include "game.hpp"
#include "state.hpp"
#include "projectile.hpp"

#include <list>

class GameMode : public State {

public:
	GameMode(int, Game&, PlayerClass);
	~GameMode();

	virtual void logic();
	virtual void render();

	void spawnEnemies(int);
	void spawnWeapons();
	void updateEnemies(int);
	void renderEnemies();

	bool handleEvents();

	void updateProjectiles();
	
	void renderAllies();
	void updateAllies();

private:
	
	int type; //1 - Endless, 2 - Survival

	// main view target (what it moves towards)
	sf::Vector2f mainViewTarget;

	// mouse position in window (updated every logic tick)
	sf::Vector2i winMousePos;
	// mouse position in world (updated every logic tick)
	sf::Vector2f mousePos;


	// views
	sf::View mainView;
	sf::View guiView;

	// player
	Player player;

	// tile map
	TileMap tileMap;

	// textures
	sf::Texture& texPlayerRight;
	sf::Texture& texPlayerLeft;
	sf::Texture& texAllyRight;
	sf::Texture& texAllyLeft;
	sf::Texture& texProjectile;
	sf::Texture& texEnemyRight;
	sf::Texture& texEnemyLeft;
	sf::Texture& texWeaponMP5;
	sf::Texture& texExplosionSmall;
	sf::Texture& texExplosionLarge;
	sf::Texture& playerDeath;

	// Music
	sf::Music music;
	sf::Music ambientZombie;

	// projectiles
	std::list<Projectile> projectiles;
	sf::SoundBuffer gunShotBuffer;
	sf::SoundBuffer emptyGunBuffer;
	sf::Sound shotSound;

	// enemies
	std::list<Enemy> enemies;
	sf::SoundBuffer zombieBuffer1;
	sf::SoundBuffer zombieBuffer2;
	sf::SoundBuffer zombieBuffer3;
	sf::SoundBuffer zombieBuffer4;
	sf::SoundBuffer zombieBuffer5;
	sf::Sound zombieSound;

	// FPS counter
	std::vector<float> fpsTimes;
	sf::Clock fpsTimer;
	sf::Clock fpsClock;
	sf::Text fpsCounter;

	// fonts
	sf::Font font;

	// effects
	Effect* explosionSmall;
	Effect* explosionLarge;

	// interactions
	sf::SoundBuffer doorOpen;
	sf::SoundBuffer doorClose;
	sf::Sound doorInteract;

	// dialog GUI
	int dialogTreeIndex = 0;
	void setDialog(const std::string&, const std::string&);
	void hideDialog();
	bool showDialog = false;
	sf::RectangleShape dialogBox1;
	sf::RectangleShape dialogBox2;
	sf::Text dialogSpeaker;
	sf::Text dialogMessage;

	//PlayerGUI
	sf::RectangleShape playerHPBar;
	sf::RectangleShape playerHPBack;
	sf::Sprite playerIcon;
	sf::Sprite ammoIcon;
	sf::Text ammoCount;
	sf::Sprite grenadeIcon;
	sf::Text grenadesNum;

	// inventory
	Inventory inventory;
	bool showInventory = false;
	bool showItemDetails = false;
	sf::Text txtItemDetails;
	sf::RectangleShape shpItemDetails;

	// allies
	std::list<NPC> allies;

	//spawned weapons
	std::list<std::pair<Item::type, sf::Sprite>> itemsOnMap;

	//Endless
	int maximumEnemyCount = 99;
	int defaultEnemySpawningCount = 3;//Default count

	//Survival
	int maxLevelCount = 20;
	int currentLevel = 1;//Starting Level
	int currentEnemySpawningCount = 3;//Default count for Level One
	int currentEnemyPresent = 3;//To keep track of enemies present in the game - Level Movement

	// ability variables
	sf::Time elapsed1;
	sf::Clock abilityTimer1;
	bool onCoolDown1 = false;
	int cooldown1;

	sf::Time elapsed2;
	sf::Clock abilityTimer2;
	bool onCoolDown2 = false;
	int cooldown2;

	sf::Time elapsed3;
	sf::Clock abilityTimer3;
	bool onCoolDown3 = false;
	int cooldown3;


	void chooseClass(PlayerClass);
	void updateCooldowns();


	// medic abilities
	void medic_bandage();
	void medic_dash();
	void medic_heal();

	// assault abilities
	void assault_grenade();
	void assault_ammo();
	void assault_deadeye();

};
#endif
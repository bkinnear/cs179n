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
#include "classes.hpp"

#include <list>
#include <unordered_map>

class EndlessState : public State {
public:
	EndlessState(Game&, PlayerClass);
	~EndlessState();

	virtual void logic();
	virtual void render();

	// returns false if state exits
	bool handleEvents();

	void chooseClass(PlayerClass);

	void spawnWeapons();

	void spawnEnemies(int);
	void updateEnemies();
	void renderEnemies();

	void updateAllies();
	void renderAllies();

	void updateProjectiles();

private:
	// views
	sf::View mainView;
	sf::View guiView;

	// main view target (what it moves towards)
	sf::Vector2f mainViewTarget;

	// mouse position in window (updated every logic tick)
	sf::Vector2i winMousePos;
	// mouse position in world (updated every logic tick)
	sf::Vector2f mousePos;

	/* ==v== place any vars we need in this state in here ==v== */

	// Music
	sf::Music music;
	sf::Music ambientZombie;

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

	// dialog GUI
	int dialogTreeIndex = 0;
	void setDialog(const std::string&, const std::string&);
	void hideDialog();
	bool showDialog = false;
	sf::RectangleShape dialogBox1;
	sf::RectangleShape dialogBox2;
	sf::Text dialogSpeaker;
	sf::Text dialogMessage;

	// FPS counter
	std::vector<float> fpsTimes;
	sf::Clock fpsTimer;
	sf::Clock fpsClock;
	sf::Text fpsCounter;

	// projectiles
	std::list<Projectile> projectiles;
	sf::SoundBuffer gunShotBuffer;
	sf::SoundBuffer emptyGunBuffer;
	sf::Sound shotSound;
  
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
	sf::SoundBuffer zombieBuffer1;
	sf::SoundBuffer zombieBuffer2;
	sf::SoundBuffer zombieBuffer3;
	sf::SoundBuffer zombieBuffer4;
	sf::SoundBuffer zombieBuffer5;
	sf::Sound zombieSound;
	int maximumEnemyCount = 99;
	int defaultEnemySpawningCount = 3;//Default count

	// items
	// items in world
	std::list<std::pair<Item::type, sf::Sprite>> itemsOnMap;
	// spatial hash for itemsOnMap (key = int, value = list of list iterators)
	std::unordered_map <
		int,
		std::list<
			std::list<std::pair<Item::type, sf::Sprite>>::iterator
		>
	> itemHash;
	// gets hash key from position on map
	int hashPos(const sf::Vector2i& pos) const;
	// creates item at position on the map
	sf::Sprite& createItem(const sf::Vector2i& pos, Item::type type);
	// returns first item found in tile at map position
	Item::type getItem(const sf::Vector2i& pos) const;

	// tile map
	TileMap tileMap;
	sf::Text pressToOpen;

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
  
	// fonts
	sf::Font font;

	// effects
	Effect* explosionSmall;
	Effect* explosionLarge;

	// interactions
	sf::SoundBuffer doorOpen;
	sf::SoundBuffer doorClose;
	sf::Sound doorInteract;

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
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

// iterator for item on map
typedef std::list<std::pair<Item::type, sf::Sprite>>::iterator ItemIterator;

class GameMode : public State {

protected:
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
	sf::Texture& texPlayerRightMp5;
	sf::Texture& texPlayerLeftMp5;
	sf::Texture& texPlayerRightM4;
	sf::Texture& texPlayerLeftM4;
	sf::Texture& texAllyRight;
	sf::Texture& texAllyLeft;
	sf::Texture& texDummyRight;
	sf::Texture& texBarrel;
	sf::Texture& texShield;
	sf::Texture& texProjectile;
	sf::Texture& texEnemyRight;
	sf::Texture& texEnemyLeft;
	sf::Texture& texWeaponMP5;
	sf::Texture& texExplosionSmall;
	sf::Texture& texExplosionLarge;
	sf::Texture& texDeadEyeOpen;
	sf::Texture& texDeadEyeClose;
	sf::Texture& playerDeath;

	// Music
	sf::Music music;
	sf::Music ambientZombie;

	// projectiles
	std::list<Projectile> projectiles;
	sf::SoundBuffer gunShotBuffer;
	sf::SoundBuffer emptyGunBuffer;
	sf::SoundBuffer mp5ReloadBuffer;
	sf::SoundBuffer grenadeShotBuffer;
	sf::SoundBuffer grenadeExplodeBuffer;
	sf::SoundBuffer meleeSoundBuffer;
	sf::SoundBuffer meleeSwingBuffer;
	sf::Sound reloadSound;
	sf::Sound grenadeSound;
	sf::Sound shotSound;
	sf::Sound meleeSound;
	sf::Sound meleeSwing;

	// enemies
	std::list<Enemy> enemies;
	sf::SoundBuffer zombieBuffer1;
	sf::SoundBuffer zombieBuffer2;
	sf::SoundBuffer zombieBuffer3;
	sf::SoundBuffer zombieBuffer4;
	sf::SoundBuffer zombieBuffer5;
	sf::Sound zombieSound;

	// FPS counter
	unsigned fpsTick;
	sf::Clock fpsClock;
	sf::Text fpsCounter;

	// fonts
	sf::Font font;

	// effects
	Effect* explosionSmall;
	Effect* explosionLarge;
	Effect* deadEyeOpen;
	Effect* deadEyeClose;

	// interactions
	sf::SoundBuffer doorOpen;
	sf::SoundBuffer doorClose;
	sf::Sound doorInteract;

	// engineer sounds
	sf::SoundBuffer mechNoise;
	sf::SoundBuffer het_hon;
	sf::Sound dropTech;

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
	sf::Sprite reticle;

	// inventory
	Inventory inventory;
	bool showInventory = false;
	bool showItemDetails = false;
	sf::Text txtItemDetails;
	sf::RectangleShape shpItemDetails;

	// allies
	std::list<NPC> allies;


	// items
	// items in world
	std::list<std::pair<Item::type, sf::Sprite>> itemsOnMap;
	// spatial hash for itemsOnMap (key = int, value = list of list iterators)
	std::unordered_map<int, std::list<ItemIterator>> itemHash;
	// gets hash key from position on map
	int hashPos(const sf::Vector2f& pos) const;
	// creates item at position on the map
	ItemIterator createItem(const sf::Vector2f& pos, Item::type type);
	// removes item at position on the map
	void removeItem(ItemIterator);
	// returns first item found in tile at map position (if no item found returns itemsOnMap.end())
	ItemIterator getItem(const sf::Vector2f& pos);

	//Endless
	int maximumEnemyCount = 99;
	int defaultEnemySpawningCount = 10;//Default count

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

	// engineer abilities
	void engineer_decoy();
	void engineer_barrel();
	void engineer_shield();

	// slasher abilities
	void slasher_smash();
	void slasher_warcry();
	void slasher_rage();

	bool debugging = false;
	bool isLoadInvoked = false;

	struct GameMeta
	{
		struct EndlessMeta
		{
			int playerPosX = 0;
			int playerPosY = 0;
			//std::vector<std::vector<Tile>> currentMap;
		}endlessMeta;

		struct SurvivalMeta
		{
			int currentLevel = 1;
			int playerPosX = 0;
			int playerPosY = 0;
			//std::vector<std::vector<Tile>> currentMap;
		}survivalMeta;
	}gameMeta;

	bool gamestateChange = false;
	std::string metaFileName = "thelastwar.dat";
	
	void loadGame();
	void saveGame();
	void initGame();

	void spawnEnemies(int);
	void respawnEnemies();
	void spawnItems();
	void updateEnemies(int);
	void renderEnemies();
	bool handleEvents();
	void updateProjectiles();
	void renderAllies();
	void updateAllies();

	std::list<sf::FloatRect> hiddenAreas;
	std::list<sf::Vector2f> lootSpawnPoints; // TODO create loot spawn points in TileMap::generate that determine where items spawns / what items

private:
	int type; //1 - Endless, 2 - Survival

public:

	GameMode(int, Game&, PlayerClass, GameMeta, bool);
	~GameMode();

	virtual void logic();
	virtual void render();

	void addHiddenArea(const sf::FloatRect&);
};
#endif
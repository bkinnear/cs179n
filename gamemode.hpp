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

// item on map
struct ItemSpr {
	Item::type type;
	sf::Sprite spr;
};

class GameMode : public State {

protected:
	// main view target (what it moves towards)
	sf::Vector2f mainViewTarget;

	// mouse position in window (updated every logic tick)
	sf::Vector2i winMousePos;
	// mouse position in world (updated every logic tick)
	sf::Vector2f mousePos;

	// shader
	sf::Shader shader;

	// views
	sf::View mainView;
	sf::View guiView;

	// player
	Player player;

	// tile map
	TileMap tileMap;

	// intro for game start
	sf::RectangleShape introShape;
	sf::Clock introClock;
	sf::Text introMessage;

	// textures
	sf::Texture& texPlayerRight;
	sf::Texture& texPlayerLeft;
	sf::Texture& texPlayerRightMp5;
	sf::Texture& texPlayerLeftMp5;
	sf::Texture& texPlayerRightM4;
	sf::Texture& texPlayerLeftM4;
	sf::Texture& texPlayerRightM240;
	sf::Texture& texPlayerLeftM240;
	sf::Texture& texPlayerRightShotgun;
	sf::Texture& texPlayerLeftShotgun;
	sf::Texture& texPlayerRightM9;
	sf::Texture& texPlayerLeftM9;
	sf::Texture& texAllyRight;
	sf::Texture& texAllyLeft;
	sf::Texture& texDummyRight;
	sf::Texture& texBarrel;
	sf::Texture& texShield;
	sf::Texture& texProjectile;
	sf::Texture& texEnemyRight;
	sf::Texture& texEnemyLeft;
	sf::Texture& texWeaponMP5;
	sf::Texture& texHealAnimation;
	sf::Texture& texExplosionSmall;
	sf::Texture& texExplosionLarge;
	sf::Texture& texDeadEyeOpen;
	sf::Texture& texDeadEyeClose;
	sf::Texture& texDeadEye;
	sf::Texture& playerDeath;
	sf::Texture& texGuardianAngel;
	sf::Texture& texGuardianWings;
	sf::Texture& texWarcry;
	sf::Texture& texRage;
	sf::Texture& texDash;
	sf::Texture& texBloodSplatter1;
	sf::Texture& texBloodSplatter2;
	sf::Texture& texBloodSplatter3;
	sf::Texture& texBloodSplatter4;
	sf::Texture& texBloodSplatter5;
	sf::Texture& texSmash;

	// Music
	sf::Music music;
	sf::Music ambientZombie;

	// projectiles
	std::list<Projectile> projectiles;
	sf::SoundBuffer gunShotBuffer;
	sf::SoundBuffer heavyGunShotBuffer;
	sf::SoundBuffer emptyGunBuffer;
	sf::SoundBuffer mp5ReloadBuffer;
	sf::SoundBuffer m4ReloadBuffer;
	sf::SoundBuffer ShotgunReloadBuffer;
	sf::SoundBuffer m240ReloadBuffer;
	sf::SoundBuffer pistolReloadBuffer;
	sf::SoundBuffer grenadeShotBuffer;
	sf::SoundBuffer ShotgunShotBuffer;
	sf::SoundBuffer grenadeExplodeBuffer;
	sf::SoundBuffer meleeSoundBuffer;
	sf::SoundBuffer meleeSwingBuffer;
	sf::Sound reloadSound;
	sf::Sound grenadeSound;
	sf::Sound shotSound;
	sf::Sound ShotgunSound;
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
	sf::Sprite siegingIcon;

	// FPS counter
	unsigned fpsTick;
	sf::Clock fpsClock;
	sf::Text fpsCounter;

	// fonts
	sf::Font font;
	sf::Font font2;

	// effects
	EffectID explosionSmall;
	EffectID explosionLarge;
	EffectID bloodSplatter;
	int bloodEffect;
	sf::Vector2f bloodLocation;
	EffectID deadEyeOpen;
	EffectID deadEyeClose;
	AnimSprite warcryFX;
	AnimSprite rageFX;
	AnimSprite healingFX;
	AnimSprite guardianAngelFX;
	AnimSprite guardianWingsFX;
	AnimSprite deadEyeFX;
	AnimSprite dashFX;
	EffectID smashFX;

	// interactions
	sf::SoundBuffer doorOpen;
	sf::SoundBuffer doorClose;
	sf::Sound doorInteract;

	// engineer sounds
	sf::SoundBuffer mechNoise;
	sf::SoundBuffer het_hon;
	sf::SoundBuffer metalBox;
	sf::SoundBuffer barrelBuffer;
	sf::Sound dropTech;

	// medic sounds
	sf::SoundBuffer medkitBuffer;
	sf::SoundBuffer dashBuffer;
	sf::SoundBuffer guardianAngelBuffer;
	sf::Sound medkitSound;
	sf::Sound dashSound;
	sf::Sound guardianAngelSound;

	// slasher sounds
	sf::SoundBuffer smashBuffer;
	sf::SoundBuffer rageBuffer;
	sf::SoundBuffer warcryBuffer;
	sf::Sound rageSound;
	sf::Sound warcrySound;
	sf::Sound smashSound;

	// powerup sound
	sf::SoundBuffer powerupBuffer;
	sf::Sound powerUp;

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
	sf::Sprite abilityIcon1;
	sf::Sprite abilityIcon2;
	sf::Sprite abilityIcon3;
	sf::Text abilityClock1;
	sf::Text abilityClock2;
	sf::Text abilityClock3;

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
	std::list<ItemSpr*> itemsOnMap;
	// spatial hash for itemsOnMap (key = int, value = list of list iterators)
	std::unordered_map<int, std::list<ItemSpr*>> itemHash;
	// gets hash key from position on map
	int hashPos(const sf::Vector2f& pos) const;
	// creates item at position on the map
	ItemSpr* createItem(const sf::Vector2f& pos, Item::type type);
	// removes item at position on the map
	void removeItem(ItemSpr*);
	// returns pointer to item sprite at map position (nullptr if no item found)
	ItemSpr* getItemAt(const sf::Vector2f& pos);

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

	// animation timers
	sf::Time healElapsed;
	sf::Clock healTimer;
	bool healPlaying = false;

	sf::Time guardianElapsed;
	sf::Clock guardianTimer;
	sf::Time wingsElapsed;
	sf::Clock wingsTimer;
	bool guardianPlaying = false;
	
	// animation timer durations
	int healDuration = 1;
	int guardianDuration = 1;

	// class utiity functions
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

	struct NPCMeta
	{
		int health = 0;
		int positionX = 0;
		int positionY = 0;
	}npcSaveMeta[2];

	struct EnemyMeta
	{
		int health = 0;
		int positionX = 0;
		int positionY = 0;
	}enemySaveMeta[42];

	struct InventoryMeta
	{
		int itemNumber = 0;
		int count = 0;
	}inventorySaveMeta[15];
	struct GameMeta
	{
		struct EndlessMeta
		{
			int playerPosX = 0;
			int playerPosY = 0;
			//std::vector<std::vector<Tile>> currentMap;
			int maxScore = 0;
			int currentScore = 0;
			int playerHealth = 100;
			int currentWieldedWeapon = 0;
			int roundsLeft = 0;
		}endlessMeta;

		struct SurvivalMeta
		{
			int currentLevel = 1;
			int playerPosX = 0;
			int playerPosY = 0;
			//std::vector<std::vector<Tile>> currentMap;
			int maxScore = 0;
			int currentScore = 0;
			int playerHealth = 100;
			int currentWieldedWeapon = 0;
			int roundsLeft = 0;
		}survivalMeta;
	}gameMeta;
	bool gamestateChange = false;
	std::string metaFileName = "thelastwar.dat";
	void loadGame(bool);
	void saveGame();
	void initGame();

	void loadShaders();

	void spawnEnemies(int);
	Enemy& createEnemy(const sf::Vector2f&);
	void respawnEnemies();
	void spawnItems();
	void updateEnemies(int);
	void renderEnemies(sf::RenderStates);
	std::list<Enemy>::iterator deleteEnemy(std::list<Enemy>::iterator&);
	bool handleEvents();
	void updateProjectiles();
	void renderAllies(sf::RenderStates);
	void updateAllies();

	struct Area : sf::FloatRect {
		Area(const sf::FloatRect& rect) : sf::FloatRect(rect) {};
		unsigned numEnemies = 1;
		unsigned numItems = 2;
	};

	std::list<Area> hiddenAreas;
	std::list<sf::Vector2f> crateSpawnPoints;
	std::list<sf::Vector2f> lootSpawnPoints;

private:
	int type; //1 - Endless, 2 - Survival

		int currentEndlessScore = 0;
		int currentSurvivalScore = 0;
		int maxEndlessScore = 0;
		int maxSurvivalScore = 0;
		sf::Text endlessScoreCounter;
		sf::Text survivalScoreCounter;
		sf::Text maxEndlessScoreCounter;
		sf::Text maxSurvivalScoreCounter;

public:

	GameMode(int, Game&, PlayerClass, GameMeta, NPCMeta[], EnemyMeta[], InventoryMeta[], bool);
	~GameMode();

	virtual void logic();
	virtual void render();

	void addHiddenArea(const sf::FloatRect&);
	void addLootSpawn(const sf::Vector2f& pos);
	void addCrateSpawn(const sf::Vector2f& pos);
};
#endif
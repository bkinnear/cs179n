#include "gamemode.hpp"
#include "menustate.hpp"

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <list>
#include <unordered_map>
#include <algorithm>

// map width in tiles
#define MAP_WIDTH 50
// map height in tiles
#define MAP_HEIGHT 50
// offset to get middle of player sprite
#define PLAYER_OFFSET sf::Vector2f({14.f, 16.f})
// minimum distance to pick up items
#define MIN_DIST_ITEM 48.f

#define gwindow game.window

// returns amount of item that picking up a certain item will yield
int getLootAmount(Item::type type) {
	switch (type) {
	case Item::type::ammo_556:
		return 15 + rand() % 30;
	case Item::type::ammo_9mm:
		return 15 + rand() % 30;
	case Item::type::MP5:
		return 1;
	case Item::type::M4:
		return 1;
	case Item::type::ammo_crate:
		return 15 + rand() % 15;
	case Item::type::health_pack:
		return 1;
	case Item::type::medkit:
		return 1;
	case Item::type::barrel:
		return 1;
	}

	return 1;
}

// returns type of item that picking up a certain item will yield
Item::type getLootItem(Item::type type) {
	switch (type) {
	case Item::type::ammo_crate:
		// ammo crates yield a random ammo type
	{
		int r = rand() % 2;
		switch (r) {
		case 0:
			return Item::type::ammo_9mm;
		case 1:
			return Item::type::ammo_556;
		}
	}
	break;
	}
	return type;
}

GameMode::GameMode(int type, Game& game, PlayerClass playerClass, GameMeta gameLoadMeta, bool isLoadCall) :
	State(game),
	type(type),
	player(playerClass),
	tileMap(MAP_WIDTH, MAP_HEIGHT),
	texPlayerRight(createTexture("res/player_r_strip.png")),
	texPlayerLeft(createTexture("res/player_l_strip.png")),
	texPlayerRightMp5(createTexture("res/player_r_mp5_strip.png")),
	texPlayerLeftMp5(createTexture("res/player_l_mp5_strip.png")),
	texPlayerRightM4(createTexture("res/player_r_m4_strip.png")),
	texPlayerLeftM4(createTexture("res/player_l_m4_strip.png")),
	texAllyRight(createTexture("res/player2_r_strip.png")),
	texAllyLeft(createTexture("res/player2_l_strip.png")),
	texDummyRight(createTexture("res/Dummy_stand.png")),
	texBarrel(createTexture("res/barrel.png")),
	texShield(createTexture("res/shield.png")),
	texProjectile(createTexture("res/projectile.png")),
	inventory(createTexture("res/inventory.png"), createTexture("res/item_strip.png")),
	texEnemyRight(createTexture("res/enemy_r_strip.png")),
	texEnemyLeft(createTexture("res/enemy_l_strip.png")),
	texWeaponMP5(createTexture("res/mp5.png")),
	texExplosionSmall(createTexture("res/explosion_small_strip.png")),
	texExplosionLarge(createTexture("res/explosion_large.png")),
	texDeadEyeOpen(createTexture("res/deadeye_open.png")),
	texDeadEyeClose(createTexture("res/deadeye_close.png")),
	playerIcon(createTexture("res/Player1_display.png")),
	playerDeath(createTexture("res/player_death.png")),
	ammoIcon(createTexture("res/ammo_icon.png")),
	grenadeIcon(createTexture("res/grenade_icon.png")),
	reticle(createTexture("res/reticle.png"))
{
	// generate tile map
	tileMap.generate(this);

	// set main view
	mainView.reset({ 0.f, 0.f, 1366.f, 768.f });
	guiView.reset({ 0.f, 0.f, 1366.f, 768.f });

	/*	=============================
		 allocate our resources here
		=============================  */

	if (!music.openFromFile("res/music.wav")) {
		std::cout << "error loading ambient music" << std::endl;
	}
	music.setLoop(true);
	music.setVolume(80);
	music.play();

	if (!ambientZombie.openFromFile("res/zombie_ambient.ogg")) {
		std::cout << "error loading ambient zombie" << std::endl;
	}
	ambientZombie.setLoop(true);
	ambientZombie.setVolume(50);
	ambientZombie.play();


	//gun sounds
	if (!gunShotBuffer.loadFromFile("res/gun-shot.wav")) {
		std::cout << "error loading gunshot noises" << std::endl;
	}
	if (!emptyGunBuffer.loadFromFile("res/empty-gun.wav")) {
		std::cout << "error loading gunshot noises" << std::endl;
	}
	if (!mp5ReloadBuffer.loadFromFile("res/mp5-reload.wav")) {
		std::cout << "error loading mp5-reload noises" << std::endl;
	}

	//grenade sounds
	if (!grenadeShotBuffer.loadFromFile("res/grenade-shot.wav")) {
		std::cout << "error loading grenade shot noises" << std::endl;
	}
	if (!grenadeExplodeBuffer.loadFromFile("res/grenade-explode.wav")) {
		std::cout << "error loading grenade explode noises" << std::endl;
	}

	//melee sounds
	if (!meleeSoundBuffer.loadFromFile("res/melee_impact.wav")) {
		std::cout << "error loading melee impact noises" << std::endl;
	}
	if (!meleeSwingBuffer.loadFromFile("res/melee_swing.wav")) {
		std::cout << "error loading melee swing noises" << std::endl;
	}

	//zombie sounds
	if (!zombieBuffer1.loadFromFile("res/zombie_groan.wav")) {
		std::cout << "error loading zombie noises" << std::endl;
	}
	if (!zombieBuffer2.loadFromFile("res/zombie-gargles.wav")) {
		std::cout << "error loading zombie noises" << std::endl;
	}
	if (!zombieBuffer3.loadFromFile("res/zombie-growl-3.wav")) {
		std::cout << "error loading zombie noises" << std::endl;
	}
	if (!zombieBuffer4.loadFromFile("res/zombie-snarl.wav")) {
		std::cout << "error loading zombie noises" << std::endl;
	}
	if (!zombieBuffer5.loadFromFile("res/zombie_groan2.wav")) {
		std::cout << "error loading zombie noises" << std::endl;
	}

	//doors opening and closing
	if (!doorOpen.loadFromFile("res/door-open.wav")) {
		std::cout << "error loading open door noises" << std::endl;
	}
	if (!doorClose.loadFromFile("res/door-closing.wav")) {
		std::cout << "error loading close door noises" << std::endl;
	}

	//dropping tech
	if (!mechNoise.loadFromFile("res/mechanical_noise.wav")) {
		std::cout << "error loading mechanical noises" << std::endl;
	}
	if (!het_hon.loadFromFile("res/het_hon.wav")) {
		std::cout << "error loading shield noises" << std::endl;
	}

	// load font
	font.loadFromFile("res/VCR_OSD_MONO.ttf");

	// load item details text
	txtItemDetails.setFont(font);
	txtItemDetails.setCharacterSize(12);
	txtItemDetails.setColor(sf::Color(0x303030FF));
	//txtItemDetails.setOutlineColor(sf::Color::Black);
	//txtItemDetails.setOutlineThickness(1);

	// load dialog GUI
	dialogBox1.setSize({ 480.f, 160.f });
	dialogBox1.setFillColor(sf::Color(0xf5eeceee));
	dialogBox1.setOutlineColor(sf::Color(0x000000FF));
	dialogBox1.setOutlineThickness(1.f);
	dialogBox1.setPosition({ game.portWidth / 2 - 240.f, game.portHeight - 220.f });

	dialogBox2.setSize({ 96.f, 28.f });
	dialogBox2.setFillColor(sf::Color(0xcef5f1ee));
	dialogBox2.setOutlineColor(sf::Color(0x000000FF));
	dialogBox2.setOutlineThickness(1.f);
	dialogBox2.setPosition(dialogBox1.getPosition() + sf::Vector2f({ 16.f, -29.f }));

	dialogMessage.setPosition(dialogBox1.getPosition() + sf::Vector2f({ 4.f, 4.f }));
	dialogMessage.setFont(font);
	dialogMessage.setCharacterSize(14);
	dialogMessage.setColor(sf::Color(0x000000ff));

	dialogSpeaker.setPosition(dialogBox2.getPosition() + sf::Vector2f({ 4.f, 4.f }));
	dialogSpeaker.setFont(font);
	dialogSpeaker.setCharacterSize(14);
	dialogSpeaker.setColor(sf::Color(0x000000ff));

	// load FPS counter
	fpsCounter.setPosition({ 4.f, 4.f });
	fpsCounter.setFont(font);
	fpsCounter.setCharacterSize(16);
	fpsCounter.setFillColor(sf::Color(0x00EE00FF));
	fpsCounter.setOutlineColor(sf::Color(0x000000FF));
	fpsCounter.setOutlineThickness(2.f);
	fpsTick = 0;

	// load item details shape (the box behind the text)
	shpItemDetails.setFillColor(sf::Color(0xAAAAAAFF));
	shpItemDetails.setOutlineThickness(1.f);
	shpItemDetails.setOutlineColor(sf::Color::Black);

	// create animated sprite for player
	player.create(texPlayerRight, { 0, 0, 32, 32 }, 8);
	player.setMaskBounds({ 6, 2, 18, 27 });
	player.speed = 2;
	
	// set player class vars
	chooseClass(playerClass);

	// load effects
	explosionSmall = loadEffect(texExplosionSmall, { 0, 0, 8, 8 }, 6, 20);
	explosionLarge = loadEffect(texExplosionLarge, { 0,0,64,64 }, 6, 20);
	deadEyeOpen = loadEffect(texDeadEyeOpen, { 0,0,16,16 }, 6, 12);
	deadEyeClose = loadEffect(texDeadEyeClose, { 0,0,16,16 }, 6, 12);

	// add some stuff to the inventory
	inventory.addItem(Item::type::MP5, 1);
	inventory.addItem(Item::type::ammo_9mm, 95);

	GameMode::spawnItems();

	// add ally
	if (type == 1) {
		allies.emplace_back(texAllyLeft);
		allies.back().setPosition(player.getPosition() + sf::Vector2f({ 32.f, 0.f }));
		allies.back().setMaskBounds({ 8, 0, 15, 32 });
		allies.emplace_back(texAllyLeft);
		allies.back().setPosition(player.getPosition() + sf::Vector2f({ 32.f, 0.f }));
		allies.back().setMaskBounds({ 8, 0, 15, 32 });
	}

	// Player HUD
	//  
	//HP bar
	playerHPBack.setSize({ 200.f, 14.f });
	playerHPBack.setFillColor(sf::Color::Transparent);
	playerHPBack.setPosition(60, 725);
	playerHPBack.setOutlineColor(sf::Color::Black);
	playerHPBack.setOutlineThickness(2.f);
	playerHPBar.setPosition(playerHPBack.getPosition().x, playerHPBack.getPosition().y);
	playerIcon.setPosition(playerHPBack.getPosition().x - 62, playerHPBack.getPosition().y - 25);
	//mag and total ammo counter;
	ammoIcon.setPosition(playerHPBack.getPosition().x, playerHPBack.getPosition().y - 25);
	ammoCount.setFont(font);
	ammoCount.setCharacterSize(12);
	ammoCount.setColor(sf::Color::Black);
	ammoCount.setPosition(playerHPBar.getPosition().x + 25, playerHPBar.getPosition().y - 20);
	//grenade counter
	int gCount = 3;
	grenadeIcon.setPosition(playerHPBack.getPosition().x + 75, playerHPBack.getPosition().y - 26);
	grenadeIcon.setColor(sf::Color::Green);
	grenadesNum.setFont(font);
	grenadesNum.setCharacterSize(12);
	grenadesNum.setColor(sf::Color::Black);
	grenadesNum.setString("x" + std::to_string(gCount));
	grenadesNum.setPosition(playerHPBar.getPosition().x + 100, playerHPBar.getPosition().y - 20);
	//ability HUD
	abilityIcon1.setPosition(playerHPBack.getPosition().x+220, playerHPBack.getPosition().y-15);
	abilityIcon1.setScale(.75, .75);
	abilityIcon2.setPosition(playerHPBack.getPosition().x+270, playerHPBack.getPosition().y-15);
	abilityIcon2.setScale(.75, .75);
	abilityIcon3.setPosition(playerHPBack.getPosition().x+320, playerHPBack.getPosition().y-15);
	abilityIcon3.setScale(.75, .75);
	//ability HUD timers
	abilityClock1.setFont(font);
	abilityClock1.setCharacterSize(20);
	abilityClock1.setColor(sf::Color::Black);
	abilityClock1.setStyle(sf::Text::Bold);
	abilityClock1.setPosition(playerHPBack.getPosition().x + 232.5, playerHPBack.getPosition().y - 10);
	abilityClock2.setFont(font);
	abilityClock2.setCharacterSize(20);
	abilityClock2.setColor(sf::Color::Black);
	abilityClock2.setStyle(sf::Text::Bold);
	abilityClock2.setPosition(playerHPBack.getPosition().x + 282.5, playerHPBack.getPosition().y - 10);
	abilityClock3.setFont(font);
	abilityClock3.setCharacterSize(20);
	abilityClock3.setColor(sf::Color::Black);
	abilityClock3.setStyle(sf::Text::Bold);
	abilityClock3.setPosition(playerHPBack.getPosition().x + 332.5, playerHPBack.getPosition().y - 10);
	if (type == 1)
	{
		endlessScoreCounter.setPosition({ 5.f, 30.f });
		endlessScoreCounter.setFont(font);
		endlessScoreCounter.setCharacterSize(16);
		endlessScoreCounter.setFillColor(sf::Color(0x00EE00FF));
		endlessScoreCounter.setOutlineColor(sf::Color(0x000000FF));
		endlessScoreCounter.setOutlineThickness(2.f);

	if (isLoadCall)
	{
		if (type == 1)
		{
			player.setPosition(gameLoadMeta.endlessMeta.playerPosX, gameLoadMeta.endlessMeta.playerPosY);
			//tileMap.setTileMap(gameLoadMeta.endlessMeta.currentMap);
			currentEndlessScore = gameLoadMeta.endlessMeta.currentScore;
			std::string str = "Score: " + std::to_string(currentEndlessScore);
			endlessScoreCounter.setString(str);
			std::string maxStr = "Max Score: " + std::to_string(maxEndlessScore);
			maxEndlessScoreCounter.setString(maxStr);
		}
		else if(type == 2)
		{
			player.setPosition(gameLoadMeta.survivalMeta.playerPosX, gameLoadMeta.survivalMeta.playerPosY);
			currentSurvivalScore = gameLoadMeta.survivalMeta.currentScore;
			std::string str = "Score: " + std::to_string(currentSurvivalScore);
			survivalScoreCounter.setString("Score: " + str);
			std::string maxStr = "Max Score: " + std::to_string(maxSurvivalScore);
			maxSurvivalScoreCounter.setString(maxStr);
			//tileMap.setTileMap(gameLoadMeta.survivalMeta.currentMap);
			currentLevel = gameLoadMeta.survivalMeta.currentLevel;
			currentEnemySpawningCount = (currentLevel * 2) + 1;
			currentEnemyPresent = currentEnemySpawningCount;
			GameMode::spawnEnemies(currentEnemySpawningCount);
		}
		maxEndlessScore = gameLoadMeta.endlessMeta.maxScore;
		maxSurvivalScore = gameLoadMeta.survivalMeta.maxScore;
	}
	else
	{

		loadGame(false);
		maxEndlessScore = gameMeta.endlessMeta.maxScore;
		maxSurvivalScore = gameMeta.survivalMeta.maxScore;
		// TODO delegate to children classes
		if (type == 1)
		{
			std::string str = "Score: " + std::to_string(currentEndlessScore);
			endlessScoreCounter.setString(str);
			std::string maxStr = "Max Score: " + std::to_string(maxEndlessScore);
			maxEndlessScoreCounter.setString(maxStr);
			GameMode::spawnEnemies(defaultEnemySpawningCount);
		}
		else if (type == 2)
		{
			std::string str = "Score: " + std::to_string(currentSurvivalScore);
			survivalScoreCounter.setString(str);
			std::string maxStr = "Max Score: " + std::to_string(maxSurvivalScore);
			maxSurvivalScoreCounter.setString(maxStr);
			GameMode::spawnEnemies(currentEnemySpawningCount);
		}
	}
	
	std::cout << "GameMode object size (on stack): " << sizeof(*this)/1024 << " KiB"<< std::endl;
}

int GameMode::hashPos(const sf::Vector2f& pos) const {
	return (int)std::floor(pos.x / 32.f) + ((int)std::floor(pos.y / 32.f)) * tileMap.mapWidth;
}

ItemSpr* GameMode::createItem(const sf::Vector2f& pos, Item::type type) {
	itemsOnMap.push_back(new ItemSpr({ type, sf::Sprite() }));
	sf::Sprite& spr = itemsOnMap.back()->spr;
	spr.setPosition(pos.x, pos.y);
	spr.setTexture(inventory.texItemTileset);
	spr.setTextureRect(sf::IntRect(getItemTexOffset(itemsOnMap.back()->type), { 48,48 }));
	spr.setScale(.5, .5);

	// hashkey organizes items in buckets that hold items in 1 tile space (32x32)
	int hashKey = hashPos(pos);
	auto itemList = itemHash.find(hashKey);
	if (itemList == itemHash.end()) {
		itemHash[hashKey] = {};
	}
	itemHash[hashKey].push_back(itemsOnMap.back());

	return itemsOnMap.back();
}

void GameMode::removeItem(ItemSpr* pItem) {
	// remove item from hash
	itemHash.at(hashPos(pItem->spr.getPosition())).remove(pItem);
	// remove item from item list
	itemsOnMap.remove(pItem);
	// free item memory
	delete pItem;
}

ItemSpr* GameMode::getItemAt(const sf::Vector2f& pos) {
	auto itemListItr = itemHash.find(hashPos(pos));
	// see if hashmap has an entry
	if (itemListItr == itemHash.end())
		return nullptr;

	// see if list is empty or not
	if (itemListItr->second.empty())
		return nullptr;

	// return first item in hashmap entry list
	return itemListItr->second.front();
}

GameMode::~GameMode() {
}

void GameMode::setDialog(const std::string& speaker, const std::string& msg) {
	showDialog = true;
	dialogSpeaker.setString(speaker + ':');
	dialogMessage.setString(msg);
}

void GameMode::hideDialog() {
	showDialog = false;
}
void GameMode::updateCooldowns() {
	switch (player.playerClass) {
	case PlayerClass::DEFAULT:
		break;
	case PlayerClass::MEDIC:
		if (onCoolDown1) {
			if (elapsed1.asSeconds() < cooldown1) {
				if (cooldown1 - elapsed1.asSeconds() > 9) {//if double digit, center the position
					abilityClock1.setPosition(playerHPBack.getPosition().x + 225.5, playerHPBack.getPosition().y - 10);
				}
				else {
					abilityClock1.setPosition(playerHPBack.getPosition().x + 232.5, playerHPBack.getPosition().y - 10);
				}
				abilityClock1.setString(std::to_string((cooldown1 - int(elapsed1.asSeconds()))));
				elapsed1 = abilityTimer1.getElapsedTime();
			}
			else { //if single digit, move back to original position
				onCoolDown1 = false;
				elapsed1 = sf::seconds(0);
				abilityIcon1.setTexture(createTexture("res/ability_icons/health_pack.png"));
				abilityClock1.setString("");
			}
		}
		if (onCoolDown2) {
			if (elapsed2.asSeconds() < cooldown2) {
				if (cooldown2 - elapsed2.asSeconds() > 9) {
					abilityClock2.setPosition(playerHPBack.getPosition().x + 275.5, playerHPBack.getPosition().y - 10);
				}
				else {
					abilityClock2.setPosition(playerHPBack.getPosition().x + 282.5, playerHPBack.getPosition().y - 10);
				}
				abilityClock2.setString(std::to_string((cooldown2 - int(elapsed2.asSeconds()))));
				elapsed2 = abilityTimer2.getElapsedTime();
				if (elapsed2.asMilliseconds() > 25) { //dash for 25 milliseconds
					player.speed = 3; //set back to default
				}
			}
			else {
				onCoolDown2 = false;
				elapsed2 = sf::seconds(0);
				abilityIcon2.setTexture(createTexture("res/ability_icons/dash.png"));
				abilityClock2.setString("");
			}
		}
		if (onCoolDown3) {
			if (elapsed3.asSeconds() < cooldown3) {
				if (cooldown3 - elapsed3.asSeconds() > 9) {
					abilityClock3.setPosition(playerHPBack.getPosition().x + 325.5, playerHPBack.getPosition().y - 10);
				}
				else {
					abilityClock3.setPosition(playerHPBack.getPosition().x + 332.5, playerHPBack.getPosition().y - 10);
				}
				abilityClock3.setString(std::to_string((cooldown3 - int(elapsed3.asSeconds()))));
				elapsed3 = abilityTimer3.getElapsedTime();
			}
			else {
				onCoolDown3 = false;
				elapsed3 = sf::seconds(0);
				abilityIcon3.setTexture(createTexture("res/ability_icons/guardian_angel.png"));
				abilityClock3.setString("");
			}
		}
		break;
	case PlayerClass::ASSAULT:
		if (onCoolDown1) {
			if (elapsed1.asSeconds() < cooldown1) {
				if (cooldown1 - elapsed1.asSeconds() > 9) {
					abilityClock1.setPosition(playerHPBack.getPosition().x + 225.5, playerHPBack.getPosition().y - 10);
				}
				else {
					abilityClock1.setPosition(playerHPBack.getPosition().x + 232.5, playerHPBack.getPosition().y - 10);
				}
				abilityClock1.setString(std::to_string((cooldown1 - int(elapsed1.asSeconds()))));
				elapsed1 = abilityTimer1.getElapsedTime();
			}
			else {
				onCoolDown1 = false;
				elapsed1 = sf::seconds(0);
				abilityIcon1.setTexture(createTexture("res/ability_icons/ammo.png"));
				abilityClock1.setString("");
			}
		}
		if (onCoolDown2) {
			if (elapsed2.asSeconds() < cooldown2) {
				if (cooldown2 - elapsed2.asSeconds() > 9) {
					abilityClock2.setPosition(playerHPBack.getPosition().x + 275.5, playerHPBack.getPosition().y - 10);
				}
				else {
					abilityClock2.setPosition(playerHPBack.getPosition().x + 282.5, playerHPBack.getPosition().y - 10);
				}
				abilityClock2.setString(std::to_string((cooldown2 - int(elapsed2.asSeconds()))));
				elapsed2 = abilityTimer2.getElapsedTime();
			}
			else {
				onCoolDown2 = false;
				elapsed2 = sf::seconds(0);
				abilityIcon2.setTexture(createTexture("res/ability_icons/grenade.png"));
				abilityClock2.setString("");
			}
		}
		if (onCoolDown3) {
			if (elapsed3.asSeconds() < cooldown3) {
				if (cooldown3 - elapsed3.asSeconds() > 9) {
					abilityClock3.setPosition(playerHPBack.getPosition().x + 325.5, playerHPBack.getPosition().y - 10);
				}
				else {
					abilityClock3.setPosition(playerHPBack.getPosition().x + 332.5, playerHPBack.getPosition().y - 10);
				}
				abilityClock3.setString(std::to_string((cooldown3 - int(elapsed3.asSeconds()))));
				elapsed3 = abilityTimer3.getElapsedTime();
				if (elapsed3.asSeconds() > 10) { //increase damage for 10 seconds
					player.isDeadEye = false; // turn off deadeye after 10 seconds
				}
			}
			else {
				onCoolDown3 = false;
				elapsed3 = sf::seconds(0);
				abilityIcon3.setTexture(createTexture("res/ability_icons/deadeye.png"));
				abilityClock3.setString("");
			}
		}
		break;
	case PlayerClass::ENGINEER:
		if (onCoolDown1) {
			if (elapsed1.asSeconds() < cooldown1) {
				if (cooldown1 - elapsed1.asSeconds() > 9) {
					abilityClock1.setPosition(playerHPBack.getPosition().x + 225.5, playerHPBack.getPosition().y - 10);
				}
				else {
					abilityClock1.setPosition(playerHPBack.getPosition().x + 232.5, playerHPBack.getPosition().y - 10);
				}
				abilityClock1.setString(std::to_string((cooldown1 - int(elapsed1.asSeconds()))));
				elapsed1 = abilityTimer1.getElapsedTime();
			}
			else {
				onCoolDown1 = false;
				elapsed1 = sf::seconds(0);
				abilityIcon1.setTexture(createTexture("res/ability_icons/decoy.png"));
				abilityClock1.setString("");
			}
		}
		if (onCoolDown2) {
			if (elapsed2.asSeconds() < cooldown2) {
				if (cooldown2 - elapsed2.asSeconds() > 9) {
					abilityClock2.setPosition(playerHPBack.getPosition().x + 275.5, playerHPBack.getPosition().y - 10);
				}
				else {
					abilityClock2.setPosition(playerHPBack.getPosition().x + 282.5, playerHPBack.getPosition().y - 10);
				}
				abilityClock2.setString(std::to_string((cooldown2 - int(elapsed2.asSeconds()))));
				elapsed2 = abilityTimer2.getElapsedTime();
			}
			else {
				onCoolDown2 = false;
				elapsed2 = sf::seconds(0);
				abilityIcon2.setTexture(createTexture("res/ability_icons/barrel.png"));
				abilityClock2.setString("");
			}
		}
		if (onCoolDown3) {
			if (elapsed3.asSeconds() < cooldown3) {
				if (cooldown3 - elapsed3.asSeconds() > 9) {
					abilityClock3.setPosition(playerHPBack.getPosition().x + 325.5, playerHPBack.getPosition().y - 10);
				}
				else {
					abilityClock3.setPosition(playerHPBack.getPosition().x + 332.5, playerHPBack.getPosition().y - 10);
				}
				abilityClock3.setString(std::to_string((cooldown3 - int(elapsed3.asSeconds()))));
				elapsed3 = abilityTimer3.getElapsedTime();
			}
			else {
				onCoolDown3 = false;
				elapsed3 = sf::seconds(0);
				abilityIcon3.setTexture(createTexture("res/ability_icons/shield.png"));
				abilityClock3.setString("");
			}
		}
		break;
	case PlayerClass::SLASHER:
		if (onCoolDown1) {
			if (elapsed1.asSeconds() < cooldown1) {
				if (cooldown1 - elapsed1.asSeconds() > 9) {
					abilityClock1.setPosition(playerHPBack.getPosition().x + 225.5, playerHPBack.getPosition().y - 10);
				}
				else {
					abilityClock1.setPosition(playerHPBack.getPosition().x + 232.5, playerHPBack.getPosition().y - 10);
				}
				abilityClock1.setString(std::to_string((cooldown1 - int(elapsed1.asSeconds()))));
				elapsed1 = abilityTimer1.getElapsedTime();
			}
			else {
				onCoolDown1 = false;
				elapsed1 = sf::seconds(0);
				abilityIcon1.setTexture(createTexture("res/ability_icons/smash.png"));
				abilityClock1.setString("");
			}
		}
		if (onCoolDown2) {
			if (elapsed2.asSeconds() < cooldown2) {
				if (cooldown2 - elapsed2.asSeconds() > 9) {
					abilityClock2.setPosition(playerHPBack.getPosition().x + 275.5, playerHPBack.getPosition().y - 10);
				}
				else {
					abilityClock2.setPosition(playerHPBack.getPosition().x + 282.5, playerHPBack.getPosition().y - 10);
				}
				abilityClock2.setString(std::to_string((cooldown2 - int(elapsed2.asSeconds()))));
				elapsed2 = abilityTimer2.getElapsedTime();
				if (elapsed2.asMilliseconds() > 3000) { //warcry for 3 seconds
					player.isWarcry = false;
				}
			}
			else {
				onCoolDown2 = false;
				elapsed2 = sf::seconds(0);
				abilityIcon2.setTexture(createTexture("res/ability_icons/warcry.png"));
				abilityClock2.setString("");
			}
		}
		if (onCoolDown3) {
			if (elapsed3.asSeconds() < cooldown3) {
				if (cooldown3 - elapsed3.asSeconds() > 9) {
					abilityClock3.setPosition(playerHPBack.getPosition().x + 325.5, playerHPBack.getPosition().y - 10);
				}
				else {
					abilityClock3.setPosition(playerHPBack.getPosition().x + 332.5, playerHPBack.getPosition().y - 10);
				}
				abilityClock3.setString(std::to_string((cooldown3 - int(elapsed3.asSeconds()))));
				elapsed3 = abilityTimer3.getElapsedTime();
				if (elapsed2.asMilliseconds() > 5000) { //dash for 5 seconds
					player.speed = 3; //set back to default
				}
			}
			else {
				onCoolDown3 = false;
				elapsed3 = sf::seconds(0);
				abilityIcon3.setTexture(createTexture("res/ability_icons/rage.png"));
				abilityClock3.setString("");
			}
		}
		break;
	}
}

bool GameMode::handleEvents() {
	// handle events
	sf::Event e;
	while (!gamestateChange && game.window.pollEvent(e)) {
		switch (e.type) {
		case sf::Event::Closed:
			// delete this gamestate
			game.close();
			return false;
		case sf::Event::KeyPressed:
			switch (e.key.code) {
			case sf::Keyboard::W:
			case sf::Keyboard::Up:
				player.movingUp = true;
				break;
			case sf::Keyboard::A:
			case sf::Keyboard::Left:
				player.movingLeft = true;
				break;
			case sf::Keyboard::S:
			case sf::Keyboard::Down:
				player.movingDown = true;
				break;
			case sf::Keyboard::D:
			case sf::Keyboard::Right:
				player.movingRight = true;
				break;
			case sf::Keyboard::Tab:
				showInventory = !showInventory;
				showItemDetails = false;
				break;
			case sf::Keyboard::E:
				// pick up item
				// check for items in tiles adjacent to player
				for (int i = -1; i <= 1; i++) {
					for (int j = -1; j <= 1; j++) {
						sf::Vector2f pos = player.getPosition() + PLAYER_OFFSET + sf::Vector2f({ i * 32.f, j * 32.f });
						ItemSpr* pItem = getItemAt(pos);
						if (!pItem)
							continue;
						float distToPlayer = Utils::pointDistance(player.getPosition() + PLAYER_OFFSET, pItem->spr.getPosition());
						if (distToPlayer <= MIN_DIST_ITEM) {
							// add item to inventory
							inventory.addItem(getLootItem(pItem->type), getLootAmount(pItem->type));

							removeItem(pItem);
						}
					}
				}
				break;
			case sf::Keyboard::Q:
			{
				sf::Vector2f position = player.getPosition();
				int x, y;
				bool isDoor = tileMap.isDoor(position.x + 16, position.y - 16);
				if (isDoor)
				{
					x = position.x + 16;
					y = position.y - 16;
				}
				else
				{
					isDoor = tileMap.isDoor(position.x + 16, position.y + 48);
					if (isDoor)
					{
						x = position.x + 16;
						y = position.y + 48;
					}
				}
				if (isDoor)
				{
					int tileX = x / TILE_SIZE;
					int tileY = y / TILE_SIZE;
					switch (tileMap.getTileType(x, y))
					{
					case 30:
						//Closed Door Type - 1
						tileMap.setTile(tileX, tileY, 31);
						std::cout << "changed door to 31" << std::endl;
						doorInteract.setBuffer(doorOpen);
						doorInteract.play();
						break;
					case 32:
						//Closed Door Type - 2
						tileMap.setTile(tileX, tileY, 33);
						std::cout << "changed door to 33" << std::endl;
						break;
					case 31:
						//Opened Door Type - 1
						tileMap.setTile(tileX, tileY, 30);
						std::cout << "changed door to 30" << std::endl;
						doorInteract.setBuffer(doorClose);
						doorInteract.play();
						break;
					case 33:
						//Opened Door Type - 2
						tileMap.setTile(tileX, tileY, 32);
						std::cout << "changed door to 32" << std::endl;
						break;
					default:
						break;
					}
				}
				else
				{
					std::cout << "Not a door!\n";
				}
			}
			break;
			case sf::Keyboard::R: // reload weapon
				if (inventory.getRoundsLeft() != inventory.getWielded().getMagCapacity() || !inventory.useWielded()) {
					reloadSound.setBuffer(mp5ReloadBuffer);
					reloadSound.play();
				}
				inventory.reloadWielded();
				break;
			case sf::Keyboard::T:
				switch (dialogTreeIndex++) {
				case 0:
					setDialog("Bryce", "I can see you...");
					break;
				case 1:
					setDialog("Bryce", "jk");
					break;
				case 2:
					setDialog("Bryce", "ok bye");
					break;
				case 3:
					hideDialog();
					break;
				}
				break;
			case sf::Keyboard::Num1: //FIRST ABILITY
				switch (player.playerClass) {
				case PlayerClass::DEFAULT:
					break;
				case PlayerClass::ASSAULT:
					//ASSAULT FIRST ABILITY GOES HERE
					if (!onCoolDown1) {
						assault_ammo();
						std::cout << "Assault Ability - Dropped Ammo crate" << std::endl;
					}
					else {
						std::cout << "Assault Ability - Ammo are on cooldown" << std::endl;
					}
					break;
				case PlayerClass::MEDIC:
					//MEDIC FIRST ABILITY GOES HERE
					if (!onCoolDown1) {
						medic_bandage();
						std::cout << "Medic Ability - Dropped Health Pack" << std::endl;
					}
					else {
						std::cout << "Medic Ability - Health Packs are on cooldown" << std::endl;
					}
					break;
				case PlayerClass::SLASHER:
					if (!onCoolDown1) {
						slasher_smash();
						std::cout << "Slasher Ability - Smash" << std::endl;
					}
					else {
						std::cout << "Slasher Ability - Smash is on cooldown" << std::endl;
					}
					break;
				case PlayerClass::ENGINEER:
					if (!onCoolDown1) {
						engineer_decoy();
						std::cout << "Engineer Ability - Deployed Decoy" << std::endl;
					}
					else {
						std::cout << "Engineer Ability - Decoys are on cooldown" << std::endl;
					}
					break;
				}
				break;
			case sf::Keyboard::Num2: //SECOND ABILITY
				switch (player.playerClass) {
				case PlayerClass::DEFAULT:
					break;
				case PlayerClass::ASSAULT:
					if (!onCoolDown2) {
						assault_grenade();
						std::cout << "Assault Ability - Grenade" << std::endl;
					}
					else {
						std::cout << "Assault Ability - Grenade is on cooldown" << std::endl;
					}
					break;
				case PlayerClass::MEDIC:
					if (!onCoolDown2) {
						medic_dash();
						std::cout << "Medic Ability - Dash" << std::endl;
					}
					else {
						std::cout << "Medic Ability - Dash is on cooldown" << std::endl;
					}
					break;
				case PlayerClass::SLASHER:
					if (!onCoolDown2) {
						slasher_warcry();
						std::cout << "Slasher Ability - Warcry" << std::endl;
					}
					else {
						std::cout << "Slasher Ability - Warcry is on cooldown" << std::endl;
					}
					break;
				case PlayerClass::ENGINEER:
					if (!onCoolDown2) {
						engineer_barrel();
						std::cout << "Engineer Ability - Deployed Barrel" << std::endl;
					}
					else {
						std::cout << "Engineer Ability - Barrels are on cooldown" << std::endl;
					}
					break;
				}
				break;
			case sf::Keyboard::Num3: //THIRD ABILITY
				switch (player.playerClass) {
				case PlayerClass::DEFAULT:
					break;
				case PlayerClass::ASSAULT:
					if (!onCoolDown3) {
						assault_deadeye();
						std::cout << "Assault Ability - Dead Eye" << std::endl;
					}
					else {
						std::cout << "Assault Ability - Dead Eye is on cooldown" << std::endl;
					}
					break;
				case PlayerClass::MEDIC:
					if (!onCoolDown3) {
						medic_heal();
						std::cout << "Medic Ability - Guardian Angel" << std::endl;
					}
					else {
						std::cout << "Medic Ability - Guardian Angel is on cooldown" << std::endl;
					}
					break;
				case PlayerClass::ENGINEER:
					if (!onCoolDown3) {
						engineer_shield();
						std::cout << "Engineer Ability - Shield" << std::endl;
					}
					else {
						std::cout << "Engineer Ability - Shield is on cooldown" << std::endl;
					}
					break;
				case PlayerClass::SLASHER:
					if (!onCoolDown3) {
						slasher_rage();
						std::cout << "Slasher Ability - Rage" << std::endl;
					}
					else {
						std::cout << "Slasher Ability - Rage is on cooldown" << std::endl;
					}
					break;
				}
				break;
			case sf::Keyboard::F1:
				std::cout << "Debug mode ";
				debugging = !debugging;
				if (debugging)
					std::cout << "on";
				else
					std::cout << "off";

				std::cout << std::endl;
				break;
			case sf::Keyboard::F2:
				// restarts the map
				if (type == 1)
					game.setState(new GameMode(1, game, player.playerClass, gameMeta, false));
				else if (type == 2)
					game.setState(new GameMode(2, game, player.playerClass, gameMeta, false));
				delete this;
				return false;
				break;
			case sf::Keyboard::F3:
				// go back to menu
				game.setState(new MenuState(game));
				delete this;
				return false;
				break;
			case sf::Keyboard::L://Load game
			{
				if (!isLoadInvoked) // Invoke loading only once
				{
					isLoadInvoked = true; 
					loadGame(true);
				}
			}
				break;
			case sf::Keyboard::K://Save Game
			{
				if (type == 1)//Endless Meta save
				{
					gameMeta.endlessMeta.playerPosX = player.getPosition().x;
					gameMeta.endlessMeta.playerPosY = player.getPosition().y;
					//gameMeta.endlessMeta.currentMap = tileMap.getTileMap();
					gameMeta.endlessMeta.maxScore = maxEndlessScore;
					gameMeta.endlessMeta.currentScore = currentEndlessScore;
				}
				else if (type == 2)//Survival Meta save
				{
					gameMeta.survivalMeta.currentLevel = currentLevel;
					gameMeta.survivalMeta.playerPosX = player.getPosition().x;
					gameMeta.survivalMeta.playerPosY = player.getPosition().y;
					//gameMeta.survivalMeta.currentMap = tileMap.getTileMap();
					gameMeta.survivalMeta.maxScore = maxSurvivalScore;
					gameMeta.survivalMeta.currentScore = currentSurvivalScore;
				}
				saveGame();
			}
				break;
			case sf::Keyboard::V:
				//melee
				if (inventory.useWieldedMelee()) {
					meleeSwing.setBuffer(meleeSwingBuffer);
					meleeSwing.setVolume(225);
					meleeSwing.play();
					// TODO - check to make sure weapon is ranged
					projectiles.emplace_back();
					Projectile& proj = projectiles.back();
					proj.isMelee = true;
					proj.setPosition(player.getPosition().x + 16, player.getPosition().y + 16);
					proj.setTexture(texProjectile);
					// set mask bounds to just the sprite bounds (default)
					proj.setMaskBounds(proj.getLocalBounds());
					proj.speed = 25;
					proj.direction = Utils::pointDirection(player.getPosition() + PLAYER_OFFSET, mousePos);
					proj.setRotation(proj.direction);
					proj.damage = inventory.getWielded().getDamage() * 1.5f;
				}
				break;
			}
			break;
		case sf::Event::KeyReleased:
			switch (e.key.code) {
			case sf::Keyboard::W:
			case sf::Keyboard::Up:
				player.movingUp = false;
				break;
			case sf::Keyboard::A:
			case sf::Keyboard::Left:
				player.movingLeft = false;
				break;
			case sf::Keyboard::S:
			case sf::Keyboard::Down:
				player.movingDown = false;
				break;
			case sf::Keyboard::D:
			case sf::Keyboard::Right:
				player.movingRight = false;
				break;
			}
			break;
		case sf::Event::MouseButtonPressed:
			switch (e.mouseButton.button) {
			case sf::Mouse::Button::Left:
				// LMB pressed

				// NOTE: moved attacks to mouse holding so user can do automatic fire
				// TODO make it so that only full-auto weapons can be held
				break;
			case sf::Mouse::Button::Right:
				// RMB pressed
				// 
				if (showInventory) {
					//using switch to enable the usage of non-weapon items
					sf::Vector2f pos;
					switch (inventory.getItemAt(winMousePos.x, winMousePos.y)->itemType) {
						case Item::type::medkit:
							if (player.health == 100) {
								std::cout << "Player already at max health." << std::endl;
								break;
							}
							else if (player.health < 50) {
								player.health += 50;
							}
							else {
								player.health = 100;
							}
							inventory.removeItem(Item::type::medkit, 1);
							break;
						case Item::type::health_pack:
							if (player.health == 100) {
								std::cout << "Player already at max health." << std::endl;
								break;
							}
							else if (player.health < 80) {
								player.health += 20;
							}
							else {
								player.health = 100;
							}
							inventory.removeItem(Item::type::health_pack, 1);
							break;
						case Item::type::walkie_talkie:
							//call in ally
							//spawn ally at random location, at least 450.f away from player, and on clear tile
							allies.emplace_back(texAllyLeft);
							for (;;) {
								pos = { (float)(rand() % tileMap.mapWidth * TILE_SIZE), (float)(rand() % tileMap.mapHeight * TILE_SIZE) };
								float dist = Utils::pointDistance(player.getPosition(), pos);
								if (!tileMap.isOpaque(pos.x, pos.y) && dist > 450.f)
									std::cout << "Walkie-talkie called in ally at position (" << pos.x << "," << pos.y << ")" << std::endl;
									break;
							}
							allies.back().setPosition(pos);
							allies.back().setMaskBounds({ 8, 0, 15, 32 });
							inventory.removeItem(Item::type::walkie_talkie, 1);
							break;
						default:
							// equip item
							inventory.wieldItemAt(winMousePos.x, winMousePos.y);
							break;

					}
				}

				break;
			}
			break;
		case sf::Event::MouseWheelScrolled:
			// TODO set limit for scrolling
			if (e.mouseWheelScroll.delta > 0)
				mainView.zoom(.9f);
			else
				mainView.zoom(1.1f);
			break;
		}
	}

	// check mouse state for holding (enabling auto fire)
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		// LMB held
		// try to use weapon
		if (inventory.useWielded()) {
			shotSound.setBuffer(gunShotBuffer);
			shotSound.setVolume(50);
			if (inventory.getWielded().itemType == Item::type::MP5) {
				shotSound.setPitch(3);
			}
			if (inventory.getWielded().itemType == Item::type::M4) {
				shotSound.setPitch(1);
			}
			shotSound.play();
			// TODO - check to make sure weapon is ranged
			projectiles.emplace_back();
			Projectile& proj = projectiles.back();
			proj.setPosition(player.getPosition().x + 16, player.getPosition().y + 16);
			proj.setTexture(texProjectile);
			// set mask bounds to just the sprite bounds (default)
			proj.setMaskBounds(proj.getLocalBounds());
			proj.speed = 12;
			proj.direction = Utils::pointDirection(player.getPosition() + PLAYER_OFFSET, mousePos);
			proj.setRotation(proj.direction);
			proj.damage = (int)floor(inventory.getWielded().getDamage() * std::max(2 * player.isDeadEye, 1) * std::max(1.5f * player.isRage, 1.f));
		}
		else if (inventory.getWielded().getAmmoType() != Item::type::null && inventory.getRoundsLeft() == 0) {
			if (shotSound.getStatus() != sf::Sound::Status::Playing) {
				shotSound.setBuffer(emptyGunBuffer);
				shotSound.setVolume(25);
				shotSound.play();
			}
		}
	}
	if (gamestateChange)
	{
		gamestateChange = false;
		return false;
	}
	// tell game state to continue
	return true;
}

void GameMode::render()
{
	// clear window - default color black
	gwindow.clear();

	// ========================== //
	// = v   world drawing   v  = //
	// ========================== //

	// move view target to center on player
	mainViewTarget = { floor(player.getPosition().x), floor(player.getPosition().y) };

	// move the view towards target
	sf::Vector2f delta(floor((mainViewTarget.x - mainView.getCenter().x) / 10), floor((mainViewTarget.y - mainView.getCenter().y) / 10));
	mainView.move(delta);

	// we must update view any time we change something in it
	// set the main view to draw the main map
	gwindow.setView(mainView);

	// draw the tilemap
	gwindow.draw(tileMap);

	//draw the weapons
	for (auto item : itemsOnMap) {
		gwindow.draw(item->spr);
	}

	// draw the player
	player.animateFrame();
	gwindow.draw(player);

	// draw the projectiles
	for (Projectile& proj : projectiles) {
		gwindow.draw(proj);
	}

	// draw the enemies
	GameMode::renderEnemies();

	if (type == 1)
	{
		// draw the allies
		renderAllies();
	}

	// draw effects
	drawEffects();

	// draw hidden areas
	sf::RectangleShape areaShape;
	areaShape.setFillColor(sf::Color::Black);
	for (auto area : hiddenAreas) {
		areaShape.setPosition(area.left, area.top);
		areaShape.setSize({ area.width, area.height });
		if (!player.getBounds().intersects(area))
			gwindow.draw(areaShape);
	}

	// ========================= //
	// =  v   GUI drawing   v  = //
	// ========================= //

	// set view to draw guis
	gwindow.setView(guiView);

	if (type == 1)
	{
		gwindow.draw(endlessScoreCounter);
		gwindow.draw(maxEndlessScoreCounter);
	}
	else if (type == 2)
	{
		gwindow.draw(survivalScoreCounter);
		gwindow.draw(maxSurvivalScoreCounter);
	}
	// draw the inventory
	if (showInventory)
		gwindow.draw(inventory);

	// draw item details
	if (showItemDetails) {
		gwindow.draw(shpItemDetails);
		gwindow.draw(txtItemDetails);
	}

	//draw player HUD
	gwindow.draw(playerHPBack);
	gwindow.draw(playerHPBar);
	gwindow.draw(playerIcon);
	gwindow.draw(ammoIcon);
	gwindow.draw(ammoCount);
	gwindow.draw(grenadeIcon);
	gwindow.draw(grenadesNum);
	gwindow.draw(abilityIcon1);
	gwindow.draw(abilityIcon2);
	gwindow.draw(abilityIcon3);
	gwindow.draw(abilityClock1);
	gwindow.draw(abilityClock2);
	gwindow.draw(abilityClock3);

	if (showDialog) {
		gwindow.draw(dialogBox1);
		gwindow.draw(dialogBox2);
		gwindow.draw(dialogMessage);
		gwindow.draw(dialogSpeaker);
	}

	gwindow.draw(fpsCounter);


	// update window
	gwindow.display();
}

void GameMode::logic()
{
	// get mouse x and y in window coords - used for GUI
	gwindow.setView(guiView);
	winMousePos = sf::Vector2i(game.window.mapPixelToCoords(sf::Mouse::getPosition(game.window)));

	// get mouse x and y in world coords
	gwindow.setView(mainView);
	mousePos = game.window.mapPixelToCoords(sf::Mouse::getPosition(gwindow));

	// update effects
	updateEffects();

	// handle all events
	// return if state exits
	if (!handleEvents())
		return;

	// check for hovering over item in inventory
	if (showInventory) {
		const Item* item = inventory.getItemAt(winMousePos.x, winMousePos.y);
		if (item) {
			showItemDetails = true;

			txtItemDetails.setString(item->getName() + '(' + std::to_string(item->num) + ')');
			txtItemDetails.setPosition(winMousePos.x, winMousePos.y - 16);

			shpItemDetails.setSize({ txtItemDetails.getGlobalBounds().width + 10, txtItemDetails.getGlobalBounds().height + 10 });
			shpItemDetails.setPosition(txtItemDetails.getGlobalBounds().left - 5, txtItemDetails.getGlobalBounds().top - 5);
		}
		else {
			showItemDetails = false;
		}
	}

	// update player sprite
	if ((player.movingLeft || player.movingUp || player.movingRight || player.movingDown) && player.alive) {
		Item::type weaponType = inventory.getWielded().itemType;
		if (player.getAnimSpeed() == -1)
			player.setAnimSpeed(12);
		switch (weaponType) {
		case Item::type::MP5:
			if (player.movingLeft) {
				player.setTexture(texPlayerLeftMp5);
			}
			if (player.movingRight) {
				player.setTexture(texPlayerRightMp5);
			}
			break;
		case Item::type::M4:
			if (player.movingLeft) {
				player.setTexture(texPlayerLeftM4);
			}
			if (player.movingRight) {
				player.setTexture(texPlayerRightM4);
			}
			break;
		default:
			if (player.movingLeft) {
				player.setTexture(texPlayerLeft);
			}
			if (player.movingRight) {
				player.setTexture(texPlayerRight);
			}
			break;
		}			
	}
	else {
		player.setIndex(0);
		player.setAnimSpeed(-1);
	}

	// player movement
	if (player.alive) {
		if (player.movingLeft)
			if (tileMap.areaClear(player, -player.speed, 0))
				player.move(-player.speed, 0);
		if (player.movingUp)
			if (tileMap.areaClear(player, 0, -player.speed))
				player.move(0, -player.speed);
		if (player.movingRight)
			if (tileMap.areaClear(player, player.speed, 0))
				player.move(player.speed, 0);
		if (player.movingDown)
			if (tileMap.areaClear(player, 0, player.speed))
				player.move(0, player.speed);
	}

	// update projectiles
	updateProjectiles();

	// update enemies

	if (type == 1)
	{
		GameMode::updateEnemies(1);
		// update all allies
		updateAllies();

		// update ability cooldowns
		updateCooldowns();
	}
	else if (type == 2)
	{
		GameMode::updateEnemies(2);
	}


	playerHPBar.setSize({ playerHPBack.getSize().x * (player.health / 100.f), playerHPBack.getSize().y });
	if (player.health > 75) {
		playerHPBar.setFillColor(sf::Color::Green);
	}
	else if (player.health > 25) {
		playerHPBar.setFillColor(sf::Color::Yellow);
	}
	else {
		playerHPBar.setFillColor(sf::Color::Red);
	}
	if (inventory.getWielded().getAmmoType() == Item::type::null)
		ammoCount.setString("-/-");
	else
		ammoCount.setString(std::to_string(inventory.getRoundsLeft()) + "/" + std::to_string(inventory.getNumItem(inventory.getWielded().getAmmoType())));

	// increment frames in fpsTick
	fpsTick++;
	if (fpsClock.getElapsedTime().asSeconds() >= .99f) {
		fpsCounter.setString("FPS: " + std::to_string(fpsTick));
		fpsClock.restart();
		fpsTick = 0;
	}
	
	inventory.tick();


	if (type == 1)
	{
		std::string str = "Score: " + std::to_string(currentEndlessScore);
		endlessScoreCounter.setString(str);
		std::string maxStr = "Max Score: " + std::to_string(maxEndlessScore);
		maxEndlessScoreCounter.setString(maxStr);

	}
	else if (type == 2)
	{
		std::string str = "Score: " + std::to_string(currentSurvivalScore);
		survivalScoreCounter.setString(str);
		std::string maxStr = "Max Score: " + std::to_string(maxSurvivalScore);
		maxSurvivalScoreCounter.setString(maxStr);
	}
}

void GameMode::updateProjectiles() {
	bool breaking = false;
	auto projItr = projectiles.begin();
	while (projItr != projectiles.end()) {
		// get movement of projectile for this frame
		sf::Vector2f moveVector = Utils::vectorInDirection(projItr->speed, projItr->direction);
		if (tileMap.areaClear(*projItr, moveVector)) {
			projItr->move(moveVector);
		}
		else {
			createEffect(
				explosionSmall,
				Utils::pointEdge(
					projItr->getPosition(),
					tileMap.getTileBounds(projItr->getPosition().x + 2.5f * moveVector.x, projItr->getPosition().y + 2.5f * moveVector.y)
				)
			);

			// destroy projectile
			projItr = projectiles.erase(projItr);
			continue;
		}

		if (projItr->isGrenade) {
			float maxRange = 250.f;
			float dist = Utils::pointDistance(projItr->shotFrom, projItr->getPosition());

			if (dist > maxRange || !(tileMap.areaClear(*projItr, moveVector))) {
				createEffect(explosionLarge, projItr->getPosition());
				grenadeSound.setBuffer(grenadeExplodeBuffer);
				grenadeSound.play();
				for (auto enemyItr = enemies.begin(); enemyItr != enemies.end(); enemyItr++) {
					float distToProj = Utils::pointDistance(enemyItr->getPosition(), projItr->getPosition());
					if (distToProj <= 150) {
						enemyItr->health -= 120;
						if (enemyItr->health <= 0) {
							if (type == 1)
							{
								currentEndlessScore += 1;
								maxEndlessScore = currentEndlessScore > maxEndlessScore ? currentEndlessScore : maxEndlessScore;
							}
							else if (type == 2)
							{
								currentSurvivalScore += 1;
								maxSurvivalScore = currentSurvivalScore > maxSurvivalScore ? currentSurvivalScore : maxSurvivalScore;
							}
							enemyItr = enemies.erase(enemyItr);
							GameMode::spawnEnemies(1);
							continue;
						}
					}
				}

				//  destroy projectile
				projItr = projectiles.erase(projItr);
				continue;
			}
		}
		else {
			//check if melee projectile, to shorten distance
			if (projItr->isMelee) {
				projItr->meleeRange--;
				if (projItr->meleeRange == 0) {
					//melee reached max range
					projItr = projectiles.erase(projItr);
					break;
				}
			}
			// check for collision with enemies
			// TODO - make enemies use a spatial hash so this algo's faster
			// this algo is currently O(K*N) where K = bullets, N = enemies
			bool collided = false;
			auto enemyItr = enemies.begin();
			while (enemyItr != enemies.end()) {
				// ignore if enemy is not colliding with projectile
				if (!enemyItr->isColliding(*projItr)) {
					enemyItr++;
					continue;
				}

				// deal damage to enemy
				if (projItr->isMelee) {
					meleeSound.setBuffer(meleeSoundBuffer);
					meleeSound.setVolume(50);
					meleeSound.play();
				}
				enemyItr->health -= projItr->damage;
				//std::cout << "DMG: " << projItr->damage << std::endl;

				// destroy enemy if health below 0
				if (enemyItr->health <= 0) {
					if (type == 1)
					{
						currentEndlessScore += 1;
						maxEndlessScore = currentEndlessScore > maxEndlessScore ? currentEndlessScore : maxEndlessScore;
					}
					else if (type == 2)
					{
						currentSurvivalScore += 1;
						maxSurvivalScore = currentSurvivalScore > maxSurvivalScore ? currentSurvivalScore : maxSurvivalScore;
					}
					enemyItr = enemies.erase(enemyItr);
					respawnEnemies();
					continue;
				}

				collided = true;
				break;
			}

			// destroy bullet
			if (collided) {
				projItr = projectiles.erase(projItr);
				continue;
			}
		}

		// move on to next projectile
		projItr++;
	}
}

void GameMode::chooseClass(PlayerClass playerClass) {
	switch (playerClass) {
	case PlayerClass::MEDIC:
		cooldown1 = 3; //in seconds
		cooldown2 = 5;
		cooldown3 = 10;
		player.speed = 3;
		abilityIcon1.setTexture(createTexture("res/ability_icons/health_pack.png"));
		abilityIcon2.setTexture(createTexture("res/ability_icons/dash.png"));
		abilityIcon3.setTexture(createTexture("res/ability_icons/guardian_angel.png"));
		break;
	case PlayerClass::ASSAULT:
		cooldown1 = 5; 
		cooldown2 = 10;
		cooldown3 = 20;
		player.speed = 3;
		abilityIcon1.setTexture(createTexture("res/ability_icons/ammo.png"));
		abilityIcon2.setTexture(createTexture("res/ability_icons/grenade.png"));
		abilityIcon3.setTexture(createTexture("res/ability_icons/deadeye.png"));
		break;
	case PlayerClass::SLASHER:
		cooldown1 = 1;
		cooldown2 = 10;
		cooldown3 = 15;
		player.speed = 3;
		abilityIcon1.setTexture(createTexture("res/ability_icons/smash.png"));
		abilityIcon2.setTexture(createTexture("res/ability_icons/warcry.png"));
		abilityIcon3.setTexture(createTexture("res/ability_icons/rage.png"));
		break;
	case PlayerClass::ENGINEER:
		cooldown1 = 10; 
		cooldown2 = 15;
		cooldown3 = 20;
		player.speed = 2;
		abilityIcon1.setTexture(createTexture("res/ability_icons/decoy.png"));
		abilityIcon2.setTexture(createTexture("res/ability_icons/barrel.png"));
		abilityIcon3.setTexture(createTexture("res/ability_icons/shield.png"));
		break;
	default:
		std::cout << "no class chosen" << std::endl;
		break;
	}
}

void GameMode::renderEnemies()
{
	//draw the enemies
	std::list<Enemy>::iterator enemyItr;
	for (enemyItr = enemies.begin(); enemyItr != enemies.end(); ++enemyItr) {
		Enemy& enemy = *enemyItr;
		enemy.animateFrame();
		gwindow.draw(enemy);

		if (debugging) {
			if (enemy.isOnPath()) {
				sf::CircleShape sh;
				sh.setFillColor(sf::Color::Green);
				sh.setPosition(enemy.getPosition() + sf::Vector2f({ 9.f, -20.f }));
				sh.setRadius(4);
				sh.setOutlineColor(sf::Color::Black);
				sh.setOutlineThickness(1.f);
				gwindow.draw(sh);

				sf::RectangleShape sh2;
				sh2.setFillColor(sf::Color::Transparent);
				sh2.setOutlineColor(sf::Color::Blue);
				sh2.setOutlineThickness(1.f);
				sh2.setSize({ 32, 32 });
				Node* pNode = enemy.pathHead;
				while (pNode != nullptr) {
					sh2.setPosition(sf::Vector2f(pNode->pos));
					gwindow.draw(sh2);
					pNode = pNode->parent;
				}
			}
		}

		// draw the HP bar
		sf::RectangleShape bar1({ 26.f, 6.f });
		bar1.setFillColor(sf::Color::Black);
		bar1.setPosition(enemy.getPosition().x, enemy.getPosition().y - 10);
		sf::RectangleShape bar2({ 24.f * (enemy.health / 100.f), 4.f });
		bar2.setFillColor(sf::Color::Red);
		bar2.setPosition(enemy.getPosition().x + 1, enemy.getPosition().y - 9);
		gwindow.draw(bar1);
		gwindow.draw(bar2);

	}
}
void GameMode::updateEnemies(int type) {
	// For Enemy Movement
	std::list<Enemy>::iterator enemyItr;
	for (enemyItr = enemies.begin(); enemyItr != enemies.end(); ++enemyItr)
	{
		Enemy& enemy = *enemyItr;
		
		// nearest target
		Character* nearestTarget = nullptr;

		// this is set to the max range of enemy targetting
		float minDist = 4096.f; // TODO set this constant somewhere (or make it based on enemy idk)
		
			
		// check player
		float dist = Utils::pointDistance(enemy.getPosition(), player.getPosition());
		if (dist < minDist) {
			minDist = dist;
			nearestTarget = &player;
		}

		//check NPCs
		if (type == 1)
		{
			// find the nearest target for the enemy to attack (allies and player)
			for (NPC& ally : allies) {
				// ignore dead allies
				if (!ally.alive)
					continue;

				float dist = Utils::pointDistance(enemy.getPosition(), ally.getPosition());
				float playertoCenter = Utils::pointDistance(player.getPosition(), ally.centerShield);
				float enemytoCenter = Utils::pointDistance(enemy.getPosition(), ally.centerShield);
				if (dist < minDist || playertoCenter < enemytoCenter) {
					minDist = dist;
					nearestTarget = &ally;
				}
			}
		}

		// if no target for enemy to attack or move towards, do nothing
		if (!nearestTarget) {
			enemy.attackTarget = nullptr;
			continue;
		}

		// check if target is same as old target
		if (nearestTarget != enemy.attackTarget) {
			enemy.attackTarget = nearestTarget;

			// calculate new path to enemy target
			enemy.pathClock.restart();
			enemy.findPath(tileMap, Utils::snapToTile(enemy.attackTarget->getPosition()+PLAYER_OFFSET));
		}

		// check if enemy can re path to target (only do this every so often, it's expensive)
		if (enemy.pathClock.getElapsedTime().asSeconds() >= 1.f) {
			enemy.pathClock.restart();
			enemy.findPath(tileMap, Utils::snapToTile(enemy.attackTarget->getPosition()+PLAYER_OFFSET));
		}		

		sf::Vector2f targetPosition = enemy.attackTarget->getPosition();
		sf::Vector2f enemyPosition = enemy.getPosition();

		int atkDist;

		if (nearestTarget->isShield) {
			targetPosition = nearestTarget->centerShield;
			atkDist = 80;
		}
		else {
			atkDist = 15;
		}

		sf::Vector2f difference = targetPosition - enemyPosition;
		float length = sqrt((difference.x * difference.x) + (difference.y * difference.y));


		if (length >= atkDist)
		{
			// enemy far away, move towards enemy
			if (enemy.isOnPath())
				enemy.moveOnPath(tileMap);
			else
				enemy.moveTowards(tileMap, enemy.attackTarget->getPosition());

			enemy.attack = -1; //reset attack cooldown if player moves away from attack range

			// change texture depending on enemy direction
			if (enemy.direction == 0)
				enemy.setTexture(texEnemyRight);
			else
				enemy.setTexture(texEnemyLeft);
		}
		else
		{
			//enemy is in attacking range
			enemy.cooldown(); //triggers attack timer/cooldown
			if (!enemy.attack) {
				if (player.isWarcry) {
					nearestTarget->health -= enemy.hitRate * 0.25;
					std::cout << "Reduced damage due to Warcry!" << std::endl;
				}
				else {
					nearestTarget->health -= enemy.hitRate; // deal amount of damage to player
				}
				std::cout << "target is taking damage, new health: " << nearestTarget->health << std::endl;
				if (nearestTarget->health <= 0) {
					if (type == 1)
					{
						maxEndlessScore = currentEndlessScore > maxEndlessScore ? currentEndlessScore : maxEndlessScore;
						std::cout << " Score = " << currentEndlessScore << " AND Max Endless Score = " << maxEndlessScore << "\n";
					}
					else if (type == 2) 
					{
						maxSurvivalScore = currentSurvivalScore > maxSurvivalScore ? currentSurvivalScore : maxSurvivalScore;
						std::cout << " Score = " << currentSurvivalScore << " AND Max Survival Score = " << maxSurvivalScore << "\n";
					}
					nearestTarget->alive = false;
					nearestTarget->setColor(sf::Color(255, 0, 0, 255));
					std::cout << "target has died" << std::endl;
				}
			}
		}
	}
}

void GameMode::spawnItems() {

	//initialize weapon list
	int numItems = 20; //set to 5 for testing purposes, otherwise set to rand()%3;
	//sf::Sprite& spr;
	for (int i = 0; i < numItems; ++i) {
		Item::type itemType = Item::type::null;
		int randomItem = 9;//randomly generate what item to spawn
		switch (randomItem) {//selects item type to spawn
		case 0:
			continue;
			break;
		case 1:
			itemType = Item::type::MP5;
			break;
		case 2:
			itemType = Item::type::ammo_9mm;
			break;
		case 3:
			itemType = Item::type::M4;
			break;
		case 4:
			itemType = Item::type::ammo_556;
			break;
		case 5:
			itemType = Item::type::medkit;
			break;
		case 6:
			itemType = Item::type::ammo_crate;
			break;
		case 7:
			itemType = Item::type::dagger;
			break;
		case 8:
			itemType = Item::type::baseball_bat;
			break;
		case 9:
			itemType = Item::type::walkie_talkie;
			break;
		}
		sf::Vector2f pos;
		for (;;) {
			pos = { (float)(rand() % tileMap.mapWidth * TILE_SIZE), (float)(rand() % tileMap.mapHeight * TILE_SIZE) };
			if (!tileMap.isOpaque(pos.x, pos.y))
				break;
		}
		createItem(pos, itemType);
	}
}

void GameMode::medic_bandage() {
	onCoolDown1 = true;

	//ability functionality
	createItem(player.getPosition(), Item::type::health_pack);

	//cooldown timer starts
	abilityTimer1.restart();

	//change hud
	abilityIcon1.setTexture(createTexture("res/ability_icons/health_pack_cd.png"));
}

void GameMode::medic_dash() {
	onCoolDown2 = true;

	player.speed = 20;

	abilityTimer2.restart();

	abilityIcon2.setTexture(createTexture("res/ability_icons/dash_cd.png"));
}

void GameMode::medic_heal() {
	onCoolDown3 = true;

	for (auto allyItr = allies.begin(); allyItr != allies.end(); ++allyItr) {
		NPC& ally = *allyItr;
		if (ally.alive) {
			if (ally.health >= 50) {
				ally.health = 100;
			}
			else {
				ally.health += 50;
			}
		}
	}

	abilityTimer3.restart();

	abilityIcon3.setTexture(createTexture("res/ability_icons/guardian_angel_cd.png"));
}

void GameMode::assault_ammo() {
	onCoolDown1 = true;

	createItem(player.getPosition(), Item::type::ammo_crate);
	
	abilityTimer1.restart();

	abilityIcon1.setTexture(createTexture("res/ability_icons/ammo_cd.png"));
}

void GameMode::assault_grenade() {
	onCoolDown2 = true;

	projectiles.emplace_back();

	Projectile& proj = projectiles.back();
	proj.setPosition(player.getPosition().x + 16, player.getPosition().y + 16);
	proj.setTexture(texProjectile);
	// set mask bounds to just the sprite bounds (default)
	proj.setMaskBounds(proj.getLocalBounds());
	proj.shotFrom = player.getPosition();
	proj.isGrenade = true;
	proj.speed = 4;
	proj.direction = Utils::pointDirection(player.getPosition(), mousePos);
	proj.setRotation(proj.direction);

	shotSound.setBuffer(grenadeShotBuffer);
	shotSound.setPitch(1);
	shotSound.play();

	abilityTimer2.restart();

	abilityIcon2.setTexture(createTexture("res/ability_icons/grenade_cd.png"));
}

void GameMode::assault_deadeye() {
	onCoolDown3 = true;

	player.isDeadEye = true;
	createEffect(deadEyeOpen, player.getPosition()+sf::Vector2f(10.f, -12.f));

	abilityTimer3.restart();

	abilityIcon3.setTexture(createTexture("res/ability_icons/deadeye_cd.png"));
}

void GameMode::slasher_smash() {
	onCoolDown1 = true;

	sf::Vector2f playerPos = player.getPosition();
	sf::Vector2f enemyPos;
	std::list<Enemy>::iterator enemyItr;

	for (enemyItr = enemies.begin(); enemyItr != enemies.end(); ++enemyItr) {
		enemyPos = enemyItr->getPosition();
		sf::Vector2f difference = playerPos - enemyPos;
		float length = sqrt((difference.x * difference.x) + (difference.y * difference.y));

		if (length < 100) {
			enemyItr->health -= 25;
			if (enemyItr->health <= 0) {
				currentEndlessScore += 1;
				maxEndlessScore = currentEndlessScore > maxEndlessScore ? currentEndlessScore : maxEndlessScore;
				enemyItr = enemies.erase(enemyItr);
				spawnEnemies(1);
			}
		}
	}

	abilityTimer1.restart();

	abilityIcon1.setTexture(createTexture("res/ability_icons/smash_cd.png"));
}

void GameMode::slasher_warcry() {
	onCoolDown2 = true;

	player.isWarcry = true;

	abilityTimer2.restart();

	abilityIcon2.setTexture(createTexture("res/ability_icons/warcry_cd.png"));
}

void GameMode::slasher_rage() {
	onCoolDown3 = true;

	player.isRage = true;
	player.speed = 5;

	abilityTimer3.restart();

	abilityIcon3.setTexture(createTexture("res/ability_icons/rage_cd.png"));
}

void GameMode::engineer_decoy() {
	onCoolDown1 = true;

	dropTech.setBuffer(mechNoise);
	dropTech.play();

	allies.emplace_back(texDummyRight);
	if (mousePos.x > player.getPosition().x) {
		allies.back().setPosition(player.getPosition() + sf::Vector2f({ 32.f, 0.f }));
	}else
		allies.back().setPosition(player.getPosition() + sf::Vector2f({ -32.f, 0.f }));
	
	allies.back().isDummy = true;

	abilityTimer1.restart();

	abilityIcon1.setTexture(createTexture("res/ability_icons/decoy_cd.png"));
}

void GameMode::engineer_barrel() {
	onCoolDown2 = true;

	createItem(player.getPosition(), Item::type::barrel);

	abilityTimer2.restart();

	abilityIcon2.setTexture(createTexture("res/ability_icons/barrel_cd.png"));
}

void GameMode::engineer_shield() {
	onCoolDown3 = true;

	dropTech.setBuffer(het_hon);
	dropTech.play();

	allies.emplace_back(texShield);
	allies.back().setPosition(player.getPosition() + sf::Vector2f({ -48.f, -48.f}));
	allies.back().setTextureRect({ 0,0,126,126 });
	allies.back().isDummy = true;
	allies.back().isShield = true;
	allies.back().health = 250;
	allies.back().centerShield = (allies.back().getPosition() + sf::Vector2f({ +48.f, +48.f }));

	abilityTimer3.restart();

	abilityIcon3.setTexture(createTexture("res/ability_icons/shield_cd.png"));
}

void GameMode::spawnEnemies(int noOfEnemies) {

	for (int i = 0;i < noOfEnemies;i++)
	{
		enemies.push_back(Enemy());
		Enemy& enemy = enemies.back();
		enemy.hitRate = 15;
		enemy.speed = 3;
		enemy.create(texEnemyRight, { 0, 0, 32,32 }, 8);
		enemy.setMaskBounds({ 4, 2, 17, 27 });
		for (;;) {
			int randWidth = rand() % tileMap.mapWidth * TILE_SIZE;
			int randHeight = rand() % tileMap.mapHeight * TILE_SIZE;
			enemy.setPosition(randWidth, randHeight);
			if (tileMap.areaClear(enemy, 0, 0))
				break;
		}
	}
}

void GameMode::respawnEnemies() {
	if (type == 1)
		spawnEnemies(1);
	else if (type == 2) {
		currentEnemyPresent = currentEnemyPresent - 1;
		if (currentEnemyPresent == 0) {
			//Level completed - Move to next Level
			std::cout << "Level " << currentLevel << " Completed " << "\n";
			currentLevel = currentLevel + 1;
			if (currentLevel == maxLevelCount) {
				//Survival Game End
				std::cout << "Survival Game Completed " << "\n";
			}
			else {
				currentEnemySpawningCount = currentEnemySpawningCount + 2;
				currentEnemyPresent = currentEnemySpawningCount;

				spawnEnemies(currentEnemySpawningCount);
				return;
			}
		}
	}
}

void GameMode::updateAllies() {
	for (auto allyItr = allies.begin(); allyItr != allies.end(); ++allyItr) {
		NPC& ally = *allyItr;

		// if ally dead, do nothing
		float playerToShieldCenter = Utils::pointDistance(player.getPosition(), ally.centerShield);
		if (!ally.alive) {
			allyItr = allies.erase(allyItr);
		}
		else if (ally.isShield && playerToShieldCenter > 80.f) {
			allyItr = allies.erase(allyItr);
		}
		else if (ally.isDummy) {
			continue;
		}

		// reset movement target to follow player every n ticks
		if (ally.pathClock.getElapsedTime().asSeconds() >= 1.f) {
			ally.pathClock.restart();

			// only update target if distance from player to ally > 32
			float distToPlayer = Utils::pointDistance(ally.getPosition(), player.getPosition());
			if (distToPlayer > 32.f) {
				for (;;) {
					ally.moveTarget = { player.getPosition().x + (rand() % 3 - 1) * 32, player.getPosition().y + (rand() % 3 - 1) * 32 };

					// make sure ally is not moving to same position as player
					if (sf::Vector2f(ally.moveTarget) != player.getPosition())
						break;
				}

				ally.moving = true;
				ally.findPath(tileMap, sf::Vector2i(ally.moveTarget));
			}
			else {
				ally.moving = false;
				ally.setAnimSpeed(-1);
			}
		}

		if (ally.moving) {
			if (ally.isOnPath())
				ally.moveOnPath(tileMap);
			else
				ally.moveTowards(tileMap, ally.moveTarget);
		}

		// attack nearest enemy (in range)
		if (ally.attackTick++ >= 45) {
			ally.attackTick = 0;
			// TODO set ally attack target based on nearest enemy
			const Enemy* nearestEnemy = nullptr;
			float minDistance = ally.range + 1.f;
			for (const Enemy& e : enemies) {
				float dist = Utils::pointDistance(ally.getPosition(), e.getPosition());
				if (dist < minDistance) {
					minDistance = dist;
					nearestEnemy = &e;
				}
			}

			// if there exists an enemy in range
			if (nearestEnemy) {
				// TODO attack ally's attack target
				// create projectile
				projectiles.emplace_back();
				shotSound.setBuffer(gunShotBuffer);
				shotSound.setVolume(25);
				shotSound.play();
				Projectile& proj = projectiles.back();
				proj.setPosition(ally.getPosition().x + 16, ally.getPosition().y + 16);
				proj.setTexture(texProjectile);
				// set mask bounds to just the sprite bounds (default)
				proj.setMaskBounds(proj.getLocalBounds());
				proj.speed = 12;
				proj.direction = Utils::pointDirection(ally.getPosition(), nearestEnemy->getPosition());
				proj.setRotation(proj.direction);
				proj.damage = 25;
			}
		}
	}
}

void GameMode::renderAllies() {
	for (auto allyItr = allies.begin(); allyItr != allies.end(); ++allyItr) {
		NPC& ally = *allyItr;
		ally.animateFrame();
		gwindow.draw(ally);

		// draw the HP bar
		if (ally.isShield) {
			sf::RectangleShape bar1({ 62.f, 6.f });
			bar1.setFillColor(sf::Color::Black);
			bar1.setPosition(ally.getPosition().x+32, ally.getPosition().y - 10);
			gwindow.draw(bar1);
			sf::RectangleShape bar2({ 24.f * (ally.health / 100.f), 4.f });
			bar2.setFillColor(sf::Color::Red);
			bar2.setPosition(ally.getPosition().x + 33, ally.getPosition().y - 9);
			gwindow.draw(bar2);
		}
		else {
			sf::RectangleShape bar1({ 24.f, 6.f });
			bar1.setFillColor(sf::Color::Black);
			bar1.setPosition(ally.getPosition().x, ally.getPosition().y - 10);
			gwindow.draw(bar1);
			sf::RectangleShape bar2({ 24.f * (ally.health / 100.f), 4.f });
			bar2.setFillColor(sf::Color::Red);
			bar2.setPosition(ally.getPosition().x + 1, ally.getPosition().y - 9);
			gwindow.draw(bar2);
		}
	}
}


void GameMode::loadGame(bool isLoadCall)
{
	FILE* readFile;

	char* fileNameChar = const_cast<char*>(metaFileName.c_str());
	readFile = fopen(fileNameChar, "r");

	if (readFile == NULL)
	{
		std::cout << "Cannot open meta file" << "\n";
		return;
	}
	struct GameMeta loadMeta;
	while (fread(&loadMeta, sizeof(struct GameMeta), 1, readFile))
	{
		gameMeta.survivalMeta.currentLevel = loadMeta.survivalMeta.currentLevel;
		gameMeta.survivalMeta.playerPosX = loadMeta.survivalMeta.playerPosX;
		gameMeta.survivalMeta.playerPosY = loadMeta.survivalMeta.playerPosY;
		gameMeta.survivalMeta.maxScore = loadMeta.survivalMeta.maxScore;
		//gameMeta.survivalMeta.currentMap = loadMeta.survivalMeta.currentMap;

		gameMeta.endlessMeta.playerPosX = loadMeta.endlessMeta.playerPosX;
		gameMeta.endlessMeta.playerPosY = loadMeta.endlessMeta.playerPosY;
		gameMeta.endlessMeta.maxScore = loadMeta.endlessMeta.maxScore;
		//gameMeta.endlessMeta.currentMap = loadMeta.endlessMeta.currentMap;

		/*
		for (std::vector <Tile>& tileMap : gameMeta.survivalMeta.currentMap)
		{
			for (Tile& tile : tileMap)
			{
				std::cout << "Tile Type = " << tile.type << " & Opaque = " << tile.opaque << "   ";
			}
			std::cout << "\n";
		}
		*/
		std::cout << "Survival Level = Current Score = " << gameMeta.survivalMeta.currentScore << " and Max Score = " << gameMeta.survivalMeta.maxScore << "\n";
		std::cout << "Endless Level = Current Score = " << gameMeta.endlessMeta.currentScore << " and Max Score = " << gameMeta.endlessMeta.maxScore << "\n";
		
		if (isLoadCall)
		{
			gameMeta.endlessMeta.currentScore = loadMeta.endlessMeta.currentScore;
			gameMeta.survivalMeta.currentScore = loadMeta.survivalMeta.currentScore;
			initGame();
		}
	}
	fclose(readFile);
}

void GameMode::initGame()
{
	gamestateChange = true;
	if (type == 1)//Start a new endless game state with the saved properties
	{
		game.setState(new GameMode(1, game, player.playerClass, gameMeta, true));
	}
	else if (type == 2)//Start a new endless game state with the saved properties
	{
		game.setState(new GameMode(2, game, player.playerClass, gameMeta, true));
	}
	delete this;
	return;
}

void GameMode::addHiddenArea(const sf::FloatRect& rect) {
	hiddenAreas.push_back(rect);
}

void GameMode::saveGame()
{
	FILE* writeFile;

	char* fileNameChar = const_cast<char*>(metaFileName.c_str());
	writeFile = fopen(fileNameChar, "w");

	if (writeFile == NULL)
	{
		std::cout << "Cannot open meta file" << "\n";
		return;
	}
	fwrite(&gameMeta, sizeof(struct GameMeta), 1, writeFile);
	if (fwrite != 0)
	{
		std::cout << "Successfully Saved!" << "\n";
		isLoadInvoked = false;
	}
	else
	{
		std::cout << "Error in Saving" << "\n";
	}
	fclose(writeFile);

}
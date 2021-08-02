#include "endless.hpp"
#include "utils.hpp"

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <time.h>

// the main game window
#define gwindow game.window

// NOTE: we must call the original constructor and pass it the Game pointer
EndlessState::EndlessState(Game& game, PlayerClass playerClass) :
	State(game),
	player(playerClass),
	tileMap(*this, 100, 100),
	texPlayerRight(createTexture("res/player_r_strip.png")),
	texPlayerLeft(createTexture("res/player_l_strip.png")),
	texAllyRight(createTexture("res/player2_r_strip.png")),
	texAllyLeft(createTexture("res/player2_l_strip.png")),
	texProjectile(createTexture("res/projectile.png")),
	inventory(createTexture("res/inventory.png"), createTexture("res/item_strip.png")),
	texEnemyRight(createTexture("res/enemy_r_strip.png")),
	texEnemyLeft(createTexture("res/enemy_l_strip.png")),
	texWeaponMP5(createTexture("res/mp5.png")),
	texExplosionSmall(createTexture("res/explosion_small_strip.png")),
	texExplosionLarge(createTexture("res/explosion_large.png")),
	playerIcon(createTexture("res/Player1_display.png")),
	playerDeath(createTexture("res/player_death.png")),
	ammoIcon(createTexture("res/ammo_icon.png")),
	grenadeIcon(createTexture("res/grenade_icon.png"))
{

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

	// load font
	font.loadFromFile("res/VCR_OSD_MONO.ttf");

	// load item details text
	txtItemDetails.setFont(font);
	txtItemDetails.setCharacterSize(12);
	txtItemDetails.setColor(sf::Color(0x303030FF));
	//txtItemDetails.setOutlineColor(sf::Color::Black);
	//txtItemDetails.setOutlineThickness(1);

	// load dialog GUI
	dialogBox1.setSize({480.f, 160.f});
	dialogBox1.setFillColor(sf::Color(0xf5eeceee));
	dialogBox1.setOutlineColor(sf::Color(0x000000FF));
	dialogBox1.setOutlineThickness(1.f);
	dialogBox1.setPosition({game.portWidth / 2 - 240.f, game.portHeight - 220.f});

	dialogBox2.setSize({96.f, 28.f});
	dialogBox2.setFillColor(sf::Color(0xcef5f1ee));
	dialogBox2.setOutlineColor(sf::Color(0x000000FF));
	dialogBox2.setOutlineThickness(1.f);
	dialogBox2.setPosition(dialogBox1.getPosition() + sf::Vector2f({16.f, -29.f}));

	dialogMessage.setPosition(dialogBox1.getPosition() + sf::Vector2f({4.f, 4.f}));
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
	fpsTimes.reserve(65);

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
	explosionSmall = loadEffect(texExplosionSmall, {0, 0, 8, 8}, 6, 20);
	explosionLarge = loadEffect(texExplosionLarge, { 0,0,64,64 }, 6, 20);

	// add some stuff to the inventory
	inventory.addItem(Item::type::MP5, 1);
	inventory.addItem(Item::type::ammo_9mm, 95);

	spawnEnemies(defaultEnemySpawningCount);

	spawnWeapons();

	// add ally
	allies.emplace_back(texAllyLeft);
	allies.back().setPosition(player.getPosition() + sf::Vector2f({ 32.f, 0.f }));
}

void EndlessState::updateCooldowns() {
	switch (player.playerClass) {
		case PlayerClass::DEFAULT:
			break;
		case PlayerClass::MEDIC:
			if (onCoolDown1) {
				if (elapsed1.asSeconds() < cooldown1) {
					elapsed1 = abilityTimer1.getElapsedTime();
				}
				else {
					onCoolDown1 = false;
					elapsed1 = sf::seconds(0);
				}
			}
			if (onCoolDown2) {
				if (elapsed2.asSeconds() < cooldown2) {
					elapsed2 = abilityTimer2.getElapsedTime();
					if (elapsed2.asMilliseconds() > 25) { //dash for 25 milliseconds
						player.speed = 3; //set back to default
					}
				}
				else {
					onCoolDown2 = false;
					elapsed2 = sf::seconds(0);
				}
			}
			if (onCoolDown3) {
				if (elapsed3.asSeconds() < cooldown3) {
					elapsed3 = abilityTimer3.getElapsedTime();
				}
				else {
					onCoolDown3 = false;
					elapsed3 = sf::seconds(0);
				}
			}
			break;
		case PlayerClass::ASSAULT:
			if (onCoolDown1) {
				if (elapsed1.asSeconds() < cooldown1) {
					elapsed1 = abilityTimer1.getElapsedTime();
				}
				else {
					onCoolDown1 = false;
					elapsed1 = sf::seconds(0);
				}
			}
			if (onCoolDown2) {
				if (elapsed2.asSeconds() < cooldown2) {
					elapsed2 = abilityTimer2.getElapsedTime();
				}
				else {
					onCoolDown2 = false;
					elapsed2 = sf::seconds(0);
				}
			}
			if (onCoolDown3) {
				if (elapsed3.asSeconds() < cooldown3) {
					elapsed3 = abilityTimer3.getElapsedTime();
					if (elapsed3.asSeconds() > 5) { //increase damage for 5 seconds
						player.isDeadEye = false; // turn off deadeye after 5 seconds
					}
				}
				else {
					onCoolDown3 = false;
					elapsed3 = sf::seconds(0);
				}
			}
			break;
		case PlayerClass::SLASHER:
			if (onCoolDown1) {
				if (elapsed1.asSeconds() < cooldown1) {
					elapsed1 = abilityTimer1.getElapsedTime();
				}
				else {
					onCoolDown1 = false;
					elapsed1 = sf::seconds(0);
				}
			}
			if (onCoolDown2) {
				if (elapsed2.asSeconds() < cooldown2) {
					elapsed2 = abilityTimer2.getElapsedTime();
					if (elapsed2.asMilliseconds() > 3000) { //warcry for 3 seconds
						player.isWarcry = false;
					}
				}
				else {
					onCoolDown2 = false;
					elapsed2 = sf::seconds(0);
				}
			}
			if (onCoolDown3) {
				if (elapsed3.asSeconds() < cooldown3) {
					elapsed3 = abilityTimer3.getElapsedTime();
					if (elapsed3.asMilliseconds() > 5000) { //rage for 5 seconds
						player.speed = 3; //set back to default
						player.isRage = false;
					}
				}
				else {
					onCoolDown3 = false;
					elapsed3 = sf::seconds(0);
				}
			}
			break;
	}
}

void EndlessState::medic_bandage() {
	onCoolDown1 = true;
	
	//ability functionality
	itemsOnMap.emplace_back();
	itemsOnMap.back().first = Item::type::health_pack;
	sf::Sprite& spr = itemsOnMap.back().second;
	spr.setTexture(inventory.texItemTileset);
	spr.setTextureRect(sf::IntRect(getItemTexOffset(itemsOnMap.back().first), { 48,48 }));
	spr.setScale(.5, .5);
	spr.setPosition(player.getPosition().x, player.getPosition().y);

	//cooldown timer starts
	abilityTimer1.restart();
}

void EndlessState::medic_dash() {
	onCoolDown2 = true;

	player.speed = 20;

	abilityTimer2.restart();
}

void EndlessState::medic_heal() {
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
}

void EndlessState::assault_ammo() {
	onCoolDown1 = true;

	itemsOnMap.emplace_back();
	itemsOnMap.back().first = Item::type::ammo_crate;
	sf::Sprite& spr = itemsOnMap.back().second;
	spr.setTexture(inventory.texItemTileset);
	spr.setTextureRect(sf::IntRect(getItemTexOffset(itemsOnMap.back().first), { 48,48 }));
	spr.setScale(.5, .5);
	spr.setPosition(player.getPosition().x, player.getPosition().y);

	abilityTimer1.restart();
}

void EndlessState::assault_grenade() {
	onCoolDown2 = true;

	projectiles.emplace_back();

	Projectile& proj = projectiles.back();
	proj.setPosition(player.getPosition().x + 16, player.getPosition().y + 16);
	proj.setTexture(texProjectile);
	// set mask bounds to just the sprite bounds (default)
	proj.setMaskBounds(proj.getLocalBounds());
	proj.isGrenade = true;
	proj.speed = 4;
	proj.direction = Utils::pointDirection(player.getPosition(), mousePos);
	proj.setRotation(proj.direction);

	abilityTimer2.restart();
}

void EndlessState::assault_deadeye() {
	onCoolDown3 = true;

	player.isDeadEye = true;

	abilityTimer3.restart();
}

void EndlessState::slasher_smash() {
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
				enemyItr = enemies.erase(enemyItr);
				spawnEnemies(1);
			}
		}
	}


	abilityTimer1.restart();
}

void EndlessState::slasher_warcry() {
	onCoolDown2 = true;

	player.isWarcry = true;

	abilityTimer2.restart();
}

void EndlessState::slasher_rage() {
	onCoolDown3 = true;

	player.isDeadEye = true;
	player.speed = 5;

	abilityTimer3.restart();
}

void EndlessState::chooseClass(PlayerClass playerClass) {
	switch (playerClass) {
		case PlayerClass::MEDIC:
			cooldown1 = 1; //in seconds
			cooldown2 = 3;
			cooldown3 = 5;
			player.speed = 3;
			break;
		case PlayerClass::ASSAULT:
			cooldown1 = 1; //in seconds
			cooldown2 = 3;
			cooldown3 = 5;
			player.speed = 3;
			break;
		case PlayerClass::SLASHER:
			cooldown1 = 1;
			cooldown2 = 10;
			cooldown3 = 15;
			player.speed = 3;
			break;
		default:
			std::cout << "no class chosen" << std::endl;
			break;
	}
}

void EndlessState::spawnWeapons() {
	//initialize weapon list
	int numWeapons = 5; //set to 5 for testing purposes, otherwise set to rand()%3;
	//sf::Sprite& spr;
	for (int i = 0; i < numWeapons; ++i) {
		int randomItem = rand() % 5;//randomly generate what item to spawn
		switch (randomItem) {//selects item type to spawn
			case 0:
				continue;
				break;
			case 1:
				itemsOnMap.emplace_back();
				itemsOnMap.back().first = Item::type::MP5;
				break;
			case 2:
				itemsOnMap.emplace_back();
				itemsOnMap.back().first = Item::type::ammo_9mm;
				break;
			case 3:
				itemsOnMap.emplace_back();
				itemsOnMap.back().first = Item::type::M4;
				break;
			case 4:
				itemsOnMap.emplace_back();
				itemsOnMap.back().first = Item::type::ammo_556;
			case 5:
				itemsOnMap.emplace_back();
				itemsOnMap.back().first = Item::type::medkit;
				break;
		}
		sf::Sprite& spr = itemsOnMap.back().second;
		spr.setTexture(inventory.texItemTileset);
		spr.setTextureRect(sf::IntRect(getItemTexOffset(itemsOnMap.back().first), { 48,48 }));
		spr.setScale(.5, .5);
		for (;;) {
			spr.setPosition(rand() % tileMap.mapWidth * TILE_SIZE, rand() % tileMap.mapHeight * TILE_SIZE);
			if (!tileMap.isOpaque(spr.getPosition().x, spr.getPosition().y))
				break;
		}
	}
}

void EndlessState::spawnEnemies(int noOfEnemies) {
	int soundNum;
	for (int i = 0;i < noOfEnemies;i++)
	{
		soundNum = rand() % 5;
		switch (soundNum) {//decides which zombie sound will be played on spawn
		case 0:
			zombieSound.setBuffer(zombieBuffer1);
			break;
		case 1:
			zombieSound.setBuffer(zombieBuffer2);
			break;
		case 2:
			zombieSound.setBuffer(zombieBuffer3);
			break;
		case 3:
			zombieSound.setBuffer(zombieBuffer4);
			break;
		case 4:
			zombieSound.setBuffer(zombieBuffer5);
			break;
		default:
			zombieSound.setBuffer(zombieBuffer1);
			break;
		}
		zombieSound.setVolume(25);
		zombieSound.play();

		Enemy enemy;
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
		enemies.push_back(enemy);
	}
}

bool EndlessState::handleEvents() {
	// handle events
	sf::Event e;
	while (game.window.pollEvent(e)) {
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
						std::cout << "Medic Ability - Dropped Bandages" << std::endl;
					}
					else {
						std::cout << "Medic Ability - Bandages are on cooldown" << std::endl;
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
			case sf::Keyboard::F2:
				// restarts the map
				game.setState(new EndlessState(game, player.playerClass));
				delete this;
				return false;
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
			case sf::Keyboard::F1:
				std::cout << allies.front().moveTarget.x << ", " << allies.front().moveTarget.x << std::endl;
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
				// equip item
				if (showInventory)
					inventory.wieldItemAt(winMousePos.x, winMousePos.y);

				break;
			}
			break;
		case sf::Event::MouseWheelScrolled:
			if (e.mouseWheelScroll.delta > 0) {
				std::cout << "up" << std::endl;
				// mouse scrolling up
				mainView.zoom(.9f);
			}
			else {
				std::cout << "down" << std::endl;
				// mouse scrolling down
				mainView.zoom(1.1f);
			}
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
			shotSound.play();
			// TODO - check to make sure weapon is ranged
			projectiles.emplace_back();
			Projectile& proj = projectiles.back();
			proj.setPosition(player.getPosition().x + 16, player.getPosition().y + 16);
			proj.setTexture(texProjectile);
			// set mask bounds to just the sprite bounds (default)
			proj.setMaskBounds(proj.getLocalBounds());
			proj.speed = 12;
			proj.direction = Utils::pointDirection({ player.getPosition().x + 16, player.getPosition().y + 16 }, mousePos);
			proj.setRotation(proj.direction);
			proj.damage = inventory.getWielded().getDamage() * (2*player.isDeadEye);
		}
		else if (inventory.getWielded().getAmmoType() != Item::type::null && inventory.getRoundsLeft() == 0) {
			if (shotSound.getStatus() != sf::Sound::Status::Playing) {
				shotSound.setBuffer(emptyGunBuffer);
				shotSound.setVolume(25);
				shotSound.play();
			}
		}
	}

	// tell game state to continue
	return true;
}

void EndlessState::updateEnemies() {
	// For Enemy Movement
	std::list<Enemy>::iterator enemyItr;
	for (enemyItr = enemies.begin(); enemyItr != enemies.end(); ++enemyItr)
	{
		Enemy& enemy = *enemyItr;
		
		// nearest target to enemy
		Character* nearestTarget = nullptr;
		// this is set to the max range of enemy attacks
		float minDist = 512.f; // TODO set this constant somewhere (or make it based on enemy idk)
		// find the nearest target for the enemy to attack (allies and player)
		for (NPC& ally: allies) {
			// ignore dead allies
			if (!ally.alive)
				continue;
				
			float dist = Utils::pointDistance(enemy.getPosition(), ally.getPosition());
			if (dist < minDist) {
				minDist = dist;
				nearestTarget = &ally;
			}
		}
		// check player
		{
			float dist = Utils::pointDistance(enemy.getPosition(), player.getPosition());
			if (dist < minDist) {
				minDist = dist;
				nearestTarget = &player;
			}
		}

		// if no target for enemy to attack, do nothing
		if (!nearestTarget)
			continue;

		sf::Vector2f targetPosition = nearestTarget->getPosition();
		sf::Vector2f enemyPosition = enemy.getPosition();

		sf::Vector2f difference = targetPosition - enemyPosition;
		float length = sqrt((difference.x * difference.x) + (difference.y * difference.y));

		if (length >= 15)
		{
			sf::Vector2f moveVector = sf::Vector2f(difference.x / length, difference.y / length);
			enemy.setAnimSpeed(12);

			// move when free
			if (tileMap.areaClear(enemy, moveVector.x, 0))
				enemy.move(moveVector.x, 0);
			if (tileMap.areaClear(enemy, 0, moveVector.y))
				enemy.move(0, moveVector.y);

			enemy.attack = -1; //reset attack cooldown if player moves away from attack range

			// change texture depending on enemy directionaa
			if (moveVector.x < 0)
				enemy.setTexture(texEnemyLeft);
			else
				enemy.setTexture(texEnemyRight);
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
					nearestTarget->alive = false;
					nearestTarget->setColor(sf::Color(255, 0, 0, 255));
					std::cout << "target has died" << std::endl;
				}
			}
		}
	}
}

void EndlessState::renderEnemies()
{
	//draw the enemies
	std::list<Enemy>::iterator enemyItr;
	for (enemyItr = enemies.begin(); enemyItr != enemies.end(); ++enemyItr) {
		Enemy& enemy = *enemyItr;
		enemy.animateFrame();
		gwindow.draw(enemy);

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

void EndlessState::updateAllies() {
	for (auto allyItr = allies.begin(); allyItr != allies.end(); ++allyItr) {
		NPC& ally = *allyItr;

		// if ally dead, do nothing
		if (!ally.alive)
			continue;
		
		// reset movement target to follow player every n ticks
		if (ally.updateTick++ >= 60) {
			ally.updateTick = 0;

			// only update target if distance from player to ally > 32
			float distToPlayer = Utils::pointDistance(ally.getPosition(), player.getPosition());
			if (distToPlayer > 32.f) {
				for (;;) {
					ally.moveTarget = { player.getPosition().x + (rand() % 3 - 1) * 32, player.getPosition().y + (rand() % 3 - 1) * 32 };

					// make sure ally is not moving to same position as player
					if (ally.moveTarget != player.getPosition())
						break;
				}
			}
		}

		// move ally towards movement target
		sf::Vector2f difference = ally.moveTarget - ally.getPosition();
		float length = sqrt((difference.x * difference.x) + (difference.y * difference.y));

		if (length >= 4) {
			sf::Vector2f moveVector = sf::Vector2f(difference.x / length, difference.y / length);
			ally.setAnimSpeed(12);
			ally.move(moveVector);
		}
		else {
			ally.setAnimSpeed(-1);
			ally.setIndex(0);
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
			}
		}
	}
}

void EndlessState::renderAllies() {
	for (auto allyItr = allies.begin(); allyItr != allies.end(); ++allyItr) {
		NPC& ally = *allyItr;
		ally.animateFrame();
		gwindow.draw(ally);

		// draw the HP bar
		sf::RectangleShape bar1({ 26.f, 6.f });
		bar1.setFillColor(sf::Color::Black);
		bar1.setPosition(ally.getPosition().x, ally.getPosition().y - 10);
		sf::RectangleShape bar2({ 24.f * (ally.health / 100.f), 4.f });
		bar2.setFillColor(sf::Color::Red);
		bar2.setPosition(ally.getPosition().x + 1, ally.getPosition().y - 9);
		gwindow.draw(bar1);
		gwindow.draw(bar2);
	}
}

void EndlessState::updateProjectiles() {
	bool breaking = false;
	for (auto projItr = projectiles.begin(); projItr != projectiles.end(); projItr++) {
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
					tileMap.getTileBounds(projItr->getPosition().x + 2.5f*moveVector.x, projItr->getPosition().y + 2.5f*moveVector.y)
				)
			);
			// destroy projectile
			projItr = projectiles.erase(projItr);
			if (projItr == projectiles.end())
				break;
			continue;
		}

		if (projItr->isGrenade == true) {
			float maxRange = 250.f;
			float dist = Utils::pointDistance(player.getPosition(), projItr->getPosition());
			
			if (dist > maxRange || !(tileMap.areaClear(*projItr, moveVector))) {
				createEffect(explosionLarge, projItr->getPosition());
				for (auto enemyItr = enemies.begin(); enemyItr != enemies.end(); enemyItr++) {
					float distToProj = Utils::pointDistance(enemyItr->getPosition(), projItr->getPosition());
					if (distToProj <= 150) {
						enemyItr->health -= 120;
						if (enemyItr->health <= 0) {
							enemyItr = enemies.erase(enemyItr);
							spawnEnemies(1);
						}
					}
				}
				projItr = projectiles.erase(projItr);
				if (projItr == projectiles.end())
					break;
				continue;
			}
		}
		else {
			// check for collision with enemies
			// TODO - make enemies use a spatial hash so this algo's faster
			// this algo is currently O(K*N) where K = bullets, N = enemies
			for (auto enemyItr = enemies.begin(); enemyItr != enemies.end(); enemyItr++) {
				// ignore if enemy is not colliding with projectile
				if (!enemyItr->isColliding(*projItr))
					continue;

				// destroy bullet
				projItr = projectiles.erase(projItr);
				if (projItr == projectiles.end())
					breaking = true;

				// deal damage to enemy
				if (player.isDeadEye) {
					enemyItr->health -= 50; //is deadeye is active double damage
				}
				else if (player.isRage) {
					enemyItr->health -= projItr->damage * 2;
				}
				else {
					enemyItr->health -= 25;// // TODO set this to the bullet's damage
				}
				if (enemyItr->health <= 0) {
					enemyItr = enemies.erase(enemyItr);
					spawnEnemies(1);
					if (enemyItr == enemies.end())
						break;
				}
				if (breaking)
					break;
			}
			if (breaking)
				break;
		}
	}
}

void EndlessState::setDialog(const std::string& speaker, const std::string& msg) {
	showDialog = true;
	dialogSpeaker.setString(speaker + ':');
	dialogMessage.setString(msg);
}

void EndlessState::hideDialog() {
	showDialog = false;
}

EndlessState::~EndlessState() {
	// here we would deallocate any resources we use in this gamestate
}

void EndlessState::logic() {
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

			txtItemDetails.setString(item->getName());
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
		if (player.getAnimSpeed() == -1)
			player.setAnimSpeed(12);
		if (player.movingLeft)
			player.setTexture(texPlayerLeft);
		if (player.movingRight)
			player.setTexture(texPlayerRight);
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
	updateEnemies();

	// update all allies
	updateAllies();

	// update ability cooldowns
	updateCooldowns();

	// Player HUD
	//  
	//HP bar
	playerHPBack.setSize({ 200.f, 14.f });
	playerHPBack.setFillColor(sf::Color::Transparent);
	playerHPBack.setPosition(60, 725);
	playerHPBack.setOutlineColor(sf::Color::Black);
	playerHPBack.setOutlineThickness(2.f);
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
	playerHPBar.setPosition(playerHPBack.getPosition().x, playerHPBack.getPosition().y);
	playerIcon.setPosition(playerHPBack.getPosition().x - 62, playerHPBack.getPosition().y - 25);
	//mag and total ammo counter;
	ammoIcon.setPosition(playerHPBack.getPosition().x, playerHPBack.getPosition().y - 25);
	ammoCount.setFont(font);
	ammoCount.setCharacterSize(12);
	ammoCount.setColor(sf::Color::Black);
	if (inventory.getWielded().getAmmoType() == Item::type::null)
		ammoCount.setString("-/-");
	else
		ammoCount.setString(std::to_string(inventory.getRoundsLeft()) + "/" + std::to_string(inventory.getNumItem(inventory.getWielded().getAmmoType())));
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

	// set FPS for this tick
	float currentTime = fpsClock.restart().asSeconds();
	float fps = 1.f / currentTime;
	fpsTimes.push_back(fps);
	if (fpsTimer.getElapsedTime().asSeconds() >= .2f) {
		float fpsAvg = 0;
		for (float i : fpsTimes)
			fpsAvg += i;
		fpsAvg /= fpsTimes.size();
		fpsCounter.setString("FPS: " + std::to_string((int)fpsAvg));
		fpsTimer.restart();
		fpsTimes.resize(0);
	}

	inventory.tick();
}

void EndlessState::render() {
	// clear window - default color black
	gwindow.clear();

	// ========================== //
	// = v   world drawing   v  = //
	// ========================== //

	// move view target to center on player
	mainViewTarget = { floor(player.getPosition().x), floor(player.getPosition().y) };

	// move the view towards target
	sf::Vector2f delta(floor((mainViewTarget.x - mainView.getCenter().x)/10), floor((mainViewTarget.y - mainView.getCenter().y)/10));
	mainView.move(delta);

	// we must update view any time we change something in it
	// set the main view to draw the main map
	gwindow.setView(mainView);

	// draw the tilemap
	gwindow.draw(tileMap);

	//draw the weapons
	for (auto item : itemsOnMap) {
		gwindow.draw(item.second);
	}

	// draw the player
	player.animateFrame();
	gwindow.draw(player);

	// draw the projectiles
	for (Projectile& proj : projectiles) {
		gwindow.draw(proj);
	}

	// draw the enemies
	renderEnemies();

	// draw the allies
	renderAllies();

	// draw effects
	drawEffects();

	// ========================= //
	// =  v   GUI drawing   v  = //
	// ========================= //

	// set view to draw guis
	gwindow.setView(guiView);

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
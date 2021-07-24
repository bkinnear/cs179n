#include "endless.hpp"
#include "utils.hpp"

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <time.h>

// the main game window
#define gwindow game.window

int magCount = 30;//temporarily set string until magazine ammo counter is made
int totalCount = 300;//temporarily set string until total ammo counter is made

// NOTE: we must call the original constructor and pass it the Game pointer
EndlessState::EndlessState(Game& game, PlayerClass playerClass) :
	State(game),
	tileMap(createTexture("res/big_32x32_tileset.png"), 30, 20),
	texPlayerRight(createTexture("res/player_r_strip.png")),
	texPlayerLeft(createTexture("res/player_l_strip.png")),
	texProjectile(createTexture("res/projectile.png")),
	inventory(createTexture("res/inventory.png"), createTexture("res/item_strip.png")),
	texEnemyRight(createTexture("res/enemy_r_strip.png")),
	texEnemyLeft(createTexture("res/enemy_l_strip.png")),
	texWeaponMP5(createTexture("res/mp5.png")),
	texExplosionSmall(createTexture("res/explosion_small_strip.png")),
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

	// load font
	font.loadFromFile("res/VCR_OSD_MONO.ttf");

	// load item details text
	txtItemDetails.setFont(font);
	txtItemDetails.setCharacterSize(12);
	txtItemDetails.setColor(sf::Color(0x303030FF));
	//txtItemDetails.setOutlineColor(sf::Color::Black);
	//txtItemDetails.setOutlineThickness(1);

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

	// add some stuff to the inventory
	inventory.addItem(Item::type::MP5, 1);
	inventory.addItem(Item::type::ammo_9mm, 95);

	spawnEnemies(defaultEnemySpawningCount);

	spawnWeapons();

	// add ally
	allies.emplace_back(texPlayerLeft);
	allies.back().setPosition(player.getPosition() + sf::Vector2f({ 32.f, 0.f }));
}

void EndlessState::chooseClass(PlayerClass playerClass) {
	switch (playerClass) {
		case PlayerClass::MEDIC:
			// do medic stuff
			break;
		case PlayerClass::ASSAULT:
			// do assault stuff
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
			break;
		}
		sf::Sprite& spr = itemsOnMap.back().second;
		spr.setTexture(inventory.texItemTileset);
		spr.setTextureRect(sf::IntRect(getItemTexOffset(itemsOnMap.back().first), { 48,48 }));
		spr.setScale(.5, .5);
		for (;;) {
			spr.setPosition(rand() % 800, rand() % 600);
			if (!tileMap.isOpaque(spr.getPosition().x, spr.getPosition().y))
				break;
		}
	}
}

void EndlessState::spawnEnemies(int noOfEnemies) {
	for (int i = 0;i < noOfEnemies;i++)
	{
		Enemy enemy;
		enemy.hitRate = 15;
		enemy.speed = 3;
		enemy.create(texEnemyRight, { 0, 0, 32,32 }, 8);
		enemy.setMaskBounds({ 4, 2, 17, 27 });
		for (;;) {
			// TODO set range to world_width and world_height instead of magic numbers
			int randWidth = rand() % 800;
			int randHeight = rand() % 600;
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
				bool isDoor = tileMap.isDoor(position.x+16, position.y - 16);
				if (isDoor)
				{
					x = position.x + 16;
					y = position.y - 16;
				}
				else
				{
					isDoor = tileMap.isDoor(position.x+16, position.y + 48);
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
			case sf::Keyboard::F2:
				// restarts the map
				game.setState(new EndlessState(game, player.playerClass));
				delete this;
				return false;
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

				// create projectiles
				projectiles.emplace_back();

				// TODO - implement mags
				{
					unsigned nRounds = inventory.getNumItem(Item::type::ammo_9mm);
					if (nRounds > 0) {
						inventory.removeItem(Item::type::ammo_9mm, 1);

						Projectile& proj = projectiles.back();
						proj.setPosition(player.getPosition().x + 16, player.getPosition().y + 16);
						proj.setTexture(texProjectile);
						// set mask bounds to just the sprite bounds (default)
						proj.setMaskBounds(proj.getLocalBounds());
						proj.speed = 12;
						proj.direction = Utils::pointDirection(player.getPosition(), mousePos);
						proj.setRotation(proj.direction);
					}
				}
				break;
			case sf::Mouse::Button::Right:
				// RMB pressed
				// 
				// equip item
				if (showInventory)
					inventory.wieldItemAt(winMousePos.x, winMousePos.y);

				break;
			}
		}
	}

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
				nearestTarget->health -= enemy.hitRate; // deal amount of damage to player
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
			enemyItr->health -= 25; // TODO set this to the bullet's damage
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

EndlessState::~EndlessState() {
	// here we would deallocate any resources we use in this gamestate
}

void EndlessState::logic() {
	// get mouse x and y in window coords - used for GUI
	gwindow.setView(guiView);
	winMousePos = sf::Mouse::getPosition(game.window);

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

	// Player HUD
	//  
	//HP bar
	playerHPBack.setSize({ 200.f, 14.f });
	playerHPBack.setFillColor(sf::Color::Transparent);
	playerHPBack.setPosition(75, 550);
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
	ammoCount.setString(std::to_string(1) + "/" + std::to_string(inventory.getNumItem(Item::type::ammo_9mm))); // TODO implement mags and more ammo types
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
}

void EndlessState::render() {
	// clear window
	gwindow.clear(sf::Color(0x40AA20FF));

	// ========================== //
	// = v   world drawing   v  = //
	// ========================== //

	// we must update view any time we change something in it
	// set the main view to draw the main map
	gwindow.setView(mainView);

	// draw the tilemap
	gwindow.draw(tileMap);

	//draw player HUD
	gwindow.draw(playerHPBack);
	gwindow.draw(playerHPBar);
	gwindow.draw(playerIcon);
	gwindow.draw(ammoIcon);
	gwindow.draw(ammoCount);
	gwindow.draw(grenadeIcon);
	gwindow.draw(grenadesNum);

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

	// update window
	gwindow.display();
}
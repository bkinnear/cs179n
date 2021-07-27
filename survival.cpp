#include "survival.hpp"
#include "utils.hpp"

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <time.h>


// the main game window
#define gwindow game.window


// NOTE: we must call the original constructor and pass it the Game pointer
SurvivalState::SurvivalState(Game& game, PlayerClass playerClass) :
	State(game),
	player(playerClass),
	tileMap(*this, 100, 100),
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

	// load effects
	explosionSmall = loadEffect(texExplosionSmall, { 0, 0, 8, 8 }, 6, 20);

	// add some stuff to the inventory
	inventory.addItem(Item::type::MP5, 1);
	inventory.addItem(Item::type::ammo_9mm, 95);

	GameMode::spawnEnemies(currentEnemySpawningCount, texEnemyRight, enemies, tileMap);
	GameMode::spawnWeapons(itemsOnMap, inventory, tileMap);

}

bool SurvivalState::handleEvents() {
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
			case sf::Keyboard::Num1: //FIRST ABILITY
				switch (player.playerClass) {
				case PlayerClass::DEFAULT:
					break;
				}
				break;
			case sf::Keyboard::Num2: //SECOND ABILITY
				switch (player.playerClass) {
				case PlayerClass::DEFAULT:
					break;
				}
				break;
			case sf::Keyboard::Num3: //THIRD ABILITY
				switch (player.playerClass)
			case PlayerClass::DEFAULT:
				break;
				break;
			case sf::Keyboard::F2:
				// restarts the map
				game.setState(new SurvivalState(game, player.playerClass));
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
	return true;
}

void SurvivalState::updateProjectiles() {
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
					tileMap.getTileBounds(projItr->getPosition().x + 2.5f * moveVector.x, projItr->getPosition().y + 2.5f * moveVector.y)
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
				currentEnemyPresent = currentEnemyPresent - 1;
				if (currentEnemyPresent == 0)
				{
					//Level completed - Move to next Level
					std::cout << "Level " << currentLevel << " Completed " << "\n";
					currentLevel = currentLevel + 1;
					if (currentLevel == maxLevelCount)
					{
						//Survival Game End
						std::cout << "Survival Game Completed " << "\n";
					}
					else
					{
						currentEnemySpawningCount = currentEnemySpawningCount + 2;
						currentEnemyPresent = currentEnemySpawningCount;

						GameMode::spawnEnemies(currentEnemySpawningCount, texEnemyRight, enemies, tileMap);
						GameMode::renderEnemies(enemies, gwindow);
						return;
					}
				}
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

SurvivalState::~SurvivalState() {
	// here we would deallocate any resources we use in this gamestate
}

void SurvivalState::logic() {
	// get mouse x and y in window coords - used for GUI
	gwindow.setView(guiView);
	winMousePos = sf::Mouse::getPosition(game.window);

	// get mouse x and y in world coords
	gwindow.setView(mainView);
	mousePos = game.window.mapPixelToCoords(sf::Mouse::getPosition(gwindow));

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
	std::list<NPC> allies;
	GameMode::updateEnemies(2, allies, texEnemyRight, texEnemyLeft, enemies, tileMap, player);

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

void SurvivalState::render() {
	// clear window - default color black
	gwindow.clear();

	// ========================== //
	// = v   world drawing   v  = //
	// ========================== //

	// move view to center on player
	mainView.setCenter(floor(player.getPosition().x), floor(player.getPosition().y));
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
	GameMode::renderEnemies(enemies, gwindow);

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

	// update window
	gwindow.display();
}
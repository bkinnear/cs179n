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
EndlessState::EndlessState(Game& game) :
	State(game),
	tileMap(createTexture("res/big_32x32_tileset.png"), 30, 20),
	texPlayerRight(createTexture("res/player_r_strip.png")),
	texPlayerLeft(createTexture("res/player_l_strip.png")),
	texProjectile(createTexture("res/projectile.png")),
	inventory(createTexture("res/inventory.png"), createTexture("res/item_strip.png")),
	texEnemyRight(createTexture("res/enemy_r_strip.png")),
	texEnemyLeft(createTexture("res/enemy_l_strip.png"))
{

	// set main view
	mainView.reset({ 0.f, 0.f, float(gwindow.getSize().x), float(gwindow.getSize().y) });
	guiView.reset({ 0.f, 0.f, float(gwindow.getSize().x), float(gwindow.getSize().y) });

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
	player.create(texPlayerRight, { 0, 0, 32, 32 }, 4);
	player.speed = 2;

	// add some stuff to the inventory
	inventory.addItem(Item::type::MP5, 1);
	inventory.addItem(Item::type::ammo_9mm, 95);

	spawnEnemies(defaultEnemySpawningCount);
}

void EndlessState::spawnEnemies(int noOfEnemies)
{
	for (int i = 0;i < noOfEnemies;i++)
	{
		Enemy enemy;
		enemy.hitRate = 15;
		enemy.speed = 3;
		enemy.create(texEnemyRight, { 0, 0, 32,32 }, 4);
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

void EndlessState::renderEnemies(int noOfEnemies)
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

EndlessState::~EndlessState() {
	// here we would deallocate any resources we use in this gamestate
}

void EndlessState::logic() {
	// get mouse x and y in window coords - used for GUI
	gwindow.setView(guiView);
	sf::Vector2i winMousePos = sf::Mouse::getPosition(game.window);

	// get mouse x and y in world coords
	gwindow.setView(mainView);
	sf::Vector2f mousePos = game.window.mapPixelToCoords(sf::Mouse::getPosition(gwindow));


	// handle events
	sf::Event e;
	while (game.window.pollEvent(e)) {
		switch (e.type) {
		case sf::Event::Closed:
			// delete this gamestate
			game.close();
			return;
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
				bool isDoor = tileMap.isDoor(position.x, position.y - 32);
				if (isDoor)
				{
					x = position.x;
					y = position.y - 32;
				}
				else
				{ 
					isDoor = tileMap.isDoor(position.x, position.y + 32);
					if (isDoor)
					{
						x = position.x;
						y = position.y + 32;
					}
				}
				if (isDoor)
				{
					x = x / TILE_SIZE;
					y = y / TILE_SIZE;
					switch (tileMap.getTileType(x, y))
					{
						case 30:
							//Closed Door Type - 1
							tileMap.setTile(x, y, 31);
							break;
						case 32:
							//Closed Door Type - 2
							tileMap.setTile(x, y, 33);
							break;
						case 31:
							//Opened Door Type - 1
							tileMap.setTile(x, y, 30);
							break;
						case 33:
							//Opened Door Type - 2
							tileMap.setTile(x, y, 32);
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
				game.setState(new EndlessState(game));
				delete this;
				return;
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
			case sf::Keyboard::Space:
			case sf::Keyboard::Q:
				break;
			}
			break;
		case sf::Event::MouseButtonPressed:
			switch (e.mouseButton.button) {
			case sf::Mouse::Button::Left:
				// LMB pressed

				// create projectiles
				projectiles.emplace_back();
				{
					Projectile& proj = projectiles.back();
					proj.setPosition(player.getPosition().x + 16, player.getPosition().y + 16);
					proj.setTexture(texProjectile);
					proj.speed = 12;
					proj.direction = Utils::pointDirection(player.getPosition(), mousePos);
					proj.setRotation(proj.direction);
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
	// TODO fix movement to make opaque tiles non passable (check every corner of sprite for collision, not just top & left)
	const sf::FloatRect& bounds = player.getGlobalBounds();

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
	bool end = false;
	for (auto projItr = projectiles.begin(); projItr != projectiles.end(); projItr++) {
		// get movement of projectile for this frame
		sf::Vector2f moveVector = Utils::vectorInDirection(projItr->speed, projItr->direction);
		if (tileMap.areaClear(*projItr, moveVector)) {
			projItr->move(moveVector);
		}
		else {
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
			if (enemyItr->getGlobalBounds().intersects(projItr->getGlobalBounds())) {
				// enemy hit
				enemyItr->health -= 25; // TODO set this to the bullet's damage
				if (enemyItr->health <= 0) {
					enemyItr = enemies.erase(enemyItr);
					spawnEnemies(1);
					renderEnemies(1);
					if (enemyItr == enemies.end())
						break;
				}

				// destroy bullet
				projItr = projectiles.erase(projItr);
				if (projItr == projectiles.end()) {
					end = true;
					break;
				}
			}
		}

		if (end)
			break;
	}

	//For Enemy Movement
	std::list<Enemy>::iterator enemyItr;
	for (enemyItr = enemies.begin(); enemyItr != enemies.end(); ++enemyItr)
	{
		Enemy& enemy = *enemyItr;
		sf::Vector2f playerPosition = player.getPosition();
		sf::Vector2f enemyPosition = enemy.getPosition();

		sf::Vector2f difference = playerPosition - enemyPosition;
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

			// change texture depending on enemy direction
			if (moveVector.x < 0)
				enemy.setTexture(texEnemyLeft);
			else
				enemy.setTexture(texEnemyRight);
		}
		else
		{
			//enemy is in attacking range
			enemy.cooldown(); //triggers attack timer/cooldown
			if (player.alive && !enemy.attack) {
				player.health -= enemy.hitRate; // deal amount of damage to player
				std::cout << "player is taking damage, new health: " << player.health << std::endl;
				if (player.health <= 0) {
					player.alive = false;
					player.setColor(sf::Color(255, 0, 0, 255));
					std::cout << "player has died" << std::endl;
				}
			}
		}
	}
}

void EndlessState::render() {
	// clear window
	gwindow.clear(sf::Color(0x40AA20FF));

	// we must update view any time we change something in it
	// set the main view to draw the main map
	gwindow.setView(mainView);

	// draw the tilemap
	gwindow.draw(tileMap);

	// draw the HP bar
	sf::RectangleShape bar1({ 26.f, 6.f });
	bar1.setFillColor(sf::Color::Black);
	bar1.setPosition(player.getPosition().x, player.getPosition().y - 10);
	sf::RectangleShape bar2({ 24.f * (player.health / 100.f), 4.f });
	bar2.setFillColor(sf::Color::Red);
	bar2.setPosition(player.getPosition().x + 1, player.getPosition().y - 9);
	gwindow.draw(bar1);
	gwindow.draw(bar2);

	// draw the player
	player.animateFrame();
	gwindow.draw(player);

	// draw the projectiles
	for (Projectile& proj : projectiles) {
		gwindow.draw(proj);
	}

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

	renderEnemies(enemies.size());

	// update window
	gwindow.display();
}

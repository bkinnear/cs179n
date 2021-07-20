#include "endless.hpp"

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <time.h>

#define _PI 3.14159265f

// the main game window
#define gwindow game.window

// returns direction (deg) from point A to point B
float pointDirection(sf::Vector2f A, sf::Vector2f B) {
	return std::atan2f(B.y - A.y, B.x - A.x) * 180.f / _PI;
}

// returns vector of given length in given direction (deg)
sf::Vector2f vectorInDirection(float length, float direction) {
	return { length * std::cosf(direction * _PI / 180.f), length * std::sinf(direction * _PI / 180.f) };
}

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
	player.setMaskBounds({ 6, 2, 18, 27 });
	player.speed = 2;

	// add some stuff to the inventory
	inventory.addItem(Item::type::MP5, 1);
	inventory.addItem(Item::type::ammo_9mm, 95);

	int limit = currentLevel * 3;
	srand(time(0));
	for(int i = 0;i < limit;i++)
	{
		Enemy enemy;
		enemy.hitRate = currentLevel * 0.5;
		enemy.speed = currentLevel + 0.5;
		enemy.create(texEnemyRight, { 0, 0, 32,32 }, 4);
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
			spr.setPosition(rand() % 800, rand() % 600);
			if (!tileMap.isOpaque(spr.getPosition().x, spr.getPosition().y))
				break;
		}
		
	}
	std::cout << itemsOnMap.size() << std::endl;

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
					// set mask bounds to just the sprite bounds (default)
					proj.setMaskBounds(proj.getLocalBounds());
					proj.speed = 12;
					proj.direction = pointDirection(player.getPosition(), mousePos);
					proj.setRotation(proj.direction);
				}
				break;
			case sf::Mouse::Button::Right:
				// RMB pressed

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
		sf::Vector2f moveVector = vectorInDirection(projItr->speed, projItr->direction);
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
			if (enemyItr->isColliding(*projItr)) {
				// enemy hit
				enemyItr->health -= 25; // TODO set this to the bullet's damage
				if (enemyItr->health <= 0) {
					enemyItr = enemies.erase(enemyItr);
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
			if (player.alive && !enemy.attack) {
				player.health -= 15; // deal amount of damage to player
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

	//draw the weapons
	for (auto item : itemsOnMap) {
		gwindow.draw(item.second);
	}

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


	// update window
	gwindow.display();
}

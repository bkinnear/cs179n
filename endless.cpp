#include "endless.hpp"

#include <iostream>
#include <cstdlib>
#include <vector>
#include <time.h>

// the main game window
#define gwindow game.window

// NOTE: we must call the original constructor and pass it the Game pointer
EndlessState::EndlessState(Game& game) :
	State(game),
	tileMap(createTexture("res/big_32x32_tileset.png"), 20, 20),
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

	projectile.create(texProjectile, { (int)player.getPosition().y, (int)player.getPosition().x, 32, 32 }, 2);
	projectile.setIndex(0);

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
		bool placed = false;
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
			case sf::Keyboard::Space:
			case sf::Keyboard::Q:
				projectile.setIndex(1);
				projectile.shoot = true;
				projectile.move(projectile.speed, 0);
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
	// TODO fix movement to make opaque tiles non passable (check every corner of sprite for collision, not just top & left)
	const sf::FloatRect& bounds = player.getGlobalBounds();

  if (player.alive) {
    if (player.movingLeft)
      if (tileMap.areaClear(player, -player.speed, 0)) {
        if (projectile.shoot == false) {
          projectile.setPosition(player.getPosition().x, player.getPosition().y);
        }
        player.move(-player.speed, 0);
      }
    if (player.movingUp)
      if (tileMap.areaClear(player, 0, -player.speed)) {
        if (projectile.shoot == false) {
          projectile.setPosition(player.getPosition().x, player.getPosition().y);
        }
        player.move(0, -player.speed);
      }
    if (player.movingRight)
      if (tileMap.areaClear(player, player.speed, 0)) {
        if (projectile.shoot == false) {
          projectile.setPosition(player.getPosition().x, player.getPosition().y);
        }
        player.move(player.speed, 0);
      }	
    if (player.movingDown)
      if (tileMap.areaClear(player, 0, player.speed)) {
        if (projectile.shoot == false) {
          projectile.setPosition(player.getPosition().x, player.getPosition().y);
        }
        player.move(0, player.speed);
      }

    if (projectile.shoot)
    {
      if (tileMap.areaClear(projectile, 0, projectile.speed)) {
        projectile.move(projectile.speed, 0);
      }
      else {
        projectile.setPosition(player.getPosition().x, player.getPosition().y);
        projectile.shoot = false;
        projectile.setIndex(0);
      }
	  }
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
				player.health -= 5;
				std::cout << "player is taking damage, new health: " << player.health << std::endl;
				if (player.health == 0) {
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

	// draw the player
	player.animateFrame();
	gwindow.draw(player);

  // draw the projectile
	projectile.animateFrame();
	gwindow.draw(projectile);

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
	}

	// update window
	gwindow.display();
}

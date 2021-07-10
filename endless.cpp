#include "endless.hpp"

#include <iostream>
#include <cstdlib>
#include <time.h>

// the main game window
#define gwindow game.window

// NOTE: we must call the original constructor and pass it the Game pointer
EndlessState::EndlessState(Game& game) :
	State(game),
	tileMap(createTexture("res/big_32x32_tileset.png"), 20, 20),
	texPlayerRight(createTexture("res/player_r_strip.png")),
	texPlayerLeft(createTexture("res/player_l_strip.png")),
	texEnemyLeft(createTexture("res/player_l_strip.png")),
	texEnemyRight(createTexture("res/player_r_strip.png"))
{
	// set view
	view.reset({ 0.f, 0.f, float(gwindow.getSize().x), float(gwindow.getSize().y) });
	// we must update view any time we change something in it
	gwindow.setView(view);

	/*	============================= 
		 allocate our resources here 
		=============================  */

	// create animated sprite for player
	player.create(texPlayerRight, { 0, 0, 32, 32 }, 4);
	player.speed = 2;

	int limit = currentLevel * 3;
	srand(time(0));
	for(int i = 0;i < limit;i++)
	{
		Enemy enemy;
		int randHeight = rand() % 600;
		int randWidth = rand() % 800;
		enemy.hitRate = currentLevel * 0.5;
		enemy.speed = currentLevel + 0.5;
		enemy.create(texEnemyLeft, { 100, 100, 32,32 }, 4);//For now
		enemy.setPosition(randWidth, randHeight);
		enemies.push_back(enemy);
	}
}

EndlessState::~EndlessState() {
	// here we would deallocate any resources we use in this gamestate
}

void EndlessState::logic() {
	// get mouse x and y in world coords
	sf::Vector2f mousePos = game.window.mapPixelToCoords(sf::Mouse::getPosition(game.window));

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
		}
	}

	// update player sprite
	if (player.movingLeft || player.movingUp || player.movingRight || player.movingDown) {
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
			sf::Vector2f pos = sf::Vector2f(difference.x / length, difference.y / length);
			enemy.setAnimSpeed(12);
			enemy.move(pos.x, pos.y);
			enemy.attack = -1;
		}
		else
		{
			//enemy is in attacking range
			enemy.cooldown();
			if (player.alive && !enemy.attack) {
				player.health -= 5;
				std::cout << "player is taking damage, new health: " << player.health << std::endl;
				if (player.health == 0) {
					player.alive = false;
					std::cout << "player has died" << std::endl;
				}
			}
		}
	}
}

void EndlessState::render() {
	// clear window
	gwindow.clear(sf::Color(sf::Uint32(0x40AA20FF)));

	// draw the tilemap
	gwindow.draw(tileMap);

	// draw the player
	player.animateFrame();
	gwindow.draw(player);

	//draw the enemies
	std::list<Enemy>::iterator enemyItr;
	for (enemyItr = enemies.begin(); enemyItr != enemies.end(); ++enemyItr)
	{
		Enemy& enemy = *enemyItr;
		enemy.animateFrame();
		gwindow.draw(enemy);
	}

	// update window
	gwindow.display();
}
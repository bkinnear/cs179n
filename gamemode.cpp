#include "gamemode.hpp"

#include <iostream>


void GameMode::renderEnemies(std::list<Enemy>& enemies, sf::RenderWindow& gwindow)
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
void GameMode::updateEnemies(int type, std::list<NPC>& allies, sf::Texture& texEnemyRight, sf::Texture& texEnemyLeft, std::list<Enemy>& enemies, TileMap& tileMap, Player& player) {
	// For Enemy Movement
	std::list<Enemy>::iterator enemyItr;
	for (enemyItr = enemies.begin(); enemyItr != enemies.end(); ++enemyItr)
	{
		Enemy& enemy = *enemyItr;

		// nearest target to enemy
		Character* nearestTarget = nullptr;
		// this is set to the max range of enemy attacks
		float minDist = 512.f; // TODO set this constant somewhere (or make it based on enemy idk)
		
		if (type == 1)
		{
			// find the nearest target for the enemy to attack (allies and player)
			for (NPC& ally : allies) {
				// ignore dead allies
				if (!ally.alive)
					continue;

				float dist = Utils::pointDistance(enemy.getPosition(), ally.getPosition());
				if (dist < minDist) {
					minDist = dist;
					nearestTarget = &ally;
				}
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

void GameMode::spawnWeapons(std::list<std::pair<Item::type, sf::Sprite>> &itemsOnMap, Inventory& inventory, TileMap& tileMap) {
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

void GameMode::spawnEnemies(int noOfEnemies, sf::Texture& texEnemyRight, std::list<Enemy>& enemies, TileMap& tileMap) {
	for (int i = 0;i < noOfEnemies;i++)
	{
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
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

class GameMode {

public:
	static void spawnEnemies(int, sf::Texture&, std::list<Enemy>&, TileMap&);
	static void spawnWeapons(std::list<std::pair<Item::type, sf::Sprite>>&, Inventory&, TileMap&);
	static void updateEnemies(int, std::list<NPC>&, sf::Texture&, sf::Texture&, std::list<Enemy>&, TileMap&, Player&);
	static void renderEnemies(std::list<Enemy>&, sf::RenderWindow&);
};
#endif
#ifndef ENDLESS_H_
#define ENDLESS_H_

#include "game.hpp"
#include "player.hpp"
#include "inventory.hpp"
#include "tilemap.hpp"
#include "projectile.hpp"
#include "enemy.hpp"
#include "npc.hpp"
#include "mask.hpp"
#include "classes.hpp"
#include "gamemode.hpp"

#include <list>
#include <unordered_map>

typedef std::list<std::pair<Item::type, sf::Sprite>>::iterator ItemIterator;

class EndlessState : public GameMode {
public:
	EndlessState(Game&, PlayerClass);
	~EndlessState();
};

#endif
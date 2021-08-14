#ifndef CHARACTER_H_
#define CHARACTER_H_

#include <queue>
#include <future>

#include "anim.hpp"

class TileMap;
struct Node;

/* any in-world character with health (enemy, NPCs, player, etc.)
 * is just an AnimSprite with health
 * NOTE: feel free to add anything else all characters might have
 * this is mostly used for polymorphism
*/
class Character: public AnimSprite {
public:
	Character() :
		AnimSprite()
	{}
	Character(const sf::Texture& texture, const sf::IntRect& subRect, unsigned nSubsprites):
		AnimSprite(texture, subRect, nSubsprites)
	{}

	void moveOnPath(const TileMap& tileMap);
	void moveTowards(const TileMap& tileMap, sf::Vector2f target);
	// non blocking function to start finding new path to a target
	void findPath(const TileMap& tileMap, sf::Vector2i target);
	// updates to latest path and returns if path exists
	bool isOnPath();

	// character health
	int health = 100;

	// whether character is alive
	bool alive = true;

	// check wether NPC is a dummy or shield
	bool isDummy = false;
	bool isShield = false;

	// position of center shield
	sf::Vector2f centerShield;

	// sprite movement speed in px per frame
	int speed = 3;

	// 0 => left, 1 => right
	int direction = 0;

	// timer to see if path needs to be recalced
	sf::Clock pathClock;

	Node* pathHead = nullptr; // TODO MAKE THIS PRIVATE WHEN NO LOGNER NEEDED FOR DEBUG

private:
	std::future<Node*> nextPathHead;
	bool pathRetrieved = false;

	// non-blocking function to check to see if new path is available
	void updatePath();
};

// node for use in path finding // TODO move to character.cpp
struct Node {
	Node::Node(const sf::Vector2i& pos, Node* parent, int cost, int priority) :
		pos(pos),
		parent(parent),
		cost(cost),
		priority(priority)
	{}

	sf::Vector2i pos;

	Node* parent;

	int cost;
	int priority;
	bool isPath = false;
};

#endif
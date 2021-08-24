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

	// 0 => left, 1 => right
	int direction = 0;

	// check wether NPC is a dummy or shield
	bool isDummy = false;
	bool isShield = false;

	// position of center shield
	sf::Vector2f centerShield;

	// timer to see if path needs to be recalced
	sf::Clock pathClock;

	Node* pathHead = nullptr; // TODO MAKE THIS PRIVATE WHEN NO LOGNER NEEDED FOR DEBUG

	void setSpeed(float spd) {
		speed = spd;
	}

	void setArmor(int arm) {
		armor = arm;
	}

	void setHealth(int hp) {
		health = hp;
	}

	void heal(int hp) {
		health = std::min(health + hp, 100);
	}

	void damage(int dmg) {
		health -= std::max(0, dmg - armor);
		if (health <= 0) {
			health = 0;
			alive = false;
		}
	}

	bool isAlive() const {
		return alive;
	}
	
	float getSpeed() const {
		return speed;
	}

	int getArmor() const {
		return armor;
	}

	int getHealth() const {
		return health;
	}

private:
	std::future<Node*> nextPathHead;
	bool pathRetrieved = false, isPathing = false;

	// non-blocking function to check to see if new path is available
	void updatePath();

	// character health
	int health = 100;

	// whether character is alive
	bool alive = true;

	// reduces damage by flat amount
	int armor = 0;

	// sprite movement speed multiplier
	float speed = 1.f;
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
#ifndef CHARACTER_H_
#define CHARACTER_H_

#include <queue>

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
	void findPath(const TileMap& tileMap, sf::Vector2i target);
	bool isOnPath() const { return (pathHead != nullptr); };

	// character health
	int health = 100;

	// whether character is alive
	bool alive = true;

	// sprite movement speed in px per frame
	int speed = 3;

	// 0 => left, 1 => right
	int direction = 0;

	// timer to see if path needs to be recalced
	sf::Clock pathClock;

private:
	Node* pathHead = nullptr;
};

#endif
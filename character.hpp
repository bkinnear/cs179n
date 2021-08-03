#ifndef CHARACTER_H_
#define CHARACTER_H_

#include "anim.hpp"

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
};

#endif
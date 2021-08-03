#ifndef NPC_H_
#define NPC_H_

#include "character.hpp"

class NPC : public Character {
public:
	NPC(const sf::Texture& texture) :
		Character(texture, {0, 0, 32, 32}, 4)
	{};

	// position that ally is moving towards
	sf::Vector2f moveTarget;
	
	// tick for when ally can attack again
	unsigned attackTick = 0;

	// range at which NPC can engage enemies
	float range = 256.f;

	// whether NPC is moving to target position
	bool moving = false;
};

#endif
#ifndef NPC_H_
#define NPC_H_

#include "anim.hpp"

class NPC : public AnimSprite {
public:
	NPC(const sf::Texture& texture) :
		AnimSprite(texture, {0, 0, 32, 32}, 4)
	{};

protected:

};

#endif
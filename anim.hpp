#ifndef ANIM_H_
#define ANIM_H_

#include "mask.hpp"

/* Animated Sprite object
 * sprite index can be specified with setIndex
 * Parameters: texture, first (leftmost) subsprite source rect, number of subsprites
*/
class AnimSprite : public MSprite {
public:
	AnimSprite() :
		MSprite(),
		subRect(0, 0, 0, 0),
		curRect(0, 0, 0, 0),
		nSubsprites(0),
		ticksPerFrame(-1) // default no animation
	{};

	AnimSprite(const sf::Texture& texture, const sf::IntRect& subRect, unsigned nSubsprites) :
		MSprite(texture),
		subRect(subRect),
		curRect(subRect),
		nSubsprites(nSubsprites),
		ticksPerFrame(-1) // default no animation
	{
		updateSubsprite();
	};

	/* create animated sprite from texture, first subsprite rect, num of subsprites */
	void create(const sf::Texture& texture, const sf::IntRect& subRect, unsigned nSubsprites) {
		setTexture(texture);
		this->subRect = subRect;
		this->curRect = subRect;
		this->nSubsprites = nSubsprites;

		updateSubsprite();
	}

	/* sets subsprite index to n
	 * if n >= nSubsprites, index = 0
	 */
	void setIndex(unsigned n) {
		if (n >= nSubsprites)
			index = 0;
		else
			index = n;
		updateSubsprite();
	}

	/* sets speed in terms of FPS
	 * set to -1 for no animation
	 */
	void setAnimSpeed(int spd) {
		ticksPerFrame = 60 / spd;
	}

	/* gets speed in terms of FPS
	 * -1 implies no animation
	 */
	int getAnimSpeed() const {
		return 60 / ticksPerFrame;
	}

	/* gets ticks per frame
	 */
	int getTicksPerFrame() const {
		return ticksPerFrame;
	}

	/* increments subsprite index per frame (depending on speed)
	 * loops back to 0 after drawing all subsprites
	 */
	void animateFrame() {
		if (tick++ >= ticksPerFrame) {
			tick = 0;
			index = (index + 1) % nSubsprites;
			updateSubsprite();
		}
	}

	/* returns subsprite index */
	unsigned getIndex() const {
		return index;
	}

	/*  */
	unsigned getNumSubsprites() const {
		return nSubsprites;
	}

private:
	/* updates source rect as specified by subsprite index */
	void updateSubsprite() {
		curRect.left = subRect.width * index;
		setTextureRect(curRect);
	}

	// first subsprite src rect (index = 0)
	sf::IntRect subRect;

	// current subsprite src rect (index = n) 
	sf::IntRect curRect;

	// current subsprite index
	unsigned index = 0;

	// number of subsprites in texture
	unsigned nSubsprites;

	// speed of animation (1 => 60FPS, 2 => 30FPS, n => 60/n FPS)
	int ticksPerFrame;
	unsigned tick = 0;
};


#endif
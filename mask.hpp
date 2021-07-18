#ifndef MASK_H_
#define MASK_H_

#include <SFML/Graphics.hpp>

/* A masked sprite
 * sf::Sprite, but with a separate collision mask
 * getBounds() returns the bounds of the collision box
*/
class MSprite: public sf::Sprite {
public:
	MSprite() {};
	MSprite(const sf::FloatRect& bounds) :
		maskBounds(bounds)
	{}
	MSprite(const sf::Texture& texture) :
		Sprite(texture),
		maskBounds(getLocalBounds())
	{}
	MSprite(const sf::Texture& texture, const sf::FloatRect& bounds) :
		Sprite(texture),
		maskBounds(bounds)
	{}
	
	sf::FloatRect getBounds() const{
		return { getGlobalBounds().left + maskBounds.left, getGlobalBounds().top + maskBounds.top, maskBounds.width, maskBounds.height};
	}

	void setMaskBounds(const sf::FloatRect& newBounds) {
		maskBounds = newBounds;
	}

	sf::FloatRect getMaskBounds() const {
		return maskBounds;
	};

	bool isColliding(const MSprite& other) {
		return getBounds().intersects(other.getBounds());
	}

	bool isColliding(const sf::FloatRect& rect) {
		return getBounds().intersects(rect);
	}

	bool pointColliding(const sf::Vector2f point) {
		return getBounds().contains(point);
	}

protected:
	sf::FloatRect maskBounds;
};

#endif
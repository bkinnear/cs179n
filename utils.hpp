#ifndef UTILS_H_
#define UTILS_H_

#include <SFML/Graphics.hpp>

class Utils
{
	public:
		static float pointDirection(sf::Vector2f, sf::Vector2f);
		static sf::Vector2f vectorInDirection(float, float);
};
#endif


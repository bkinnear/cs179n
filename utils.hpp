#ifndef UTILS_H_
#define UTILS_H_

#include <SFML/Graphics.hpp>

class Utils
{
	public:
		// returns direction from point A to point B
		static float pointDirection(sf::Vector2f A, sf::Vector2f B);
		// returns vector of length len point in direction dir
		static sf::Vector2f vectorInDirection(float len , float dir);
		// returns distance from point A to point B
		static float pointDistance(sf::Vector2f A, sf::Vector2f B);
		// returns point along edge of B nearest to A
		static sf::Vector2f pointEdge(const sf::Vector2f A, const sf::FloatRect& B);
		// returns tile coordinates given world coordinates
		static sf::Vector2i toTileCoords(sf::Vector2f pos);
		// returns world coordinates snapped to tile coordinates
		static sf::Vector2i snapToTile(sf::Vector2f pos);
		// returns world coordinates snapped to tile coordinates
		static sf::Vector2i snapToTile(sf::Vector2i pos);
		// returns world coordinates snapped to tile coordinates
		static sf::Vector2f snapToTilef(sf::Vector2f pos);
};
#endif


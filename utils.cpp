#include "utils.hpp"
#include "sfml/Graphics.hpp"

#define _PI 3.14159265f

float Utils::pointDirection(sf::Vector2f A, sf::Vector2f B) {
	return std::atan2f(B.y - A.y, B.x - A.x) * 180.f / _PI;
}

// returns vector of given length in given direction (deg)
sf::Vector2f Utils::vectorInDirection(float length, float direction) {
	return { length * std::cosf(direction * _PI / 180.f), length * std::sinf(direction * _PI / 180.f) };
}

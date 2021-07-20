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

float Utils::pointDistance(sf::Vector2f A, sf::Vector2f B) {
	return std::sqrt(std::pow((B.x - A.x), 2) + std::pow((B.y - A.y), 2));
}

// this is a desmos graph I cooked up to create this function (for inside sqr):
// https://www.desmos.com/calculator/7tj8hqkuif
sf::Vector2f Utils::pointEdge(const sf::Vector2f A, const sf::FloatRect& B) {
	if (A.x <= B.left) {
		// left of rect
		if (A.y < B.top) {
			// above rect UL
			return { B.left, B.top };
		}
		else if (A.y < B.top + B.height) {
			// middle left of rect ML
			return { B.left, A.y };
		}
		else {
			// below rect LL
			return { B.left, B.top + B.height };
		}
	}
	else if (A.x < B.left + B.width) {
		// middle x axis of rect
		if (A.y < B.top) {
			// above rect UM
			return { A.x, B.top };
		}
		else if (A.y < B.top + B.height) {
			// inside rect
			sf::Vector2f midpoint = { B.left + B.width / 2, B.top + B.height / 2 };
			// |w/h * (y - m.y)| < (x - m.x) => right preference
			// |w/h * (y - m.y)| < -(x - m.x) => left preference
			// else => top or bottom preference
			if (std::abs(B.width / B.height * (A.y - midpoint.y)) < (A.x - midpoint.x)) {
				// right side preference
				return { B.left + B.width, A.y };
			}
			else if (std::abs(B.width / B.height * (A.y - midpoint.y)) < -(A.x - midpoint.x)) {
				// left side preference
				return { B.left, A.y };
			}
			else {
				if (A.y < midpoint.y) {
					// top side preference
					return { A.x, B.top };
				}
				else {
					// bottom side preference
					return { A.x, B.top + B.height };
				}
			}
		}
		else {
			// below rect LM
			return { A.x, B.top + B.height };
		}
	}
	else {
		// right of rect
		if (A.y < B.top) {
			// above rect UR
			return { B.left + B.width, B.top };
		}
		else if (A.y < B.top + B.height) {
			// middle right of rect MR
			return { B.left + B.width, A.y };
		}
		else {
			 // below rect LR
			return { B.left + B.width, B.top + B.height };
		}
	}
}

#include "state.hpp"

#define gwindow game.window

void State::updateEffects() {
	for (auto eItr = effects.begin(); eItr != effects.end(); eItr++) {
		Effect& e = *(eItr->first);
		unsigned& ticks = eItr->second.first;
		sf::Vector2f& pos = eItr->second.second;

		// check if effect has reached end of animation
		if (ticks++ >= e.getNumSubsprites() * e.getTicksPerFrame() - 1) {
			eItr = effects.erase(eItr);
			if (eItr == effects.end())
				break;
		}
	}
}

void State::drawEffects() {
	for (auto eItr = effects.begin(); eItr != effects.end(); eItr++) {
		Effect& e = *(eItr->first);
		unsigned& ticks = eItr->second.first;
		sf::Vector2f& pos = eItr->second.second;

		e.setIndex(ticks / e.getTicksPerFrame());
		e.setPosition(pos);

		gwindow.draw(e);
	}
}

sf::Texture& State::createTexture(const std::string& fname) {
	// grow textures by 1
	textures.push_back(sf::Texture());

	// load texture from image
	textures.back().loadFromFile(fname);

	return textures.back();
}

sf::Texture& State::createTexture(const std::string& fname, sf::IntRect src) {
	// grow textures by 1
	textures.push_back(sf::Texture());

	// load texture from image and source rect
	textures.back().loadFromFile(fname, src);

	return textures.back();
}

Effect* State::loadEffect(const sf::Texture& texture, const sf::IntRect& subRect, unsigned nSubsprites, unsigned animationSpeed) {
	effectSprites.emplace_back(texture, subRect, nSubsprites);
	Effect& effect = effectSprites.back();

	effect.setAnimSpeed(animationSpeed);
	
	return &effect;
}

void State::createEffect(Effect* effect, const sf::Vector2f& pos) {
	effects[effect] = { 0, pos };
}
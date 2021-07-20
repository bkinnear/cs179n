#include "state.hpp"

#define gwindow game.window

void State::updateEffects() {
	for (auto mapItr = effects.begin(); mapItr != effects.end(); mapItr++) {
		// effect sprite
		Effect& effectSprite = *(mapItr->first);
		// list of all effects of type "effectSprite"
		auto& effectList = mapItr->second;
		for (auto eItr = effectList.begin(); eItr != effectList.end(); eItr++) {
			// ticks this effect instance has ticked
			unsigned& ticks = eItr->first;
			// position of this effect instance
			sf::Vector2f& pos = eItr->second;

			// check if effect instance has reached end of animation
			if (ticks++ >= effectSprite.getNumSubsprites() * effectSprite.getTicksPerFrame() - 1) {
				eItr = effectList.erase(eItr);
				if (eItr == effectList.end())
					break;
			}
		}
	}
}

void State::drawEffects() {
	for (auto mapItr = effects.begin(); mapItr != effects.end(); mapItr++) {
		// effect sprite
		Effect& effectSprite = *(mapItr->first);
		// list of all effects of type "effectSprite"
		auto& effectList = mapItr->second;
		for (auto eItr = effectList.begin(); eItr != effectList.end(); eItr++) {
			// ticks this effect instance has ticked
			unsigned& ticks = eItr->first;
			// position of this effect instance
			sf::Vector2f& pos = eItr->second;

			// draw effect using effectSprite
			effectSprite.setIndex(ticks / effectSprite.getTicksPerFrame());
			effectSprite.setPosition(pos);

			gwindow.draw(effectSprite);
		}
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

	// add bucket to effect map
	effects[&effect] = std::list<std::pair<unsigned, sf::Vector2f>>();
	
	return &effect;
}

void State::createEffect(Effect* effect, const sf::Vector2f& pos) {
	effects.at(effect).push_back({ 0, pos });
}
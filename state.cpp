#include "state.hpp"

#include <iostream>

#define gwindow game.window

void State::updateEffects() {
	// loop through all effect buckets
	for (auto& eDataPair: effects) {
		EffectData& eData = eDataPair.second;

		// loop through all effects in effectlist for this effectID
		auto eItr = eData.effectList.begin();
		while (eItr != eData.effectList.end()) {
			// get effect
			Effect& effect = *eItr;

			effect.tick++;

			//std::cout << "tick: " << effect.tick << ", index: " << eData.effectSprite.getIndex() << std::endl;

			// check if effect instance has reached end of animation
			if (effectClock.getElapsedTime().asSeconds() > effect.startTime + eData.lifetime) {
				eItr = eData.effectList.erase(eItr);
				continue;
			}

			eItr++;
		}
	}
}

void State::drawEffects(sf::RenderStates states) {
	// loop through all effect buckets
	for (auto eDataPair : effects) {
		EffectData& eData = eDataPair.second;

		// loop through all effects in effectList for this effectID
		auto eItr = eData.effectList.begin();
		while (eItr != eData.effectList.end()) {
			// get effect
			Effect& effect = *eItr;

			// draw effect using effectSprite
			eData.effectSprite.setIndex(effect.tick / eData.effectSprite.getTicksPerFrame());
			eData.effectSprite.setPosition(effect.pos);

			gwindow.draw(eData.effectSprite, states);

			eItr++;
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

EffectID State::loadEffect(const sf::Texture& texture, const sf::IntRect& subRect, unsigned nSubsprites, unsigned animationSpeed, float animationTime) {
	// add effect sprite
	EffectID effectID = nextEffectID++;

	// animation time is default, set animation time to be 1 full loop
	if (animationTime == 0.f) {
		animationTime = nSubsprites / 60.f;
	}

	// add new effect to map
	effects[effectID] = { AnimSprite(), animationTime, std::list<Effect>() };
	effects[effectID].effectSprite.create(texture, subRect, nSubsprites);
	effects[effectID].effectSprite.setAnimSpeed(animationSpeed);
	
	return effectID;
}

AnimSprite& State::getEffectSprite(EffectID effectID) {
	return effects[effectID].effectSprite;
}

void State::createEffect(EffectID effectID, const sf::Vector2f& pos) {
	// add new effect data to effects
	effects.at(effectID).effectList.push_back({ effectClock.getElapsedTime().asSeconds(), 0, pos });
}
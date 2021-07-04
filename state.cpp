#include "state.hpp"

sf::Texture& State::createTexture(const std::string& fname) {
	// grow textures by 1
	textures.resize(textures.size() + 1);

	// get new texture
	sf::Texture& tex = textures.back();

	// load texture from image
	tex.loadFromFile(fname);

	return tex;
}

sf::Texture& State::createTexture(const std::string& fname, sf::IntRect src) {
	// grow textures by 1
	textures.push_back(sf::Texture());

	// load texture from image and source rect
	textures.back().loadFromFile(fname, src);

	return textures.back();
}
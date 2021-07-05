#ifndef TILEMAP_H_
#define TILEMAP_H_

#include "game.hpp"
#include <vector>
#include <string>

#define TILE_SIZE 32

struct Tile {
	/* 0 for empty tile */
	unsigned char type;
	bool opaque;
};

class TileMap: public sf::Drawable {
public:
	TileMap(sf::Texture& tileset, unsigned mapWidth, unsigned mapHeight);

	void setTile(unsigned x, unsigned y, unsigned char type);

	bool isOpaque(unsigned x, unsigned y);

	static sf::Vector2i getTileTexOffset(unsigned tileType);

	const int mapWidth;
	const int mapHeight;

protected:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	sf::Texture& texTileset;

	std::vector<std::vector<Tile>> map;
};

#endif
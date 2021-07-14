#ifndef TILEMAP_H_
#define TILEMAP_H_

#include "game.hpp"
#include <vector>
#include <string>

#define TILE_SIZE 32

typedef unsigned char tileType;

struct Tile {
	/* 0 for empty tile */
	tileType type;
	bool opaque;
};

/* holds tiles
 * TODO: make multilevelled tilemap (just add second layer)
*/
class TileMap: public sf::Drawable {
public:
	TileMap(sf::Texture& tileset, unsigned mapWidth, unsigned mapHeight);

	// sets tile at (x, y) to type specified
	void setTile(unsigned x, unsigned y, tileType type);

	// returns whether tile at world coords (x, y) is opaque
	bool isOpaque(unsigned x, unsigned y) const;

	//returns true if the area needed to place an obstacle is clear
	bool tileClear(unsigned h, unsigned w, unsigned y_off, unsigned x_off);

	// returns true if moving sprite's position by <dx, dy> is clear of opaque tiles
	bool areaClear(const MSprite& spr, float dx, float dy) const;

	// returns true if moving sprite's position by <dpos.x, dpos.y> is clear of opaque tiles
	bool areaClear(const MSprite& spr, const sf::Vector2f& dpos) const;

	const int mapWidth;
	const int mapHeight;

protected:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	sf::Texture& texTileset;

	std::vector<std::vector<Tile>> map;

	// returns texture rect offset of given tile in the tilesheet
	static sf::Vector2i getTileTexOffset(tileType type);
};

#endif
#ifndef TILEMAP_H_
#define TILEMAP_H_

#include "game.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

class GameMode;

#define TILE_SIZE 32

#define TILE_DOOR_CLOSED 42
#define TILE_DOOR_OPEN 43

typedef unsigned char Tile;

/* holds tiles
 * TODO: make multilevelled tilemap (just add second layer)
*/
class TileMap: public sf::Drawable {
public:
	TileMap() : TileMap(0, 0) {};
	TileMap(unsigned mapWidth, unsigned mapHeight);

	std::vector<std::vector<Tile>> getTileMap();
	void setTileMap(std::vector<std::vector<Tile>>&);

	// loads textures
	void loadTextures(GameMode* gmode);

	// loads map from CSV file
	void loadMap(GameMode* gmode, const std::string& fname);

	// saves map from to file
	void saveMap(int);

	// generates tile map
	void generate(GameMode* gmode);

	// sets tile at (x, y) to type specified
	void setTile(unsigned x, unsigned y, Tile tile);

	// returns whether tile at world coords (x, y) is opaque (cant be passed through)
	bool isOpaqueAt(float x, float y) const;

	// returns whether tile at world coords (x, y) is open (can be shot through)
	bool isOpenAt(float x, float y) const;

	// returns whether a tile at world coords (x,y) is a door
	bool isDoor(unsigned x, unsigned y) const;

	// returns whether a tile at world coords (x,y) is a door and whether its open
	bool isDoorOpen(unsigned x, unsigned y) const;

	// returns the type of tile at world coords (x,y)
	Tile getTile(unsigned x, unsigned y) const;

	//returns true if the area needed to place an obstacle is clear
	bool tileClear(unsigned h, unsigned w, unsigned y_off, unsigned x_off);

	// returns true if moving sprite's position by <dx, dy> is clear of opaque tiles
	bool areaClear(const MSprite& spr, float dx=0.f, float dy=0.f) const;

	// returns true if moving sprite's position by <dpos.x, dpos.y> is clear of opaque tiles
	bool areaClear(const MSprite& spr, const sf::Vector2f& dpos) const;

	sf::FloatRect getTileBounds(float x, float y) const;

	// returns tilemap width
	unsigned getWidth() const {
		return width;
	}

	// returns tilemap height
	unsigned getHeight() const {
		return height;
	}

protected:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	std::vector<sf::Texture*> textures;

	std::vector<std::vector<Tile>> map;

	// returns texture rect offset of given tile in the tilesheet
	static sf::Vector2i getTileTexOffset(Tile tile);

	unsigned width, height;
};

#endif
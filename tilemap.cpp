#include "tilemap.hpp"

/* Hard coded building presets */
struct building1 {
	static const int w = 7, h = 7;
	static const unsigned char array[w][h];
};

const unsigned char building1::array[w][h] =
{
{1, 2, 2, 2, 2, 2, 3},
{8, 9, 9, 9, 9, 9, 10},
{8, 9, 9, 9, 9, 9, 10},
{8, 9, 9, 9, 9, 9, 10},
{8, 9, 9, 9, 9, 9, 10},
{8, 9, 9, 9, 9, 9, 10},
{13, 14, 14, 30, 14, 14, 15}
};

TileMap::TileMap(sf::Texture& tileset, unsigned mapWidth, unsigned mapHeight) :
	texTileset(tileset),
	mapWidth(mapWidth),
	mapHeight(mapHeight)
{
	// initialize map
	map.resize(mapHeight);
	for (unsigned y = 0; y < mapHeight; y++) {
		map[y].resize(mapWidth);
		for (unsigned x = 0; x < mapWidth; x++) {
			// goes over every tile
			// set all tiles to empty (0)
			setTile(x, y, 0);
		}
	}

	// place example building
	sf::Vector2i offset = { 4, 4 };
	for (unsigned y = 0; y < building1::h; y++) {
		for (unsigned x = 0; x < building1::w; x++) {
			setTile(x+offset.x, y+offset.y, building1::array[y][x]);
		}
	}
};

void TileMap::setTile(unsigned x, unsigned y, unsigned char type) {
	map[y][x].type = type;

	switch (type) {
	case 0:
	case 9:
		map[y][x].opaque = false;
		break;
	default:
		map[y][x].opaque = true;
	}
}

bool TileMap::isOpaque(unsigned x, unsigned y) {
	if (x >= 0 && x < mapWidth && y >=0 && y < mapHeight)
		return map[y][x].opaque;

	return true;
}

sf::Vector2i TileMap::getTileTexOffset(unsigned tileType) {
	int i = tileType;
	if (i <= 7)
		return { (i - 1) * 32, 576 };
	if (i <= 11)
		return { (i - 8) * 32, 608 };
	if (i == 12)
		return { 160, 608 };
	if (i <= 19)
		return { (i - 13) * 32, 640 };
	if (i <= 27)
		return { (i - 20) * 32, 672 };
	if (i <= 33)
		return { (i - 28) * 32, 704 };
	if (i <= 41)
		return { (i - 34) * 32, 736 };
	if (i <= 59)
		return { (i - 42) * 32, 768 };
	if (i <= 67)
		return { (i - 60) * 32, 800 };
	if (i <= 72)
		return { (i - 68) * 32, 832 };

	// default set to red rug
	return { 224, 832 };
}

// TODO optimize tile drawing so we only need to draw tiles inside screen bounds
void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::Sprite sprTile;
	sprTile.setTexture(texTileset);

	for (unsigned x = 0; x < mapWidth; x++) {
		for (unsigned y = 0; y < mapHeight; y++) {
			const Tile& tile = map[y][x];
			if (tile.type == 0)
				continue;
			sprTile.setTextureRect(sf::IntRect(getTileTexOffset(tile.type), { TILE_SIZE, TILE_SIZE }));
			sprTile.setPosition(x*TILE_SIZE, y*TILE_SIZE);
			target.draw(sprTile, states);
		}
	}
}
#include "tilemap.hpp"
#include <time.h>

/* Hard coded building presets */
struct building1 {
	static const int w = 7, h = 7;
	static const tileType array[w][h];
};

struct building2 {
	static const int w = 5, h = 5;
	static const tileType array[w][h];
};

struct building3 {
	static const int w = 9, h = 9;
	static const tileType array[w][h];
};

struct building4 {
	static const int w = 10, h = 10;
	static const tileType array[w][h];
};

struct building5 {
	static const int w = 10, h = 10;
	static const tileType array[w][h];
};


const tileType building1::array[w][h] =
{
{1, 2, 2, 2, 2, 2, 3},
{8, 9, 9, 9, 9, 9, 10},
{0, 9, 9, 9, 9, 9, 10},
{0, 9, 9, 9, 9, 9, 10},
{8, 9, 9, 9, 9, 9, 10},
{8, 9, 9, 9, 9, 9, 10},
{13, 14, 14, 30, 14, 14, 15}
};

const tileType building2::array[w][h] =
{
{1, 2, 37, 2, 3},
{8, 9, 9, 9, 10},
{0, 9, 9, 9, 10},
{0, 9, 9, 9, 10},
{13, 14, 14, 14, 15}
};

const tileType building3::array[w][h] =
{
{1, 2, 2, 2, 26, 2, 2, 2, 3},
{8, 9, 9, 61, 61, 61, 9, 9, 10},
{8, 9, 9, 9, 9, 9, 9, 9, 10},
{0, 9, 9, 9, 9, 9, 9, 9, 10},
{0, 9, 9, 9, 9, 9, 9, 9, 10},
{8, 9, 9, 9, 9, 9, 9, 9, 10},
{8, 9, 9, 9, 9, 9, 9, 9, 10},
{8, 9, 9, 9, 9, 9, 9, 9, 10},
{13, 14, 14, 14, 14, 14, 14, 14, 15}
};

const tileType building4::array[w][h] =
{
{1, 2, 2, 2, 26, 2, 2, 2, 3},
{8, 9, 9, 61, 61, 61, 9, 9, 10},
{8, 9, 9, 9, 9, 9, 9, 9, 10},
{0, 9, 9, 9, 9, 9, 9, 9, 10},
{0, 9, 9, 9, 9, 9, 9, 9, 10},
{8, 9, 9, 9, 9, 9, 9, 9, 10},
{8, 9, 9, 9, 9, 1, 2, 2, 15},
{8, 9, 9, 9, 9, 10, 0, 0, 0},
{8, 9, 9, 9, 9, 10, 0, 0, 0},
{13, 14, 14, 14, 14, 15, 0, 0, 0}
};

const tileType building5::array[w][h] =
{
{1, 2, 2, 2, 2, 2, 2, 2, 3},
{8, 9, 9, 9, 37, 38, 9, 9, 10},
{8, 9, 9, 9, 9, 9, 9, 9, 10},
{8, 9, 9, 9, 9, 9, 9, 9, 10},
{8, 9, 9, 9, 9, 9, 9, 9, 10},
{8, 9, 9, 9, 9, 9, 9, 9, 10},
{13, 14, 14, 14, 9, 9, 1, 2, 15},
{0, 0, 0, 0, 0, 0, 10, 0, 0},
{0, 0, 0, 0, 0, 0, 10, 0, 0},
{22, 23, 23, 23, 23, 24, 15, 0, 0}
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
	srand(time(NULL));
	int bldg_num = rand() % 5 + 1;		//randomly selects one of the building presets
	int x_offset = rand() % 10 + 2;		//generates a random number to determine the x offset
	int y_offset = rand() % 8 + 2;		//generates a random number to determine the y offset

	sf::Vector2i offset = { x_offset, y_offset };

	switch (bldg_num) {
	case 1:
		for (unsigned y = 0; y < building1::h; y++) {
			for (unsigned x = 0; x < building1::w; x++) {
				setTile(x + offset.x, y + offset.y, building1::array[y][x]);
			}
		}
		break;
	case 2:
		for (unsigned y = 0; y < building2::h; y++) {
			for (unsigned x = 0; x < building2::w; x++) {
				setTile(x + offset.x, y + offset.y, building2::array[y][x]);
			}
		}
		break;
	case 3:
		for (unsigned y = 0; y < building3::h; y++) {
			for (unsigned x = 0; x < building3::w; x++) {
				setTile(x + offset.x, y + offset.y, building3::array[y][x]);
			}
		}break;
	case 4:
		for (unsigned y = 0; y < building4::h; y++) {
			for (unsigned x = 0; x < building4::w; x++) {
				setTile(x + offset.x, y + offset.y, building4::array[y][x]);
			}
		}break;
	case 5:
		for (unsigned y = 0; y < building5::h; y++) {
			for (unsigned x = 0; x < building5::w; x++) {
				setTile(x + offset.x, y + offset.y, building5::array[y][x]);
			}
		}break;
	default:
		for (unsigned y = 0; y < building1::h; y++) {
			for (unsigned x = 0; x < building1::w; x++) {
				setTile(x + offset.x, y + offset.y, building1::array[y][x]);
			}
		}

	}
};

void TileMap::setTile(unsigned x, unsigned y, tileType type) {
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
	// convert from world to map coordinates
	x = int(x / TILE_SIZE);
	y = int(y / TILE_SIZE);

	if (x >= 0 && x < mapWidth && y >=0 && y < mapHeight)
		return map[y][x].opaque;

	return true;
}

sf::Vector2i TileMap::getTileTexOffset(tileType type) {
	int i = type;
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

bool TileMap::areaClear(const sf::Sprite& spr, float dx, float dy) {
	// get spr's world bounds
	sf::FloatRect bounds = spr.getGlobalBounds();

	// shift bounds by deltas
	bounds.left += dx;
	bounds.top += dy;

	bool lt = isOpaque(bounds.left, bounds.top);
	bool rt = isOpaque(bounds.left + bounds.width, bounds.top);
	bool ll = isOpaque(bounds.left, bounds.top + bounds.height);
	bool lr = isOpaque(bounds.left + bounds.width, bounds.top + bounds.height);

	if (lt || rt || ll || lr)
		return false;

	return true;
}

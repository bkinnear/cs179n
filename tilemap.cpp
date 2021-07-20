#include "tilemap.hpp"
#include <time.h>
#include <vector>

#include <iostream>

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
	static const int w = 6, h = 6;
	static const tileType array[w][h];
};

struct building4 {
	static const int w = 6, h = 6;
	static const tileType array[w][h];
};

struct building5 {
	static const int w = 6, h = 6;
	static const tileType array[w][h];
};

struct buildingsmall {
	static const int w = 4, h = 4;
	static const tileType array[w][h];
};

const tileType building1::array[w][h] =
{
{1, 14, 14, 30, 14, 26, 3},
{8, 9, 9, 9, 9, 9, 10},
{8, 9, 9, 9, 9, 9, 10},
{8, 9, 9, 41, 9, 9, 10},
{8, 9, 9, 9, 9, 9, 10},
{8, 9, 9, 9, 9, 9, 10},
{13, 14, 14, 30, 14, 14, 15}
};

const tileType building2::array[w][h] =
{
{1, 2, 37, 37, 3},
{8, 9, 9, 9, 10},
{8, 9, 9, 9, 10},
{8, 9, 9, 9, 10},
{13, 30, 14, 14, 15}
};

const tileType building3::array[w][h] =
{
{1, 30, 2, 2, 26,  3},
{8, 9, 61, 61, 9, 10},
{8, 9, 9, 9, 9, 10},
{8, 9, 9, 9, 9, 10},
{8, 9, 9, 9, 9, 10},
{13, 14, 30, 14, 14, 15}
};

const tileType building4::array[w][h] =
{
{1, 26, 2, 2, 30, 3},
{8, 61, 61, 61, 9, 10},
{8, 9, 9, 9, 9, 10},
{8, 9, 9, 9, 9, 10},
{8, 9, 9, 9, 9, 10},
{13, 14, 30, 14, 14, 15}
};

const tileType building5::array[w][h] =
{
{1, 30, 2, 2, 2, 3},
{8, 9, 9, 37, 38, 10},
{8, 9, 9, 9, 9, 10},
{8, 9, 9, 9, 9, 10},
{8, 9, 9, 9, 9, 10},
{13, 14, 14, 30, 14, 15}
};

const tileType buildingsmall::array[w][h] =
{
{1, 2, 2, 3},
{8, 9, 9, 10},
{8, 9, 9, 10},
{13, 30, 14, 15},
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
	int num_bldg = 50;
	
	for (int i = 1; i <= num_bldg; i++) {
		int bldg_num = rand() % 6 + 1;		//randomly selects one of the building presets
		int x_offset = rand() % 9 + 2;		//generates a random number to determine the x offset
		int y_offset = rand() % 9 + 2;		//generates a random number to determine the y offset
		bool empty;

		sf::Vector2i offset = {y_offset,x_offset};

		switch (bldg_num) {
		case 1:
			if (tileClear(building1::h, building1::w, offset.y, offset.x)) {
				for (unsigned y = 0; y < building1::h; y++) {
					for (unsigned x = 0; x < building1::w; x++) {
						setTile(x + offset.x, y + offset.y, building1::array[y][x]);
					}
				}
			}
			else {
				break;
			}
		case 2:
			if (tileClear(building2::h, building2::w, offset.y, offset.x)) {
				for (unsigned y = 0; y < building2::h; y++) {
					for (unsigned x = 0; x < building2::w; x++) {
						setTile(x + offset.x, y + offset.y, building2::array[y][x]);
					}
				}
			}
			else {
				break;
			}
		case 3:
			if (tileClear(building3::h, building3::w, offset.y, offset.x)) {
				for (unsigned y = 0; y < building3::h; y++) {
					for (unsigned x = 0; x < building3::w; x++) {
						setTile(x + offset.x, y + offset.y, building3::array[y][x]);
					}
				}
			}
			else {
				break;
			}
		case 4:
			if (tileClear(building4::h, building4::w, offset.y, offset.x)) {
				for (unsigned y = 0; y < building4::h; y++) {
					for (unsigned x = 0; x < building4::w; x++) {
						setTile(x + offset.x, y + offset.y, building4::array[y][x]);
					}
				}
			}
			else {
				break;
			}
		case 5:
			if (tileClear(building5::h, building5::w, offset.y, offset.x)) {
				for (unsigned y = 0; y < building5::h; y++) {
					for (unsigned x = 0; x < building5::w; x++) {
						setTile(x + offset.x, y + offset.y, building5::array[y][x]);
					}
				}
			}
			else {
				break;
			}
		case 6:
			if (tileClear(buildingsmall::h, buildingsmall::w, offset.y, offset.x)) {
				for (unsigned y = 0; y < buildingsmall::h; y++) {
					for (unsigned x = 0; x < buildingsmall::w; x++) {
						setTile(x + offset.x, y + offset.y, buildingsmall::array[y][x]);
					}
				}
			}
			else {
				break;
			}
		}
	}
};

void TileMap::setTile(unsigned x, unsigned y, tileType type) {
	map[y][x].type = type;

	switch (type) {
	case 0:
	case 9:
	case 31:
	case 33:
		map[y][x].opaque = false;
		break;
	default:
		map[y][x].opaque = true;
	}
}

bool TileMap::isOpaque(float x, float y) const {
	// convert from world to map coordinates
	unsigned tileX = int(x / TILE_SIZE);
	unsigned tileY = int(y / TILE_SIZE);

	if (tileX >= 0 && tileX < mapWidth && tileY >=0 && tileY < mapHeight)
		return map[tileY][tileX].opaque;

	return true;
}
tileType TileMap::getTileType(unsigned x, unsigned y) const {
	x = int(x / TILE_SIZE);
	y = int(y / TILE_SIZE);

	if (x >= 0 && x < mapWidth && y >= 0 && y < mapHeight)
	{
		return map[y][x].type;
	}
	return -1;
}
bool TileMap::isDoor(unsigned x, unsigned y) const {
	x = int(x / TILE_SIZE);
	y = int(y / TILE_SIZE);

	if (x >= 0 && x < mapWidth && y >= 0 && y < mapHeight)
	{
		switch (map[y][x].type)
		{
			case 30:
			case 31:
			case 32:
			case 33:
				return true;
				break;
			default:
				return false;
				break;
		}
	}
	return false;
}
bool TileMap::isDoorOpen(unsigned x, unsigned y) const {
	x = int(x / TILE_SIZE);
	y = int(y / TILE_SIZE);

	if (x >= 0 && x < mapWidth && y >= 0 && y < mapHeight)
	{
		switch (map[y][x].type)
		{
		case 31:
		case 33:
			return true;
			break;
		default:
			return false;
			break;
		}
	}
	return false;
}
bool TileMap::tileClear(unsigned h, unsigned w, unsigned y_off, unsigned x_off)
{
	for (unsigned y = 0; y < h; y++) {
		for (unsigned x = 0; x < w; x++) {
			if (map[y + y_off][x + x_off].type == 0) {
				if (y == 0 && x == 0) {
					if (map[y + y_off - 2][x + x_off - 2].type != 0 ||
						map[y + y_off - 2][x + x_off - 1].type != 0 ||
						map[y + y_off - 1][x + x_off - 2].type != 0 ||
						map[y + y_off - 1][x + x_off - 1].type != 0) {
						return false;
					}
				}
				if (y == h - 1 && x == w - 1) {
					if (map[y + y_off + 2][x + x_off + 2].type != 0 ||
						map[y + y_off + 2][x + x_off + 1].type != 0 ||
						map[y + y_off + 1][x + x_off + 2].type != 0 ||
						map[y + y_off + 1][x + x_off + 1].type != 0) {
						return false;
					}
				}
				if (y == 0 && x == w - 1) {
					if (map[y + y_off - 2][x + x_off + 2].type != 0 ||
						map[y + y_off - 2][x + x_off + 1].type != 0 ||
						map[y + y_off - 1][x + x_off + 2].type != 0 ||
						map[y + y_off - 1][x + x_off + 1].type != 0) {
						return false;
					}
				}
				if (y == h - 1 && x == 0) {
					if (map[y + y_off + 2][x + x_off - 2].type != 0 ||
						map[y + y_off + 2][x + x_off - 1].type != 0 ||
						map[y + y_off + 1][x + x_off - 2].type != 0 ||
						map[y + y_off + 1][x + x_off - 1].type != 0) {
						return false;
					}
				}
				if (y == 0) {
					if (map[y + y_off - 2][x + x_off].type != 0 ||
						map[y + y_off - 1][x + x_off].type != 0) {
						return false;
					}
				}
				if (x == 0) {
					if (map[y + y_off][x + x_off - 2].type != 0 ||
						map[y + y_off][x + x_off - 1].type != 0) {
						return false;
					}
				}
				if (y == h - 1) {
					if (map[y + y_off + 2][x + x_off].type != 0 ||
						map[y + y_off + 1][x + x_off].type != 0) {
						return false;
					}
				}
				if (x == w - 1) {
					if (map[y + y_off][x + x_off + 2].type != 0 ||
						map[y + y_off][x + x_off + 1].type != 0) {
						return false;
					}
				}
			}
			else {
				return false;
			}
		}
	}
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

bool TileMap::areaClear(const MSprite& spr, float dx, float dy) const {
	// get mspr's global bounds
	sf::FloatRect bounds = spr.getBounds();

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

bool TileMap::areaClear(const MSprite& spr, const sf::Vector2f& dpos) const {
	return areaClear(spr, dpos.x, dpos.y);
}

sf::FloatRect TileMap::getTileBounds(float x, float y) const {
	// convert world coords to map coords
	int tileX = int(x / TILE_SIZE);
	int tileY = int(y / TILE_SIZE);

	return { tileX * (float)TILE_SIZE, tileY * (float)TILE_SIZE, (float)TILE_SIZE, (float)TILE_SIZE};
}

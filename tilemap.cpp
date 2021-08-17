#include "tilemap.hpp"
#include "state.hpp"
#include "gamemode.hpp"

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
{8, 9, 51, 51, 9, 10},
{8, 9, 9, 9, 9, 10},
{8, 9, 9, 9, 9, 10},
{8, 9, 9, 9, 9, 10},
{13, 14, 30, 14, 14, 15}
};

const tileType building4::array[w][h] =
{
{1, 26, 2, 2, 30, 3},
{8, 51, 51, 51, 9, 10},
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

void setLootSpawn(GameMode& gmode, int x, int y, tileType tile) {
	// add item spawnpoint for certain tiles that should spawn loot
	switch (tile) {
	case 37:
	case 38:
	case 40:
	case 41:
	case 49:
	case 50:
	case 54:
	case 55:
		gmode.addLootSpawn({ x * 32.f, y * 32.f });
		break;
	}
}

TileMap::TileMap(unsigned mapWidth, unsigned mapHeight):
	mapWidth(mapWidth),
	mapHeight(mapHeight)
{}

void TileMap::generate(GameMode* gmode) {
	// change state to a reference
	GameMode& state = *gmode;

	// load tile textures
	int nTextures = 74; // TODO make this not a magic number
	textures.resize(nTextures);
	for (int i = 1; i < nTextures; i++) {
		// create new texture for tile of type i
		textures[i] = &state.createTexture(
			"res/big_32x32_tileset.png", // TODO make this not a magic string (maybe doesnt matter)
			sf::IntRect(getTileTexOffset(i), { TILE_SIZE,TILE_SIZE }
		));
	}

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
		int x_offset = rand() % (mapWidth-12) + 2;		//generates a random number to determine the x offset
		int y_offset = rand() % (mapHeight-12) + 2;		//generates a random number to determine the y offset
		bool empty;

		sf::Vector2i offset = {y_offset,x_offset};

		switch (bldg_num) {
		case 1:
			if (tileClear(building1::h, building1::w, offset.y, offset.x)) {
				for (unsigned y = 0; y < building1::h; y++) {
					for (unsigned x = 0; x < building1::w; x++) {
						setTile(x + offset.x, y + offset.y, building1::array[y][x]);
						setLootSpawn(state, x + offset.x, y + offset.y, building1::array[y][x]);
					}
				}
				state.addHiddenArea({ (offset.x + 1) * 32.f, (offset.y + 1) * 32.f - 20.f, (building1::w - 2) * 32.f, (building1::h - 2) * 32.f + 20.f });
			}
			else {
				break;
			}
		case 2:
			if (tileClear(building2::h, building2::w, offset.y, offset.x)) {
				for (unsigned y = 0; y < building2::h; y++) {
					for (unsigned x = 0; x < building2::w; x++) {
						setTile(x + offset.x, y + offset.y, building2::array[y][x]);
						setLootSpawn(state, x + offset.x, y + offset.y, building2::array[y][x]);
					}
				}
				state.addHiddenArea({ (offset.x + 1) * 32.f, (offset.y + 1) * 32.f - 20.f, (building2::w - 2) * 32.f, (building2::h - 2) * 32.f + 20.f });
			}
			else {
				break;
			}
		case 3:
			if (tileClear(building3::h, building3::w, offset.y, offset.x)) {
				for (unsigned y = 0; y < building3::h; y++) {
					for (unsigned x = 0; x < building3::w; x++) {
						setTile(x + offset.x, y + offset.y, building3::array[y][x]);
						setLootSpawn(state, x + offset.x, y + offset.y, building3::array[y][x]);
					}
				}
				state.addHiddenArea({ (offset.x + 1) * 32.f, (offset.y + 1) * 32.f - 20.f, (building3::w - 2) * 32.f, (building3::h - 2) * 32.f + 20.f });
			}
			else {
				break;
			}
		case 4:
			if (tileClear(building4::h, building4::w, offset.y, offset.x)) {
				for (unsigned y = 0; y < building4::h; y++) {
					for (unsigned x = 0; x < building4::w; x++) {
						setTile(x + offset.x, y + offset.y, building4::array[y][x]);
						setLootSpawn(state, x + offset.x, y + offset.y, building4::array[y][x]);
					}
				}
				state.addHiddenArea({ (offset.x + 1) * 32.f, (offset.y + 1) * 32.f - 20.f, (building4::w - 2) * 32.f, (building4::h - 2) * 32.f + 20.f });
			}
			else {
				break;
			}
		case 5:
			if (tileClear(building5::h, building5::w, offset.y, offset.x)) {
				for (unsigned y = 0; y < building5::h; y++) {
					for (unsigned x = 0; x < building5::w; x++) {
						setTile(x + offset.x, y + offset.y, building5::array[y][x]);
						setLootSpawn(state, x + offset.x, y + offset.y, building5::array[y][x]);
					}
				}
				state.addHiddenArea({ (offset.x + 1) * 32.f, (offset.y + 1) * 32.f - 20.f, (building5::w - 2) * 32.f, (building5::h - 2) * 32.f + 20.f });
			}
			else {
				break;
			}
		case 6:
			if (tileClear(buildingsmall::h, buildingsmall::w, offset.y, offset.x)) {
				for (unsigned y = 0; y < buildingsmall::h; y++) {
					for (unsigned x = 0; x < buildingsmall::w; x++) {
						setTile(x + offset.x, y + offset.y, buildingsmall::array[y][x]);
						setLootSpawn(state, x + offset.x, y + offset.y, buildingsmall::array[y][x]);
					}
				}
				state.addHiddenArea({ (offset.x + 1) * 32.f, (offset.y + 1) * 32.f - 20.f, (buildingsmall::w - 2) * 32.f, (buildingsmall::h - 2) * 32.f + 20.f });
			}
			else {
				break;
			}
		}
	}

	// find and place crate spawn points
	int numCrates = (mapWidth * mapHeight)/100;
	for (int i = 0; i < numCrates; i++) {
		// find free spot for crate
		int x, y;
		do {
			x = rand() % mapWidth;
			y = rand() % mapHeight;
		} while (isOpaque(x, y));
		state.addCrateSpawn({ x * 32.f, y * 32.f });
	}
};

void TileMap::setTile(unsigned x, unsigned y, tileType type) {
	if (x >= mapWidth || y >= mapHeight)
		return;

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
			if (x + x_off > mapWidth || y + y_off > mapHeight)
				break;
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
	if (i <= 49)
		return { (i - 42) * 32, 768 };
	if (i <= 57)
		return { (i - 50) * 32, 800 };
	if (i <= 63)
		return { (i - 58) * 32, 832 };

	// default set to red rug
	return { 224, 832 };
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::Sprite sprTile;

	const sf::Vector2f& viewSize = target.getView().getSize();
	const sf::Vector2f& viewCenter = target.getView().getCenter();
	sf::IntRect viewBounds(sf::FloatRect({ viewCenter.x - viewSize.x/2, viewCenter.y - viewSize.y/2, viewSize.x, viewSize.y }));

	int left = std::max(viewBounds.left / TILE_SIZE, 0);
	int top = std::max(viewBounds.top / TILE_SIZE, 0);
	int right = std::min((viewBounds.left + viewBounds.width) / TILE_SIZE + 1, mapWidth);
	int bottom = std::min((viewBounds.top + viewBounds.height) / TILE_SIZE + 1, mapHeight);

	for (unsigned x = left; x < right; x++) {
		for (unsigned y = top; y < bottom; y++) {
			const Tile& tile = map[y][x];
			// no tile - draw grass (63)
			if (tile.type == 0) {
				sprTile.setTexture(*textures[63]);
				sprTile.setPosition(x * TILE_SIZE, y * TILE_SIZE);
				target.draw(sprTile, states);
				continue;
			}
			sprTile.setTexture(*textures[tile.type]);
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

std::vector<std::vector<Tile>> TileMap::getTileMap()
{
	return map;
}

void TileMap::setTileMap(std::vector<std::vector<Tile>>& map)
{
	this->map = map;
}
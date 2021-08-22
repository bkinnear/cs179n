#include "tilemap.hpp"
#include "state.hpp"
#include "gamemode.hpp"

#include <time.h>
#include <vector>

#include <iostream>

/* Hard coded building presets */

struct Building {
	Building::Building(const std::vector<std::vector<Tile>>& tiles) : tiles(tiles) {};
	std::vector<std::vector<Tile>> tiles;

	int getHeight() const {
		return tiles.size();
	};

	int getWidth() const {
		return tiles.at(0).size();
	};
};

Building building1(
{
{8, 9, 9, 42, 9, 9, 10},
{16, 17, 17, 17, 17, 51, 18},
{16, 17, 17, 65, 17, 17, 18},
{16, 17, 17, 55, 17, 17, 18},
{16, 17, 17, 17, 17, 17, 18},
{16, 17, 17, 17, 17, 17, 18},
{24, 25, 25, 42, 25, 25, 26}
});

Building building2(
{
{8, 9, 9, 9, 10},
{16, 55, 53, 17, 18},
{16, 17, 17, 17, 18},
{16, 17, 17, 17, 18},
{24, 42, 25, 25, 26}
});

Building building3(
{
{8, 42, 9, 9, 39,  10},
{16, 17, 57, 48, 17, 18},
{16, 17, 17, 17, 17, 18},
{16, 17, 17, 17, 17, 18},
{16, 17, 17, 17, 17, 18},
{24, 25, 42, 25, 25, 26}
});

Building building4(
{
{8, 39, 9, 9, 42, 10},
{16, 49, 51, 52, 17, 18},
{16, 17, 17, 17, 17, 18},
{16, 17, 17, 17, 17, 18},
{16, 17, 17, 17, 17, 18},
{24, 25, 42, 25, 25, 26}
});

Building building5(
{
{8, 42, 9, 9, 9, 10},
{16, 17, 17, 51, 52, 18},
{16, 17, 17, 17, 17, 18},
{16, 17, 17, 17, 17, 18},
{16, 17, 17, 17, 17, 18},
{24, 25, 25, 42, 25, 26}
});

Building buildingsmall(
{
{8, 9, 9, 10},
{16, 17, 17, 18},
{16, 17, 17, 18},
{24, 42, 25, 26},
});

void setLootSpawn(GameMode& gmode, int x, int y, Tile tile) {
	// add item spawnpoint for certain tiles that should spawn loot
	switch (tile) {
	case 51:
	case 52:
	case 54:
	case 55:
	case 57:
	case 63:
	case 64:
		gmode.addLootSpawn({ x * 32.f + 4.f, y * 32.f + 4.f});
		break;
	}
}

// returns if tile can be passed through
bool isOpaque(Tile tile) {
	switch (tile) {
	case 0:
	case 15:
	case 17:
	case 22:
	case 23:
	case 43:
	case 77:
	case 78:
		return false;
		break;
	default:
		return true;
	}
}

// returns if tile can be shot through
bool isOpen(Tile tile) {
	switch (tile) {
	case 0:
	case 15:
	case 17:
	case 22:
	case 23:
	case 31:
	case 43:
	case 47:
	case 48:
	case 49:
	case 50:
	case 51:
	case 52:
	case 53:
	case 54:
	case 55:
	case 57:
	case 58:
	case 65:
	case 66:
	case 67:
	case 77:
	case 78:
		return true;
		break;
	default:
		return false;
	}
}

TileMap::TileMap(unsigned mapWidth, unsigned mapHeight):
	width(mapWidth),
	height(mapHeight)
{}

void TileMap::loadTextures(GameMode* gmode) {
	// load tile textures
	int nTextures = 80; // TODO make this not a magic number
	textures.resize(nTextures);
	for (int i = 0; i < nTextures; i++) {
		// create new texture for tile of type i
		textures[i] = &gmode->createTexture(
			"res/tileset.png", // TODO make this not a magic string (maybe doesnt matter)
			sf::IntRect(getTileTexOffset(i), { TILE_SIZE, TILE_SIZE }
		));
	}
}

void TileMap::generate(GameMode* gmode) {
	// change state to a reference
	GameMode& state = *gmode;

	loadTextures(gmode);

	// initialize map
	map.resize(height);
	for (unsigned y = 0; y < height; y++) {
		map[y].resize(width);
		for (unsigned x = 0; x < width; x++) {
			// goes over every tile
			// set all tiles to empty (0)
			setTile(x, y, 0);
		}
	}

	// populate building list
	std::vector<Building> buildings;
	buildings.push_back(building1);
	buildings.push_back(building2);
	buildings.push_back(building3);
	buildings.push_back(building4);
	buildings.push_back(building5);
	buildings.push_back(buildingsmall);

	// place example building
	srand(time(NULL));
	int num_bldg = 50;
	
	for (int i = 1; i <= num_bldg; i++) {
		int bldg_num = rand() % buildings.size();		//randomly selects one of the building presets
		int x_offset = rand() % (width-12) + 2;		//generates a random number to determine the x offset
		int y_offset = rand() % (height-12) + 2;		//generates a random number to determine the y offset
		bool empty;

		sf::Vector2i offset = {y_offset,x_offset};

		if (tileClear(buildings[bldg_num].getHeight(), buildings[bldg_num].getWidth(), offset.y, offset.x)) {
			for (unsigned y = 0; y < buildings[bldg_num].getHeight(); y++) {
				for (unsigned x = 0; x < buildings[bldg_num].getWidth(); x++) {
					setTile(x + offset.x, y + offset.y, buildings[bldg_num].tiles[y][x]);
					setLootSpawn(state, x + offset.x, y + offset.y, buildings[bldg_num].tiles[y][x]);
				}
			}
			state.addHiddenArea({ (offset.x + 1) * 32.f, (offset.y + 1) * 32.f - 20.f, (buildings[bldg_num].getWidth() - 2) * 32.f, (buildings[bldg_num].getHeight() - 2) * 32.f + 20.f });
		}
	}

	// find and place crate spawn points
	int numCrates = height/4; // set num of crates to 1/4 the map hiehgt for sparse crate distribution
	for (int i = 0; i < numCrates; i++) {
		// find free spot for crate
		int x, y;
		do {
			x = rand() % width;
			y = rand() % height;
		} while (isOpaqueAt(x, y));
		state.addCrateSpawn({ x * 32.f, y * 32.f });
	}
};

void TileMap::loadMap(GameMode* gmode, const std::string& fname)
{
	loadTextures(gmode);

	std::ifstream input(fname.c_str());
	std::string line;
	std::vector<std::vector<unsigned>> parsedMap;
	while (std::getline(input, line))
	{
		std::stringstream lineStream(line);
		std::string tile;
		std::vector<unsigned> parsedRow;
		while (std::getline(lineStream, tile, ','))
		{
			parsedRow.push_back((unsigned)std::stoi(tile));
		}

		parsedMap.push_back(parsedRow);
	}

	// return if empty map loaded
	if (parsedMap.size() == 0)
		throw std::runtime_error("map csv has invalid size 0");
	else if (parsedMap.at(0).size() == 0)
		throw std::runtime_error("map csv has invalid size 0");

	// set map width and height
	width = parsedMap.at(0).size();
	height = parsedMap.size();
	std::cout << "Loaded map of size " << width << 'x' << height << std::endl;

	// initialize map
	map.resize(height);
	for (unsigned y = 0; y < height; y++) {
		map[y].resize(width);
		for (unsigned x = 0; x < width; x++) {
			// set all tiles to value from csv
			setTile(x, y, (Tile)parsedMap.at(y).at(x));
		}
	}
	std::cout << "done loading level" << std::endl;
};

void TileMap::saveMap(int gameType)
{
	std::string fileName = "res/maps/save/";
	if (gameType == 1)
	{
		fileName = fileName + "map_Endless.csv";
	}
	else if (gameType == 2)
	{
		fileName = fileName + "map_Survival.csv";
	}
	std::ofstream out(fileName);
	for (unsigned i = 0;i < height;i++)
	{
		for (unsigned j = 0;j < width;j++)
		{
			Tile tile = map[j][i];
			out << (int) tile <<",";
		}
		out << "\n";
	}
	std::cout << "\nFile written in " << fileName<<"\n";
	out.close();
}
void TileMap::setTile(unsigned x, unsigned y, Tile tile) {
	if (x >= width || y >= height)
		return;

	map[y][x] = tile;
}

bool TileMap::isOpaqueAt(float x, float y) const {
	// convert from world to map coordinates
	unsigned tileX = int(x / TILE_SIZE);
	unsigned tileY = int(y / TILE_SIZE);

	if (tileX >= 0 && tileX < width && tileY >=0 && tileY < height)
		return isOpaque(map[tileY][tileX]);

	return true;
}

bool TileMap::isOpenAt(float x, float y) const {
	// convert from world to map coordinates
	unsigned tileX = int(x / TILE_SIZE);
	unsigned tileY = int(y / TILE_SIZE);

	if (tileX >= 0 && tileX < width && tileY >= 0 && tileY < height)
		return isOpen(map[tileY][tileX]);

	return true;
}

Tile TileMap::getTile(unsigned x, unsigned y) const {
	x = int(x / TILE_SIZE);
	y = int(y / TILE_SIZE);

	if (x >= 0 && x < width && y >= 0 && y < height)
	{
		return map[y][x];
	}
	return -1;
}

bool TileMap::isDoor(unsigned x, unsigned y) const {
	x = int(x / TILE_SIZE);
	y = int(y / TILE_SIZE);

	if (x >= 0 && x < width && y >= 0 && y < height)
	{
		switch (map[y][x])
		{
			case TILE_DOOR_CLOSED:
			case TILE_DOOR_OPEN:
				return true;
			default:
				return false;
		}
	}
	return false;
}
bool TileMap::isDoorOpen(unsigned x, unsigned y) const {
	x = int(x / TILE_SIZE);
	y = int(y / TILE_SIZE);

	if (x >= 0 && x < width && y >= 0 && y < height)
	{
		switch (map[y][x])
		{
		case TILE_DOOR_OPEN:
			return true;
		default:
			return false;
		}
	}
	return false;
}
bool TileMap::tileClear(unsigned h, unsigned w, unsigned y_off, unsigned x_off)
{
	for (unsigned y = 0; y < h; y++) {
		for (unsigned x = 0; x < w; x++) {
			if (x + x_off > width || y + y_off > height)
				break;
			if (map[y + y_off][x + x_off] == 0) {
				if (y == 0 && x == 0) {
					if (map[y + y_off - 2][x + x_off - 2] != 0 ||
						map[y + y_off - 2][x + x_off - 1] != 0 ||
						map[y + y_off - 1][x + x_off - 2] != 0 ||
						map[y + y_off - 1][x + x_off - 1] != 0) {
						return false;
					}
				}
				if (y == h - 1 && x == w - 1) {
					if (map[y + y_off + 2][x + x_off + 2] != 0 ||
						map[y + y_off + 2][x + x_off + 1] != 0 ||
						map[y + y_off + 1][x + x_off + 2] != 0 ||
						map[y + y_off + 1][x + x_off + 1] != 0) {
						return false;
					}
				}
				if (y == 0 && x == w - 1) {
					if (map[y + y_off - 2][x + x_off + 2] != 0 ||
						map[y + y_off - 2][x + x_off + 1] != 0 ||
						map[y + y_off - 1][x + x_off + 2] != 0 ||
						map[y + y_off - 1][x + x_off + 1] != 0) {
						return false;
					}
				}
				if (y == h - 1 && x == 0) {
					if (map[y + y_off + 2][x + x_off - 2] != 0 ||
						map[y + y_off + 2][x + x_off - 1] != 0 ||
						map[y + y_off + 1][x + x_off - 2] != 0 ||
						map[y + y_off + 1][x + x_off - 1] != 0) {
						return false;
					}
				}
				if (y == 0) {
					if (map[y + y_off - 2][x + x_off] != 0 ||
						map[y + y_off - 1][x + x_off] != 0) {
						return false;
					}
				}
				if (x == 0) {
					if (map[y + y_off][x + x_off - 2] != 0 ||
						map[y + y_off][x + x_off - 1] != 0) {
						return false;
					}
				}
				if (y == h - 1) {
					if (map[y + y_off + 2][x + x_off] != 0 ||
						map[y + y_off + 1][x + x_off] != 0) {
						return false;
					}
				}
				if (x == w - 1) {
					if (map[y + y_off][x + x_off + 2] != 0 ||
						map[y + y_off][x + x_off + 1] != 0) {
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

sf::Vector2i TileMap::getTileTexOffset(Tile type) {
	int i = (int)type;

	return { (i * 32) % 256, ((i * 32) / 256) * 32 };
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::Sprite sprTile;

	const sf::Vector2f& viewSize = target.getView().getSize();
	const sf::Vector2f& viewCenter = target.getView().getCenter();
	sf::IntRect viewBounds(sf::FloatRect({ viewCenter.x - viewSize.x/2, viewCenter.y - viewSize.y/2, viewSize.x, viewSize.y }));

	int left = std::max(viewBounds.left / TILE_SIZE, 0);
	int top = std::max(viewBounds.top / TILE_SIZE, 0);
	int right = std::min((viewBounds.left + viewBounds.width) / TILE_SIZE + 1, (int)width);
	int bottom = std::min((viewBounds.top + viewBounds.height) / TILE_SIZE + 1, (int)height);

	for (unsigned x = left; x < right; x++) {
		for (unsigned y = top; y < bottom; y++) {
			const Tile& tile = map[y][x];
			sprTile.setTexture(*textures[tile]);
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

	bool lt = isOpaqueAt(bounds.left, bounds.top);
	bool rt = isOpaqueAt(bounds.left + bounds.width, bounds.top);
	bool ll = isOpaqueAt(bounds.left, bounds.top + bounds.height);
	bool lr = isOpaqueAt(bounds.left + bounds.width, bounds.top + bounds.height);

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
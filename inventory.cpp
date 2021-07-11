#include "inventory.hpp"

// returns inventory slot offset
inline sf::Vector2f getSlotOffset(unsigned x, unsigned y) {
	float dx, dy;

	dx = 29 + x * 53;
	dy = 74 + y * 53;

	return { dx, dy };
}

// returns item texture offset
inline sf::Vector2i getItemTexOffset(Item::type type) {
	int dx;

	dx = ((int)type - 1) * 48;

	return { dx, 0 };
}

Inventory::Inventory(sf::Texture& inventoryTexture, sf::Texture& itemTileset):
	sprInventory(inventoryTexture),
	texItemTileset(itemTileset)
{
	sprInventory.setPosition(64.f, 64.f);

	// initialize the inventory grid
	inventoryGrid.resize(height);
	for (unsigned y = 0; y < height; y++) {
		inventoryGrid[y].resize(width);
	}
}

void Inventory::addItem(Item::type type, unsigned num) {
	// search for first item
	// store first empty position in case we need new slot
	unsigned openX = 0, openY = 0;
	bool hasOpenSlot = false;
	for (unsigned y = 0; y < height; y++) {
		for (unsigned x = 0; x < width; x++) {
			if (inventoryGrid[y][x].itemType == type) {
				inventoryGrid[y][x].num += num;
				return;
			} 
			else if (!hasOpenSlot && inventoryGrid[y][x].itemType == Item::type::null) {
				hasOpenSlot = true;
				openX = x;
				openY = y;
			}
		}
	}

	if (hasOpenSlot) {
		inventoryGrid[openY][openX].itemType = type;
		inventoryGrid[openY][openX].num = num;
		return;
	}

	// if no slots open, code will return here
}

void Inventory::moveItem(unsigned x, unsigned y, unsigned x2, unsigned y2) {
	// swaps item at <x, y> with item at <x2, y2>
	Item temp = inventoryGrid[x2][y2];
	inventoryGrid[x2][y2] = inventoryGrid[x][y];
	inventoryGrid[x][y] = temp;
}

void Inventory::removeItem(Item::type type, unsigned num) {
	// search for item
	for (unsigned y = 0; y < height; y++) {
		for (unsigned x = 0; x < width; x++) {
			if (inventoryGrid[y][x].itemType == type) {
				Item& item = inventoryGrid[x][y];
				if (item.num <= num) {
					// no items left - set item type to null
					item.itemType = Item::type::null;
				}
				else {
					inventoryGrid[y][x].num -= num;
				}
				return;
			}
		}
	}

	// if item not found, code will reach here
}

const Item* Inventory::getItemAt(float x, float y) const {
	// return nullptr if not in inventory
	if (!sprInventory.getGlobalBounds().contains(x, y))
		return nullptr;

	// get local (x, y)
	x = x - sprInventory.getPosition().x;
	y = y - sprInventory.getPosition().y;

	// check if in wielded slot
	if (sf::FloatRect(241.f, 12.f, 48.f, 48.f).contains(x, y))
		return &wielded;

	// get grid location using offsets
	int gridX = (x - 29.f) / 53.f;
	int gridY = (y - 74.f) / 53.f;

	// return nullptr if not in grid space
	if (gridX < 0 || gridX >= width || gridY < 0 || gridY >= height)
		return nullptr;

	// see if item is actually in the slot at [gridX][gridY]
	sf::FloatRect slotBounds(getSlotOffset(gridX, gridY), { 48.f, 48.f });
	if (slotBounds.contains(x, y))
		return &(inventoryGrid[gridY][gridX]);
	else
		return nullptr;
}

void Inventory::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	// draw actual inventory menu
	target.draw(sprInventory, states);

	sf::Sprite sprItem(texItemTileset);
	// draw each item in inventory
	for (unsigned y = 0; y < height; y++) {
		for (unsigned x = 0; x < width; x++) {
			const Item& item = inventoryGrid[y][x];
			if (item.itemType == Item::type::null)
				continue;
			sprItem.setTextureRect(sf::IntRect(getItemTexOffset(item.itemType), { 48, 48 }));
			sprItem.setPosition(sprInventory.getPosition() + getSlotOffset(x, y));
			target.draw(sprItem, states);
		}
	}

	// draw wielded item
	if (wielded.itemType != Item::type::null) {
		sprItem.setTextureRect(sf::IntRect(getItemTexOffset(wielded.itemType), { 48, 48 }));
		sprItem.setPosition(sprInventory.getPosition() + sf::Vector2f(241.f, 12.f));
		target.draw(sprItem, states);
	}
}
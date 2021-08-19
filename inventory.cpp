#include "inventory.hpp"
#include <iostream>
#include <algorithm>

// returns inventory slot offset
inline sf::Vector2f getSlotOffset(unsigned x, unsigned y) {
	float dx, dy;

	dx = 29 + x * 53;
	dy = 74 + y * 53;

	return { dx, dy };
}

// returns item texture offset
sf::Vector2i getItemTexOffset(Item::type type) {
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
	// ignore if adding nothing
	if (num == 0)
		return;

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
	Item temp = inventoryGrid[y2][x2];
	inventoryGrid[y2][x2] = inventoryGrid[y][x];
	inventoryGrid[y][y] = temp;
}

unsigned Inventory::getNumItem(Item::type type) {
	// search for item and return number
	for (unsigned y = 0; y < height; y++)
		for (unsigned x = 0; x < width; x++)
			if (inventoryGrid[y][x].itemType == type)
				return inventoryGrid[y][x].num;

	// if item not found, code will reach here
	return 0;
}

void Inventory::removeItem(Item::type type, unsigned num) {
	if (num == 0)
		return;

	// search for item
	for (unsigned y = 0; y < height; y++) {
		for (unsigned x = 0; x < width; x++) {
			if (inventoryGrid[y][x].itemType == type) {
				Item& item = inventoryGrid[y][x];
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

	// see if item is actually in the slot at [gridY][gridX]
	sf::FloatRect slotBounds(getSlotOffset(gridX, gridY), { 48.f, 48.f });
	if (slotBounds.contains(x, y))
		return &(inventoryGrid[gridY][gridX]);
	else
		return nullptr;
}

void Inventory::wieldItemAt(float x, float y) {
	// return if not in inventory
	if (!sprInventory.getGlobalBounds().contains(x, y))
		return;

	// get local (x, y)
	x = x - sprInventory.getPosition().x;
	y = y - sprInventory.getPosition().y;

	// unwield if in wielded slot
	if (sf::FloatRect(241.f, 12.f, 48.f, 48.f).contains(x, y)) {
		addItem(wielded.itemType, wielded.num);
		
		// return ammo in mag to inventory
		addItem(wielded.getAmmoType(), roundsLeft);

		// set wielded slot to empty
		wielded = Item();
	}

	// get grid location using offsets
	int gridX = (x - 29.f) / 53.f;
	int gridY = (y - 74.f) / 53.f;

	// return if not in grid space
	if (gridX < 0 || gridX >= width || gridY < 0 || gridY >= height)
		return;

	// see if item is actually in the slot at [gridY][gridX]
	sf::FloatRect slotBounds(getSlotOffset(gridX, gridY), { 48.f, 48.f });
	if (slotBounds.contains(x, y)) {
		// unwield old item
		Item toWield = inventoryGrid[gridY][gridX];
		inventoryGrid[gridY][gridX] = Item();
		addItem(wielded.itemType, wielded.num);

		// wield the item
		wielded = toWield;

		// reset wielded item variables
		roundsLeft = 0;
		weaponWaitTick = -1;
		weaponReloadTick = -1;

		// automatically reload new weapon
		reloadWielded();
	}
}

const Item& Inventory::getWielded() const {
	return wielded;
}

void Inventory::reloadWielded() {
	// ignore if mag is full
	if (roundsLeft == wielded.getMagCapacity())
		return;
	// ignore if already reloading
	if (weaponReloadTick >= 0)
		return;

	weaponReloadTick = wielded.getReloadTime() * 60;
	weaponReady = false;
}

int Inventory::getRoundsLeft() const {
	return roundsLeft;
}

bool Inventory::useWielded() {
	switch (wielded.itemType) {
	case Item::type::MP5:
		if (weaponReady) {
			if (roundsLeft == 0) {
				reloadWielded();
				return false;
			}
			roundsLeft--;
			weaponReady = false;
			weaponWaitTick = wielded.getDelayTime();
			return true;
		}
		else
			return false;
		break;
	case Item::type::M4:
		if (weaponReady) {
			if (roundsLeft == 0) {
				reloadWielded();
				return false;
			}
			roundsLeft--;
			weaponReady = false;
			weaponWaitTick = wielded.getDelayTime();
			return true;
		}
		else
			return false;
		break;
	default:
		return false; // Return false, not wielding an item that can be used
	}
}

bool Inventory::useWieldedMelee() {
	switch (wielded.itemType) {
	case Item::type::MP5:
		if (weaponReady) {
			weaponReady = false;
			weaponWaitTick = wielded.getMeleeDelayTime();
			return true;
		}
		else
			return false;
	case Item::type::M4:
		if (weaponReady) {
			weaponReady = false;
			weaponWaitTick = wielded.getMeleeDelayTime();
			return true;
		}
		else
			return false;
	case Item::type::dagger:
		if (weaponReady) {
			weaponReady = false;
			weaponWaitTick = wielded.getMeleeDelayTime();
			return true;
		}
		else
			return false;
	case Item::type::baseball_bat:
		if (weaponReady) {
			weaponReady = false;
			weaponWaitTick = wielded.getMeleeDelayTime();
			return true;
		}
		else
			return false;
	default:
		if (weaponReady) {
			weaponReady = false;
			weaponWaitTick = wielded.getMeleeDelayTime();
			return true; // Return true, not wielding an item that can be used, but will instead punch with hands
		}
		else {
			return false;
		}
	}
}

void Inventory::tick() {
	if (weaponWaitTick > 0)
		weaponWaitTick--;
	else if (weaponWaitTick == 0) {
		// weapon ready to fire again
		weaponWaitTick = -1; // stop ticking
		if (weaponReloadTick == -1)
			weaponReady = true;
	}

	if (weaponReloadTick > 0)
		weaponReloadTick--;
	else if (weaponReloadTick == 0) {
		// reload weapon
		weaponReloadTick = -1; // stop ticking
		int roundsNeeded = wielded.getMagCapacity() - roundsLeft; // rounds needed to "top off" mag
		int roundsToReload = std::min((int)getNumItem(wielded.getAmmoType()), roundsNeeded); // rounds that will actually be used up
		removeItem(wielded.getAmmoType(), roundsToReload); // remove rounds being used
		roundsLeft += roundsToReload; // add rounds to count of rounds in mag
		weaponReady = true;
	}
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
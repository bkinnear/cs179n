#include "inventory.hpp"

void Inventory::addItem(Item::type type, unsigned num) {
	// search for first item
	// store first empty position in case we need new slot
	unsigned openX = 0, openY = 0;
	bool hasOpenSlot = false;
	for (unsigned y = 0; y < height; y++) {
		for (unsigned x = 0; x < width; x++) {
			if (inventory[y][x].itemType == type) {
				inventory[y][x].num += num;
				return;
			} 
			else if (!hasOpenSlot && inventory[y][x].itemType == Item::type::null) {
				hasOpenSlot = true;
				openX = x;
				openY = y;
			}
		}
	}

	if (hasOpenSlot) {
		inventory[openY][openX].itemType = type;
		inventory[openY][openX].num = num;
		return;
	}

	// if no slots open, code will return here
}

void Inventory::moveItem(unsigned x, unsigned y, unsigned x2, unsigned y2) {
	// swaps item at <x, y> with item at <x2, y2>
	Item temp = inventory[x2][y2];
	inventory[x2][y2] = inventory[x][y];
	inventory[x][y] = temp;
}

void Inventory::removeItem(Item::type type, unsigned num) {
	// search for item
	for (unsigned y = 0; y < height; y++) {
		for (unsigned x = 0; x < width; x++) {
			if (inventory[y][x].itemType == type) {
				Item& item = inventory[x][y];
				if (item.num <= num) {
					// no items left - set item type to null
					item.itemType = Item::type::null;
				}
				else {
					inventory[y][x].num -= num;
				}
				return;
			}
		}
	}

	// if item not found, code will reach here
}

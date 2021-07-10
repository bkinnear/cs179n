#ifndef INVENTORY_H_
#define INVENTORY_H_

#include <vector>
#include <string>

/* Item in inventory
*/
struct Item {
	// different types of items
	enum type { null, MP5, ammo_9mm, M4, ammo_556 };

	// type of item
	type itemType = null;

	// number of items in stack
	unsigned num = 0;

	// returns name of item type
	std::string getName() {
		switch (itemType) {
		case null:
			return "null item";
		case MP5:
			return "MP5 SMG";
		case ammo_9mm:
			return "9mm Rounds";
		case M4:
			return "M4 Rifle";
		case ammo_556:
			return "5.56 Rounds";
		default:
			return "unknown item";
		}
	}

	// returns description of item type
	std::string getDescription() {
		switch (itemType) {
		case null:
			return "null item";
		case MP5:
			return "The MP5 submachinegun";
		case ammo_9mm:
			return "9mm rounds. Used for SMGs and pistols";
		case M4:
			return "The military M4 automatic rifle.";
		case ammo_556:
			return "5.56 Rounds. Used for rifles.";
		default:
			return "unknown item desc";
		}
	}
};

/* Inventory object
 * 
*/
class Inventory {
public:	
	/* adds item to next available slot in inventory
	 * increases item count if item type already in inventory
	*/
	void addItem(Item::type item, unsigned num);

	/* moves item from inventory grid at position (x, y) to (x2, y2)
	 * swaps items if destination position is non-empty
	*/
	void moveItem(unsigned x, unsigned y, unsigned x2, unsigned y2);

	/* removes item from inventory
	 * decreases item count if not item type is present
	*/
	void removeItem(Item::type item, unsigned num);

	// inventory grid width
	const unsigned width = 5;

	// inventory grid height
	const unsigned height = 3;
private:
	// inventory grid
	std::vector<std::vector<Item>> inventory;

	// wielded item
	Item wielded;
};

#endif
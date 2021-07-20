#ifndef INVENTORY_H_
#define INVENTORY_H_

#include "game.hpp"
#include <vector>
#include <string>


/* Item in inventory
*/
struct Item {
	// different types of items
	enum class type { null, MP5, ammo_9mm, M4, ammo_556 };

	// type of item
	type itemType = type::null;

	// number of items in stack
	unsigned num = 0;

	// returns name of item type
	std::string getName() const {
		switch (itemType) {
		case type::null:
			return "nothing";
		case type::MP5:
			return "MP5 SMG";
		case type::ammo_9mm:
			return "9mm Rounds";
		case type::M4:
			return "M4 Rifle";
		case type::ammo_556:
			return "5.56 Rounds";
		default:
			return "unknown item";
		}
	}

	// returns description of item type
	std::string getDescription() const {
		switch (itemType) {
		case type::null:
			return "empty slot";
		case type::MP5:
			return "The MP5 submachinegun";
		case type::ammo_9mm:
			return "9mm rounds. Used for SMGs and pistols";
		case type::M4:
			return "The military M4 automatic rifle.";
		case type::ammo_556:
			return "5.56 Rounds. Used for rifles.";
		default:
			return "unknown item desc";
		}
	}
};

sf::Vector2i getItemTexOffset(Item::type type); //declared here because inventory.cpp has an inline call, but endless.cpp needs to use it

/* Inventory object
 * 
*/
class Inventory: public sf::Drawable {
public:	
	Inventory(sf::Texture& inventoryTexture, sf::Texture& itemTileset);

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

	/* returns pointer to item at position (x, y) in gui
	 * returns nullptr if (x, y) is not in grid space
	*/
	const Item* getItemAt(float x, float y) const;

	/* attempts to wield item at position (x, y) in gui
	*/
	void wieldItemAt(float x, float y);

	// inventory grid width
	const unsigned width = 5;

	// inventory grid height
	const unsigned height = 3;

	// item texture set
	sf::Texture& texItemTileset;
protected:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	// inventory sprites
	sf::Sprite sprInventory;

	// inventory grid
	std::vector<std::vector<Item>> inventoryGrid;

	// wielded item
	Item wielded;
};

#endif
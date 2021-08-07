#ifndef INVENTORY_H_
#define INVENTORY_H_

#include "game.hpp"
#include <vector>
#include <string>


/* Item in inventory
*/
struct Item {
	// different types of items
	enum class type { null, MP5, ammo_9mm, M4, ammo_556, medkit, health_pack, ammo_crate, barrel, dagger, baseball_bat, shield};

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
		case type::medkit:
			return "Medkit";
		case type::health_pack:
			return "Health Pack";
		case type::ammo_crate:
			return "Ammo Crate";
		case type::dagger:
			return "Dagger";
		case type::baseball_bat:
			return "Baseball Bat";
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
		case type::medkit:
			return "Medkit. Used for healing (+50).";
		case type::health_pack:
			return "Health Pack. Used for minor healing (+10).";
		case type::ammo_crate:
			return "Ammo Crate. Used to replenish ammunition";
		case type::dagger:
			return "Dagger. Used to stab enemies.";
		case type::baseball_bat:
			return "Baseball Bat. Used to bash enemies.";
		default:
			return "unknown item desc";
		}
	}

	// returns the damage dealt by a wielded item
	int getDamage() const {
		switch (itemType) {
		case type::MP5:
			return 25;
		case type::M4:
			return 35;
		case type::dagger:
			return 40;
		case type::baseball_bat:
			return 50;
		default:
			return 0;
		}
	}

	// returns the magazine capacity held by a wielded item
	int getMagCapacity() const {
		switch (itemType) {
		case type::MP5:
			return 30;
		case type::M4:
			return 30;
		default:
			return 0;
		}
	}

	// returns reload time for each weapon in seconds
	int getReloadTime() const {
		switch (itemType) {
		case type::MP5:
			return 3;
		case type::M4:
			return 4;
		default:
			return 0;
		}
	}

	// returns ammo type of weapon
	type getAmmoType() const {
		switch (itemType) {
		case type::MP5:
			return type::ammo_9mm;
		case type::M4:
			return type::ammo_556;
		default:
			return type::null;
		}
	}

	// returns delay between each shot in ticks (-1 => semi automatic)
	int getDelayTime() const {
		switch (itemType) {
		case type::MP5:
			return 5;
		case type::M4:
			return 4;
		default:
			return -1;
		}
	}

	int getMeleeDelayTime() const {
		switch (itemType) {
		case type::MP5:
			return 75;
		case type::M4:
			return 150;
		case type::dagger:
			return 30;
		case type::baseball_bat:
			return 50;
		default:
			return -1;
		}
	}
};

sf::Vector2i getItemTexOffset(Item::type type); //declared here because inventory.cpp has an inline call, but endless.cpp needs to use it

/* Inventory object
 *
*/
class Inventory : public sf::Drawable {
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

	/* returns number of item type in inventory
	*/
	unsigned getNumItem(Item::type type);

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

	/* returns wielded item
	*/
	const Item& getWielded() const;

	/* reloads wielded weapon
	*/
	void reloadWielded();

	/* returns rounds left in magazine
	*/
	int getRoundsLeft() const;

	/* returns true if weapon can be used
	 * returns false if weapon is not ready to fire, reloading, or out of ammo
	 * "uses" ammo if relevant
	*/
	bool useWielded();

	bool useWieldedMelee();

	/* advance all ticks
	*/
	void tick();

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
	
	// weapon ready to attack
	//bool weaponReady = true;
	
	// rounds left in wieled weapon magazine
	int roundsLeft = 0;

	// weapon ready to attack
	bool weaponReady = true;

	// ticks until weapon can be used again
	int weaponWaitTick = 0;

	// ticks until mag is loaded
	int weaponReloadTick = 0;

};

#endif
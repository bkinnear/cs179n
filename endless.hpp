#ifndef ENDLESS_H_
#define ENDLESS_H_

#include "game.hpp"
#include "state.hpp"
#include "player.hpp"
#include "inventory.hpp"
#include "tilemap.hpp"

class EndlessState : public State {
public:
	EndlessState(Game&);
	~EndlessState();

	virtual void logic();
	virtual void render();

private:
	sf::View mainView;
	sf::View guiView;

	/* ==v== place any vars we need in this state in here ==v== */


	// player
	Player player;

	// inventory
	Inventory inventory;
	bool showInventory = false;
	bool showItemDetails = false;
	sf::Text txtItemDetails;
	sf::RectangleShape shpItemDetails;

	// tile map
	TileMap tileMap;

	// textures
	sf::Texture& texPlayerRight;
	sf::Texture& texPlayerLeft;

	// fonts
	sf::Font font;
};

#endif
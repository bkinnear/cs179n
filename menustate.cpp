#include "menustate.hpp"
#include <iostream>
#include <cstdlib>

// NOTE: we must call the original constructor and pass it the Game pointer
MenuState::MenuState(Game& game):
	State(game),
	shape(50) // initialize our circle
{
	// set circle details
	shape.setFillColor(sf::Color(0, 0, 255));
	shape.setOutlineThickness(2);
	shape.setOutlineColor(sf::Color(255, 0, 0));

	// TODO - allocate our resources here
	// load assets (textures, sounds, fonts, etc.)
	// and do any initialization we need
}

MenuState::~MenuState() {
	// here we would deallocate any resources we use in this gamestate
}

void MenuState::logic() {
	// handle events
	sf::Event e;
	while (game.window.pollEvent(e)) {
		switch (e.type) {
		case sf::Event::Closed:
			// delete this gamestate
			game.deleteState();

			// never try to do anything after deleting the gamestate (we are now in a deallocated object)
			// there are better ways of programming game states, but...
			// I'm trying to keep it simple so we can make persistent game states

			// game.curState is now null, so the main game loop, game.run(), will break
			break;
		case sf::Event::MouseButtonPressed:
			if (e.mouseButton.button == sf::Mouse::Left)
				std::cout << "LMB Pressed" << std::endl;
			break;
		}
	}
}

void MenuState::render() {
	// clear window
	game.window.clear();

	// draw the circle we created in the constructor
	game.window.draw(shape);

	// update window
	game.window.display();
}
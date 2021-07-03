#include "menustate.hpp"
#include <iostream>
#include <cstdlib>

// the main game window
#define gwindow game.window

// NOTE: we must call the original constructor and pass it the Game pointer
MenuState::MenuState(Game& game):
	State(game)
{
	// set view
	view.reset({0.f, 0.f, float(gwindow.getSize().x), float(gwindow.getSize().y)});
	// we must update view any time we change something in it
	gwindow.setView(view);

	// allocate our resources here
	// load assets (textures, sounds, fonts, etc.)
	// and do any initialization we need


	// we're not going to store the texture anywhere (because we don't need to modify)
	// so we just just use the createTexture() return to set the sprite texture
	sprPlayButton.create(createTexture("res/menu_endless_strip.png"), {0, 0, 160, 96}, 2);
	sprPlayButton.setPosition(320.f, 128.f);
}

MenuState::~MenuState() {
	// here we would deallocate any resources we use in this gamestate
}

void MenuState::logic() {
	// get mouse x and y in world coords
	sf::Vector2f mousePos = game.window.mapPixelToCoords(sf::Mouse::getPosition(game.window));

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
				std::cout << "Mouse Pos: " << mousePos.x << ", " << mousePos.y << std::endl;
			break;
		}
	}

	// change play button subsprite if being hovered over
	if (sprPlayButton.getGlobalBounds().contains(mousePos)) {
		sprPlayButton.setIndex(1);
	} else {
		sprPlayButton.setIndex(0);
	}
}

void MenuState::render() {
	// clear window
	game.window.clear();

	// draw the circle we created in the constructor
	game.window.draw(sprPlayButton);

	// update window
	game.window.display();
}
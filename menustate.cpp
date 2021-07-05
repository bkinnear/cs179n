#include "menustate.hpp"

#include <iostream>
#include <cstdlib>
#include "endless.hpp"

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
	sprEndlessButton.create(createTexture("res/menu_endless_strip.png"), {0, 0, 160, 96}, 2);
	sprEndlessButton.setPosition(320.f, 128.f);

	sprYoshi.create(createTexture("res/yoshi_strip.png"), { 0, 0, 79, 79 }, 54);
	sprYoshi.setAnimSpeed(15);
	sprYoshi.setPosition(0, 0);
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
			game.close();

			// never try to do anything after deleting the gamestate (we are now in a deallocated object)
			// there are better ways of programming game states, but...
			// I'm trying to keep it simple so we can make persistent game states

			// game.curState is now null, so the main game loop, game.run(), will break
			break;
		case sf::Event::MouseButtonPressed:
			// user clicked endless button
			if (sprEndlessButton.getGlobalBounds().contains(mousePos)) {
				// this is what changing state needs to look like
				// set new state
				game.setState(new EndlessState(game));

				// delete old state (or not if you want to keep it loaded in RAM to go back)
				delete this;

				// break switch case
				return;
			}

			break;
		}
	}
	
	// change play button subsprite if being hovered over
	if (sprEndlessButton.getGlobalBounds().contains(mousePos)) {
		sprEndlessButton.setIndex(1);
	} else {
		sprEndlessButton.setIndex(0);
	}
}

void MenuState::render() {
	// clear window
	gwindow.clear();

	// draw the menu button
	gwindow.draw(sprEndlessButton);

	sprYoshi.animateFrame();
	gwindow.draw(sprYoshi);

	// update window
	gwindow.display();
}
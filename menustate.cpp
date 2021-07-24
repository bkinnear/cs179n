#include "menustate.hpp"

#include <iostream>
#include <cstdlib>
#include "endless.hpp"
#include "survival.hpp"
#include "classmenu.hpp"
#include "optionsmenu.hpp"

// the main game window
#define gwindow game.window

// NOTE: we must call the original constructor and pass it the Game pointer
MenuState::MenuState(Game& game):
	State(game)
{
	// set view
	view.reset({0.f, 0.f, 1366.f, 768.f});
	// we must update view any time we change something in it
	gwindow.setView(view);

	// allocate our resources here
	// load assets (textures, sounds, fonts, etc.)
	// and do any initialization we need


	// we're not going to store the texture anywhere (because we don't need to modify)
	// so we just just use the createTexture() return to set the sprite texture
	sprEndlessButton.create(createTexture("res/menu_endless_strip.png"), {0, 0, 160, 64}, 2);
	sprEndlessButton.setPosition(320.f, 128.f);

	//Survival Mode Button Placement Start
	sprSurvivalButton.create(createTexture("res/menu_survival_strip.png"), { 0, 0, 160, 64 }, 2);
	sprSurvivalButton.setPosition(320.f, 205.f);
	//Survival Mode Button Placement End

	sprOptionsButton.create(createTexture("res/menu_options.png"), { 0, 0, 160, 64 }, 2);
	sprOptionsButton.setPosition(320.f, 320.f);
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

			// end function so we don't run this state anymore
			return;
		case sf::Event::MouseButtonPressed:
			// user clicked endless button
			if (sprEndlessButton.getGlobalBounds().contains(mousePos)) {
				std::cout << "Starting Endless Mode\n";
				// this is what changing state needs to look like
				// set new state
				game.setState(new ClassMenu(game));

				// delete old state (or not if you want to keep it loaded in RAM to go back)
				delete this;

				// break switch case
				return;
			}

			if (sprSurvivalButton.getGlobalBounds().contains(mousePos)) {
				std::cout << "Starting Survival Mode\n";
				game.setState(new SurvivalState(game));
				delete this;
				return;
			}

			if (sprOptionsButton.getGlobalBounds().contains(mousePos)) {
				game.setState(new OptionsMenu(game));
				delete this;
				return;
			}

			break;
		}
	}
	
	if (sprEndlessButton.getGlobalBounds().contains(mousePos))
		sprEndlessButton.setIndex(1);
	else
		sprEndlessButton.setIndex(0);

	if (sprSurvivalButton.getGlobalBounds().contains(mousePos))
		sprSurvivalButton.setIndex(1);
	else
		sprSurvivalButton.setIndex(0);

	if (sprOptionsButton.getGlobalBounds().contains(mousePos))
		sprOptionsButton.setIndex(1);
	else
		sprOptionsButton.setIndex(0);
}

void MenuState::render() {
	// clear window
	gwindow.clear();

	// draw the menu button
	gwindow.draw(sprEndlessButton);
	gwindow.draw(sprSurvivalButton);
	gwindow.draw(sprOptionsButton);

	// update window
	gwindow.display();
}
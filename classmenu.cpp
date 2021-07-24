#include "classmenu.hpp"

#include <iostream>
#include <cstdlib>
#include "endless.hpp"
#include "survival.hpp"
#include "classes.hpp"

// the main game window
#define gwindow game.window

// NOTE: we must call the original constructor and pass it the Game pointer
ClassMenu::ClassMenu(Game& game) :
	State(game)
{
	// set view
	view.reset({ 0.f, 0.f, float(gwindow.getSize().x), float(gwindow.getSize().y) });
	// we must update view any time we change something in it
	gwindow.setView(view);

	// allocate our resources here
	// load assets (textures, sounds, fonts, etc.)
	// and do any initialization we need

	sprChooseClasses.setTexture(createTexture("res/choose_class.png"));
	sprChooseClasses.setPosition(272.f, 48.f);

	sprMedicButton.create(createTexture("res/class_medic_strip.png"), { 0, 0, 160, 64 }, 2);
	sprMedicButton.setPosition(320.f, 128.f);

	sprAssaultButton.create(createTexture("res/class_assault_strip.png"), { 0, 0, 160, 64 }, 2);
	sprAssaultButton.setPosition(320.f, 205.f);
}

ClassMenu::~ClassMenu() {
	// here we would deallocate any resources we use in this gamestate
}

void ClassMenu::logic() {
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
			// user clicked button
			if (sprMedicButton.getGlobalBounds().contains(mousePos)) {
				std::cout << "Chose Medic\n";
				game.setState(new EndlessState(game, PlayerClass::MEDIC));
				delete this;
				return;
			}
			if (sprAssaultButton.getGlobalBounds().contains(mousePos)) {
				std::cout << "Chose Assault\n";
				game.setState(new EndlessState(game, PlayerClass::ASSAULT));
				delete this;
				return;
			}

			break;
		}
	}

	// change play button subsprite if being hovered over
	if (sprMedicButton.getGlobalBounds().contains(mousePos))
		sprMedicButton.setIndex(1);
	else
		sprMedicButton.setIndex(0);

	if (sprAssaultButton.getGlobalBounds().contains(mousePos))
		sprAssaultButton.setIndex(1);
	else
		sprAssaultButton.setIndex(0);
}

void ClassMenu::render() {
	// clear window
	gwindow.clear();

	gwindow.draw(sprChooseClasses);

	// draw the menu buttons
	gwindow.draw(sprMedicButton);
	gwindow.draw(sprAssaultButton);

	// update window
	gwindow.display();
}
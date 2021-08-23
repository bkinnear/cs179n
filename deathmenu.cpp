#include "deathmenu.hpp"
#include "menustate.hpp"

#include <iostream>
#include <cstdlib>
#include "menustate.hpp"

// the main game window
#define gwindow game.window

// NOTE: we must call the original constructor and pass it the Game pointer
DeathMenu::DeathMenu(Game& game) :
	State(game)
{
	// set view
	view.reset({ 0.f, 0.f, float(gwindow.getSize().x), float(gwindow.getSize().y) });
	// we must update view any time we change something in it
	gwindow.setView(view);

	// allocate our resources here
	// load assets (textures, sounds, fonts, etc.)
	// and do any initialization we need
	sprYouDied.setTexture(createTexture("res/youDied.png"));
	sprYouDied.setPosition(-120.f, -200.f);

	sprReturnButton.create(createTexture("res/menu_main_strip.png"), { 0, 0, 160, 64 }, 2);
	sprReturnButton.setPosition(game.portWidth - 200.f, game.portHeight - 120.f);

}

DeathMenu::~DeathMenu() {
	// here we would deallocate any resources we use in this gamestate
}

void DeathMenu::logic() {
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
			// user clicked return
			if (sprReturnButton.getGlobalBounds().contains(mousePos)) {
				game.menuSelect1.play();
				game.setState(new MenuState(game));
				delete this;
				return;
			}

			break;
		}
	}

	// change play button subsprite if being hovered over
	if (sprReturnButton.getGlobalBounds().contains(mousePos))
		sprReturnButton.setIndex(1);
	else
		sprReturnButton.setIndex(0);
}

void DeathMenu::render() {
	// clear window
	gwindow.clear();

	gwindow.setMouseCursorVisible(true);

	// draw the menu buttons
	gwindow.draw(sprYouDied);
	gwindow.draw(sprReturnButton);

	// update window
	gwindow.display();
}
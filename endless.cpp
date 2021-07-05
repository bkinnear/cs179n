#include "endless.hpp"

#include <iostream>
#include <cstdlib>

// the main game window
#define gwindow game.window

// NOTE: we must call the original constructor and pass it the Game pointer
EndlessState::EndlessState(Game& game) :
	State(game),
	texPlayerRight(createTexture("res/player_r_strip.png")),
	texPlayerLeft(createTexture("res/player_l_strip.png"))
{
	// set view
	view.reset({ 0.f, 0.f, float(gwindow.getSize().x), float(gwindow.getSize().y) });
	// we must update view any time we change something in it
	gwindow.setView(view);

	/*	============================= 
		 allocate our resources here 
		=============================  */

	// create animated sprite for player
	player.create(texPlayerRight, { 0, 0, 32, 32 }, 4);
}

EndlessState::~EndlessState() {
	// here we would deallocate any resources we use in this gamestate
}

void EndlessState::logic() {
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
		case sf::Event::KeyPressed:
			switch (e.key.code) {
			case sf::Keyboard::W:
			case sf::Keyboard::Up:
				player.movingUp = true;
				break;
			case sf::Keyboard::A:
			case sf::Keyboard::Left:
				player.movingLeft = true;
				break;
			case sf::Keyboard::S:
			case sf::Keyboard::Down:
				player.movingDown = true;
				break;
			case sf::Keyboard::D:
			case sf::Keyboard::Right:
				player.movingRight = true;
				break;
			}
			break;
		case sf::Event::KeyReleased:
			switch (e.key.code) {
			case sf::Keyboard::W:
			case sf::Keyboard::Up:
				player.movingUp = false;
				break;
			case sf::Keyboard::A:
			case sf::Keyboard::Left:
				player.movingLeft = false;
				break;
			case sf::Keyboard::S:
			case sf::Keyboard::Down:
				player.movingDown = false;
				break;
			case sf::Keyboard::D:
			case sf::Keyboard::Right:
				player.movingRight = false;
				break;
			}
			break;
		}
	}

	// update player sprite
	if (player.movingLeft || player.movingUp || player.movingRight || player.movingDown) {
		player.setAnimSpeed(12);
		if (player.movingLeft)
			player.setTexture(texPlayerLeft);
		if (player.movingRight)
			player.setTexture(texPlayerRight);
	} 
	else {
		player.setIndex(0);
		player.setAnimSpeed(-1);
	}

	// player movement
	// TODO implement player.speed to determine how fast player moves
	if (player.movingLeft)
		player.move(-2, 0);
	if (player.movingUp)
		player.move(0, -2);
	if (player.movingRight)
		player.move(2, 0);
	if (player.movingDown)
		player.move(0, 2);
}

void EndlessState::render() {
	// clear window
	gwindow.clear(sf::Color(sf::Uint32(0x40AA20FF)));

	// draw the player
	player.animateFrame();
	gwindow.draw(player);

	// update window
	gwindow.display();
}
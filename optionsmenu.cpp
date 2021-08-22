#include "optionsmenu.hpp"
#include "menustate.hpp"

// the main game window
#define gwindow game.window

// NOTE: we must call the original constructor and pass it the Game pointer
OptionsMenu::OptionsMenu(Game& game) :
	State(game)
{
	// set view
	view.reset({ 0.f, 0.f, game.portWidth, game.portHeight });
	// we must update view any time we change something in it
	gwindow.setView(view);

	// allocate our resources here
	// load assets (textures, sounds, fonts, etc.)
	// and do any initialization we need

	sprOptions.setTexture(createTexture("res/options_text.png"));
	sprOptions.setPosition(600.f, 48.f);

	sprFullscreen.create(createTexture("res/options_fullscreen_strip.png"), { 0, 0, 520/2, 64 }, 2);
	sprFullscreen.setPosition(550.f, 128.f);
	if (game.isFullscreen())
		sprFullscreen.setIndex(1);

	sprVsync.create(createTexture("res/options_vsync_strip.png"), { 0, 0, 520/2, 64 }, 2);
	sprVsync.setPosition(550.f, 192.f);
	if (game.usingVsync())
		sprVsync.setIndex(1);

	sprReturnButton.create(createTexture("res/options_return.png"), {0, 0, 160, 64}, 2);
	sprReturnButton.setPosition(game.portWidth - 200.f, game.portHeight - 120.f);
}

OptionsMenu::~OptionsMenu() {
	// here we would deallocate any resources we use in this gamestate
}

void OptionsMenu::logic() {
	// get mouse x and y in world coords
	sf::Vector2f mousePos = game.window.mapPixelToCoords(sf::Mouse::getPosition(game.window));

	// handle events
	sf::Event e;
	while (game.window.pollEvent(e)) {
		switch (e.type) {
		case sf::Event::Closed:
			game.close();
			return;
		case sf::Event::MouseButtonPressed:
			// user clicked fullscreen
			if (sprFullscreen.getGlobalBounds().contains(mousePos)) {
				game.setFullscreen(!game.isFullscreen());

				if (game.isFullscreen())
					sprFullscreen.setIndex(1);
				else
					sprFullscreen.setIndex(0);

				break;
			}

			// user clicked vsync
			if (sprVsync.getGlobalBounds().contains(mousePos)) {
				game.setVsync(!game.usingVsync());

				if (game.usingVsync())
					sprVsync.setIndex(1);
				else
					sprVsync.setIndex(0);

				break;
			}

			// user clicked return
			if (sprReturnButton.getGlobalBounds().contains(mousePos)) {
				game.menuSelect1.play();
				game.setState(new MenuState(game));
				delete this;
				return;
			}

			break;
		case sf::Event::KeyPressed:
			switch (e.key.code) {
			case sf::Keyboard::Escape:
				game.setState(new MenuState(game));
				delete this;
				return;
			}
			break;
		}
	}

	if (sprReturnButton.getGlobalBounds().contains(mousePos))
		sprReturnButton.setIndex(1);
	else
		sprReturnButton.setIndex(0);
}

void OptionsMenu::render() {
	// clear window
	gwindow.clear();

	gwindow.draw(sprOptions);

	// draw the menu buttons
	gwindow.draw(sprFullscreen);
	gwindow.draw(sprVsync);
	gwindow.draw(sprReturnButton);

	// update window
	gwindow.display();
}
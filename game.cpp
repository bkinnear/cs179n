#include "game.hpp"
#include "state.hpp"

Game::Game():
	title("The Last War"),
	window(sf::VideoMode(1366, 768), "The Last War"),
	curState(nullptr)
{
	setVsync(true);

	//set sounds
	if (!menuSongBuffer.loadFromFile("res/menu_music.wav")) {
		std::cout << "error loading menu music" << std::endl;
	}
	menuSong.setBuffer(menuSongBuffer);
	menuSong.setLoop(true);
	menuSong.play();

	if (!menuSelectBuffer1.loadFromFile("res/menu_select1.wav")) {
		std::cout << "error loading menu select 1 sound" << std::endl;
	}
	menuSelect1.setBuffer(menuSelectBuffer1);
	menuSelect1.setVolume(50);

	if (!menuSelectBuffer2.loadFromFile("res/menu_select2.wav")) {
		std::cout << "error loading menu select 2 sound" << std::endl;
	}
	menuSelect2.setBuffer(menuSelectBuffer2);
	menuSelect2.setVolume(50);
	
	if (!toggleBuffer.loadFromFile("res/toggle.wav")) {
		std::cout << "error loading menu toggle sound" << std::endl;
	}
	toggleSound.setBuffer(toggleBuffer);
	toggleSound.setVolume(50);
}

Game::~Game() {
	window.close();
}

void Game::setState(State* newState) {
	curState = newState;
}

State* Game::getState() {
	return curState;
}

void Game::close() {
	delete curState;
	curState = nullptr;
}

void Game::run() {
	// run while curState is not null
	for (;;) {
		// execute game logic
		curState->logic();

		// if curState == null, return
		if (!curState)
			return;

		// render the state
		curState->render();
	}
}

void Game::setFullscreen(bool mode) {
	// mode == true => on
	// mode == false => off 
	
	// do nothing if already in desired fullscreen mode
	if (mode == fullscreen)
		return;
	else
		fullscreen = mode;

	// recreate window with new resolution - use default port width and height for bordered window size
	if (mode)
		window.create(sf::VideoMode::getFullscreenModes()[0], title, sf::Style::Fullscreen);
	else
		window.create(sf::VideoMode(portWidth, portHeight), title);

	// need to reset framerate since we're "recreating" window
	if (usingVsync()) {
		window.setVerticalSyncEnabled(true);
	} else {
		window.setVerticalSyncEnabled(false);
		window.setFramerateLimit(60);
	}
}

bool Game::isFullscreen() const {
	return fullscreen;
}

void Game::setVsync(bool mode) {
	// mode == true => on
	// mode == false => off 

	// do nothing if already in desired fullscreen mode
	if (mode == vsync)
		return;
	else
		vsync = mode;

	if (usingVsync()) {
		window.setVerticalSyncEnabled(true);
	} else {
		window.setVerticalSyncEnabled(false);
		window.setFramerateLimit(60);
	}
}

bool Game::usingVsync() const {
	return vsync;
}
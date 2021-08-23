#ifndef DEATHMENU_H_
#define DEATHMENU_H_


#include "state.hpp"

class DeathMenu : public State {
public:
	DeathMenu(Game&);
	~DeathMenu();

	virtual void logic();
	virtual void render();

private:
	/* view is the camera
	 * this determines what we see in the window
	 * any time we modify this, we need to call window.setView(view)
	 */
	sf::View view;

	// place any vars we need in this state in here

	sf::Sprite sprYouDied;
	AnimSprite sprReturnButton;
};

#endif#pragma once

#ifndef OPTIONSMENU_H_
#define OPTIONSMENU_H_

#include "state.hpp"

class OptionsMenu : public State {
public:
	OptionsMenu(Game&);
	~OptionsMenu();

	virtual void logic();
	virtual void render();

private:
	/* view is the camera
	 * this determines what we see in the window
	 * any time we modify this, we need to call window.setView(view)
	 */
	sf::View view;

	// place any vars we need in this state in here
	sf::Sprite sprOptions;
	AnimSprite sprFullscreen;
	AnimSprite sprVsync;
	AnimSprite sprReturnButton;
};

#endif
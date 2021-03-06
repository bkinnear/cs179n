#ifndef MENUSTATE_H_
#define MENUSTATE_H_

#include "state.hpp"

class MenuState : public State {
public:
	MenuState(Game&);
	~MenuState();

	virtual void logic();
	virtual void render();

private:
	/* view is the camera
	 * this determines what we see in the window
	 * any time we modify this, we need to call window.setView(view)
	 */
	sf::View view;

	// place any vars we need in this state in here
	sf::Sprite sprTitle;
	AnimSprite sprStoryButton;
	AnimSprite sprEndlessButton;
	AnimSprite sprSurvivalButton;
	AnimSprite sprOptionsButton;

	sf::Sprite sprBackground;
};

#endif MENUSTATE_H_
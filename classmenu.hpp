#ifndef CLASSMENU_H_
#define CLASSMENU_H_

#include "state.hpp"

class ClassMenu : public State {
public:
	ClassMenu(Game&);
	~ClassMenu();

	virtual void logic();
	virtual void render();

private:
	/* view is the camera
	 * this determines what we see in the window
	 * any time we modify this, we need to call window.setView(view)
	 */
	sf::View view;

	// place any vars we need in this state in here
	sf::Sprite sprChooseClasses;
	AnimSprite sprMedicButton;
	AnimSprite sprAssaultButton;
	AnimSprite sprSlasherButton;
	AnimSprite sprEngineerButton;
};

#endif
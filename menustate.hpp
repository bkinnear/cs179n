#include "state.hpp"

class MenuState : public State {
public:
	MenuState(Game&);
	~MenuState();

	virtual void logic();
	virtual void render();

private:
	sf::CircleShape shape;
};
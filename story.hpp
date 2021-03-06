#ifndef STORY_H_
#define STORY_H_

#include "gamemode.hpp"

class StoryState : public GameMode {
public:
	StoryState(Game&);
	~StoryState();

	virtual void modeLogic();
	virtual void modeRenderWorld();
	virtual void modeRenderGUI();
	virtual void execTriggers();

private:
	std::list<sf::Sprite> decor;
	std::list<NPC> npcs;
	sf::Sprite introNews;
	AnimSprite flag;

	struct Trigger {
		int index;
		sf::FloatRect area;
	};
	
	std::list<Trigger> triggers;
	bool triggerTimerActive = false;
	sf::Clock triggerTimer;
	float triggerWaitTime = 0.f;
	float triggerWaitIndex = 0;

	void setTimedTrigger(int, float);

	sf::Texture& texSoldierRight;
	sf::Texture& texSoldierLeft;
	sf::Texture& texHumvee;
	sf::Texture& texTent;
};

#endif
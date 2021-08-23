#include "story.hpp"

// the main game window
#define gwindow game.window

// NOTE: we must call the original constructor and pass it the Game pointer
StoryState::StoryState(Game& game) :
	GameMode(MODE_STORY, game, PlayerClass::DEFAULT, gameMeta, npcSaveMeta, enemySaveMeta, inventorySaveMeta, false),
	introNews(createTexture("res/news.png"))
{
	// populate decor
	decor.emplace_back();
	AnimSprite& flag = decor.back();
	flag.create(createTexture("res/jdf_flag_strip.png"), {0, 0, 64, 96}, 8);
	flag.setAnimSpeed(15);
	flag.setPosition(65*32.f, 78*32.f);

	player.setPosition(16*32.f, 12*32.f);

	introShape.setFillColor(sf::Color(0x00, 0x00, 0x00, 0x00));
	introMessage.setFillColor(sf::Color(0xFF, 0xFF, 0xFF, 0x00));

	// place baseball bat in home
	createItem({ 17 * 32.f, 12 * 32.f }, Item::type::baseball_bat);

	// execute intro
	execTriggers();
}

StoryState::~StoryState() {
}

void StoryState::modeLogic() {
	// check trigger timer
	if (triggerTimerActive) {
		if (triggerTimer.getElapsedTime().asSeconds() > triggerWaitTime) {
			triggerIndex = triggerWaitIndex;
			triggerSubIndex = 0;
			execTriggers();
			triggerTimerActive = false;
		}
	}

	// check all triggers
	auto tItr = triggers.begin();
	while (tItr != triggers.end()) {
		Trigger& t = *tItr;
		if (player.getBounds().intersects(t.area)) {
			triggerIndex = t.index;
			triggerSubIndex = 0;
			tItr = triggers.erase(tItr);
			execTriggers();
			continue;
		}

		tItr++;
	}
}

void StoryState::modeRenderWorld() {
	// draw decor sprites
	for (AnimSprite& spr : decor) {
		spr.animateFrame();
		gwindow.draw(spr, &shader);
	}
}

void StoryState::modeRenderGUI() {
	if (triggerIndex == 0 && triggerSubIndex <= 3) {
		gwindow.draw(introNews);
	}
}

void StoryState::setTimedTrigger(int index, float time) {
	triggerTimer.restart();
	triggerWaitTime = time;
	triggerWaitIndex = index;
	triggerTimerActive = true;
}

void StoryState::execTriggers() {
	// execute trigger actions
	switch (triggerIndex) {
	case 0:
		// intro dialog
		switch (triggerSubIndex) {
		case 0:
			frozen = true;
			setDialog("You", "Well at least Japan is isolated from all this.\nThere's no nothing to worry about");
			break;
		case 1:
			setDialog("You", "...");
			break;
		case 2:
			setDialog("You", "Right?");
			break;
		case 3:
			setDialog("You", "Maybe I should check out that new evac center...");
			break;
		case 4:
			frozen = false;
			hideDialog();
			// trigger 1 - step outside door
			triggers.push_back({ 1, {16 * 32.f, 16 * 32.f, 32.f, 32.f} });
			introShape.setFillColor(sf::Color(0x00, 0x00, 0x00, 0xFF));
			introMessage.setFillColor(sf::Color(0xFF, 0xFF, 0xFF, 0xFF));
			triggerSubIndex++;
			break;
		}
		break;
	case 1:
		switch (triggerSubIndex) {
		case 0:
			setDialog("You", "Agh! Don't come any closer!");
			spawnEnemy({ 21 * 32.f, 16 * 32.f });
			break;
		case 1:
			setDialog("You", "I really need find that evac shelter now.\nBefore I get myself killed");
			break;
		case 2:
			setDialog("You", "I think it was South-West of here.\nCan't be too far");
			break;
		case 3:
			hideDialog();
			// trigger 2 - approach soldiers
			triggers.push_back({ 2, {63 * 32.f, 80 * 32.f, 3 * 32.f, 7 * 32.f} });
			break;
		}
		break;
	case 2:
		switch (triggerSubIndex) {
		case 0:
			frozen = true;
			setDialog("Soldier", "Halt! Civilians aren't allowed beyond this point");
			break;
		case 1:
			setDialog("You", "Isn't this the evacuation center?");
			break;
		case 2:
			setDialog("Soldier", "Not anymore. Get lost");
			break;
		case 3:
			frozen = false;
			hideDialog();
			setTimedTrigger(3, 2.f);
			break;
		}
		break;
	case 3:
		switch (triggerSubIndex) {
		case 0:
			setDialog("You", "Well that's not good. I better go find Yamasaki");
			break;
		case 1:
			setDialog("You", "If he's still alive...");
			break;
		case 2:
			setDialog("You", "He should be in the house South of mine");
			break;
		case 3:
			hideDialog();
			break;
		}
		break;
	default:
		break;
	}
}
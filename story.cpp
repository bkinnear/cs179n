#include "story.hpp"

// the main game window
#define gwindow game.window

// NOTE: we must call the original constructor and pass it the Game pointer
StoryState::StoryState(Game& game) :
	GameMode(MODE_STORY, game, PlayerClass::DEFAULT, gameMeta, npcSaveMeta, enemySaveMeta, inventorySaveMeta, false),
	introNews(createTexture("res/news.png")),
	texSoldierRight(createTexture("res/soldier_r_strip.png")),
	texSoldierLeft(createTexture("res/soldier_l_strip.png")),
	texHumvee(createTexture("res/humvee.png")),
	texTent(createTexture("res/tent.png"))
{
	// place flag
	flag.create(createTexture("res/jdf_flag_strip.png"), {0, 0, 64, 96}, 8);
	flag.setAnimSpeed(15);
	flag.setPosition(65*32.f, 78*32.f);

	// place the player in the starting house
	player.setPosition(16*32.f, 12*32.f);

	// place soldiers
	npcs.emplace_back(texSoldierLeft);
	NPC& soldier1 = npcs.back();
	soldier1.setMaskBounds({ 8, 0, 15, 32 });
	soldier1.setPosition({ 65*32.f, 83*32.f });

	// populate decor
	decor.emplace_back(texTent);
	decor.back().setPosition({ 73 * 32.f, 76 * 32.f });
	decor.emplace_back(texTent);
	decor.back().setPosition({ 79 * 32.f, 76 * 32.f });
	decor.emplace_back(texHumvee);
	decor.back().setPosition({ 70 * 32.f, 80 * 32.f });
	decor.emplace_back(texTent);
	decor.back().setPosition({ 73 * 32.f, 85 * 32.f });
	decor.emplace_back(texTent);
	decor.back().setPosition({ 79 * 32.f, 85 * 32.f });

	// dont show intro right away
	introShape.setFillColor(sf::Color(0x00, 0x00, 0x00, 0x00));
	introMessage.setFillColor(sf::Color(0xFF, 0xFF, 0xFF, 0x00));

	// place baseball bat in home
	createItem({ 17 * 32.f, 12 * 32.f }, Item::type::baseball_bat);

	// execute intro
	// execute first dialogue trigger
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
	for (NPC& npc : npcs) {
		npc.animateFrame();
		gwindow.draw(npc);
	}

	// draw decor sprites
	for (sf::Sprite& spr : decor) {
		gwindow.draw(spr, &shader);
	}

	flag.animateFrame();
	gwindow.draw(flag, &shader);
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
	// trigger 0 - intro
	case 0:
		switch (triggerSubIndex) {
		case 0:
			frozen = true;
			setDialog("You", "Well at least Japan is isolated.\nThere's no nothing to worry about");
			break;
		case 1:
			setDialog("You", "...");
			break;
		case 2:
			setDialog("You", "Right?");
			break;
		case 3:
			setDialog("You", "Maybe I should check out that new\nevac center...");
			break;
		case 4:
			frozen = false;
			hideDialog();
			// add next trigger location
			triggers.push_back({ 1, {16 * 32.f, 16 * 32.f, 32.f, 32.f} });
			introShape.setFillColor(sf::Color(0x00, 0x00, 0x00, 0xFF));
			introMessage.setFillColor(sf::Color(0xFF, 0xFF, 0xFF, 0xFF));
			break;
		}
		break;
	// trigger 1 - attacked outside door
	case 1:
		switch (triggerSubIndex) {
		case 0:
			setDialog("You", "Agh! Don't come any closer!");
			spawnEnemy({ 21 * 32.f, 16 * 32.f });
			break;
		case 1:
			setDialog("You", "Now I really need find that evac\nshelter. Before I get myself killed");
			break;
		case 2:
			setDialog("You", "I think it was South-West of here.\nCan't be too far");
			break;
		case 3:
			hideDialog();
			// add next trigger location
			triggers.push_back({ 2, {63 * 32.f, 80 * 32.f, 3 * 32.f, 7 * 32.f} });
			break;
		}
		break;
	// trigger 2 - soldier convo
	case 2:
		switch (triggerSubIndex) {
		case 0:
			frozen = true;
			setDialog("Soldier", "Halt! Only military personnel are\n allowed beyond this point");
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
			// add next trigger with timer
			setTimedTrigger(3, 2.f);
			break;
		}
		break;
	// trigger 3 - mission to go to Yamasaki's house
	case 3:
		switch (triggerSubIndex) {
		case 0:
			setDialog("You", "Well that's not good.\nI better go find Yamasaki");
			break;
		case 1:
			setDialog("You", "If she's still alive...");
			break;
		case 2:
			setDialog("You", "She should be in her house south\n of mine");
			break;
		case 3:
			hideDialog();
			// add next trigger location
			triggers.push_back({ 4, {16 * 32.f, 21 * 32.f, 32.f, 32.f} });
			break;
		}
		break;
	// trigger 4 - convo in Yamasaki's house
	case 4:
		switch (triggerSubIndex) {
		case 0:
			frozen = true;
			setDialog("You", "Yamasaki?");
			// create Yamasaki
			allies.emplace_back(texAllyLeft);
			allies.back().setPosition({ 17 * 32.f, 25 * 32.f });
			allies.back().setMaskBounds({ 8, 0, 15, 32 });
			break;
		case 1:
			setDialog("Yamasaki", "I almost shot you, be careful!");
			break;
		case 2:
			setDialog("You", "The evac shelter turned me away\nWhat's going on?");
			break;
		case 3:
			setDialog("Yamasaki", "I don't know, they sent me away too.\nWho doesn't need a medic!?");
			break;
		case 4:
			setDialog("Yamasaki", "I saw some strange things out there...\nAre you okay?");
			break;
		case 5:
			setDialog("You", "Yeah, but we should leave.\nI don't like what's going on here");
			break;
		case 6:
			frozen = false;
			hideDialog();
			break;
		}
		break;
	default:
		break;
	}
}
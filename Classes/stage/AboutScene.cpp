#include "AboutScene.h"
#include "MenuScene.h"
#include "SceneM.h"
#include "ui/CocosGUI.h"
#include "myAudio/AudioM.h"
using namespace ui;

bool AboutScene::init()
{
	if (!Scene::init()) {
		return false;
	}

	bool isWendy = (((MenuScene *)Director::getInstance()->getRunningScene())->getClickPotato() > 6);

	Sprite * bg = Sprite::create("about/Background.png");
	bg->setAnchorPoint(Vec2(0.5, 0.5));
	bg->setPosition(DESIGN_WIDTH * 0.5, DESIGN_HEIGHT * 0.5);
	bg->setOpacity(80);
	this->addChild(bg);

	if (isWendy) {
		Button * forWendy = Button::create("about/Happy.png", "about/Laugh.png");
		forWendy->setAnchorPoint(Vec2(0.5, 0.5));
		forWendy->setPosition(Vec2(DESIGN_WIDTH * 0.5, DESIGN_HEIGHT * 0.5));
		forWendy->setScale(0.8);
		this->addChild(forWendy);

		AudioM::playBGM("audio/music/Canon");
	} else {
		rule = Sprite::create("about/Rule.png");
		rule->setAnchorPoint(Vec2(0.5, 0.5));
		rule->setPosition(DESIGN_WIDTH * 0.5, DESIGN_HEIGHT * 0.5);
		rule->setScale(0.8);
		this->addChild(rule);
	}

	auto bt = Button::create("newGame/True0.png", "newGame/True1.png");
	bt->setPosition(Vec2(DESIGN_WIDTH * 0.88, DESIGN_HEIGHT * 0.07));
	bt->setScale(0.5);
	bt->addClickEventListener([](Ref * pSender) {
		SceneM::getInstance()->changeScene(stMenuScene, PAGE_TRANSITION_BACKWARD);

		// play sound
		AudioM::playSound("audio/sound/Button");
	});
	this->addChild(bt);

	return true;
}

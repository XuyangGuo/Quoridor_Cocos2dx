#include "GameMenuLayer.h"
#include "data/GlobalData.h"
#include "ui/CocosGUI.h"
#include "MenuScene.h"
#include "SceneM.h"
#include "NewGameLayer.h"
#include "data/GlobalData.h"
#include <string>
#include "myAudio/AudioM.h"
using namespace cocos2d::ui;
using namespace std;

bool GameMenuLayer::init()
{
	if (!Layer::init()) {
		return false;
	}

	// shadow
	shadow = Sprite::create("newGame/Black.png");
	shadow->setPosition(0, 0);
	shadow->setAnchorPoint(Vec2::ZERO);
	shadow->setOpacity(0);
	this->addChild(shadow, 0);

	// board
	board = Sprite::create("newGame/Board.png");
	board->setAnchorPoint(Vec2(0.5, 0.5));
	board->setPosition(DESIGN_WIDTH / 2 + DESIGN_WIDTH, DESIGN_HEIGHT / 2);
	shadow->addChild(board, 1);

	// font
	auto chnStrings = Dictionary::createWithContentsOfFile("text/ItemText.xml");

	// continue game
	auto buttonContinue = Button::create("newGame/Button0.png", "newGame/Button1.png");
	buttonContinue->setTitleText(((String*)chnStrings->objectForKey("ContinueGame"))->getCString());
	buttonContinue->setTitleFontSize(25);
	buttonContinue->setScale(1.5);
	buttonContinue->setTitleFontName("fonts/hanzi.ttf");
	buttonContinue->setPosition(Vec2(DESIGN_WIDTH * 0.5 - BOARD_SHIFT_X, DESIGN_HEIGHT * 0.7 - BOARD_SHIFT_Y));
	buttonContinue->setAnchorPoint(Vec2(0.5, 0.5));
	buttonContinue->addClickEventListener([buttonContinue, this](Ref * sender) {
		disappearAction();

		// play sound
		AudioM::playSound("audio/sound/Button");
	});
	board->addChild(buttonContinue, 2);

	// leave game
	auto buttonLeave = Button::create("newGame/Button0.png", "newGame/Button1.png");
	buttonLeave->setTitleText(((String*)chnStrings->objectForKey("LeaveGame"))->getCString());
	buttonLeave->setTitleFontSize(25);
	buttonLeave->setScale(1.5);
	buttonLeave->setTitleFontName("fonts/hanzi.ttf");
	buttonLeave->setPosition(Vec2(DESIGN_WIDTH * 0.5 - BOARD_SHIFT_X, DESIGN_HEIGHT * 0.6 - BOARD_SHIFT_Y));
	buttonLeave->setAnchorPoint(Vec2(0.5, 0.5));
	buttonLeave->addClickEventListener([](Ref * sender) {
		SceneM::getInstance()->changeScene(stMenuScene, TRANSITION_MOVE_IN_L);

		// play sound
		AudioM::playSound("audio/sound/Button");
	});
	board->addChild(buttonLeave, 2);

	// new game
	auto buttonNew = Button::create("newGame/Button0.png", "newGame/Button1.png");
	buttonNew->setTitleText(((String*)chnStrings->objectForKey("NewGame"))->getCString());
	buttonNew->setTitleFontSize(25);
	buttonNew->setScale(1.5);
	buttonNew->setTitleFontName("fonts/hanzi.ttf");
	buttonNew->setPosition(Vec2(DESIGN_WIDTH * 0.5 - BOARD_SHIFT_X, DESIGN_HEIGHT * 0.5 - BOARD_SHIFT_Y));
	buttonNew->setAnchorPoint(Vec2(0.5, 0.5));
	buttonNew->addClickEventListener([](Ref * sender) {
		auto scene = Director::getInstance()->getRunningScene();
		scene->removeChildByName("gameMenuLayer");
		scene->addChild(NewGameLayer::create(), 15, "newGameLayer");

		// play sound
		AudioM::playSound("audio/sound/Button");
	});
	board->addChild(buttonNew, 2);

	// bgm
	auto buttonBGM = Button::create("newGame/Button0.png", "newGame/Button1.png");


	auto bgmOn = ((String *)chnStrings->objectForKey("BGM1"))->_string;
	auto bgmOff = ((String *)chnStrings->objectForKey("BGM0"))->_string;

	if (AudioM::hasBgm()) {
		buttonBGM->setTitleText(bgmOn);
	} else {
		buttonBGM->setTitleText(bgmOff);
	}

	buttonBGM->setTitleFontSize(25);
	buttonBGM->setScale(1.5);
	buttonBGM->setTitleFontName("fonts/hanzi.ttf");
	buttonBGM->setPosition(Vec2(DESIGN_WIDTH * 0.5 - BOARD_SHIFT_X, DESIGN_HEIGHT * 0.4 - BOARD_SHIFT_Y));
	buttonBGM->setAnchorPoint(Vec2(0.5, 0.5));
	board->addChild(buttonBGM, 2);
	buttonBGM->addClickEventListener([bgmOn, bgmOff, buttonBGM](Ref * pSender){
		if (AudioM::hasBgm()) {
			buttonBGM->setTitleText(bgmOff);
			AudioM::turnBgm(false);
		} else {
			buttonBGM->setTitleText(bgmOn);
			AudioM::turnBgm(true);
		}

		// play sound
		AudioM::playSound("audio/sound/Button");
	});

	// sound
	auto buttonSound = Button::create("newGame/Button0.png", "newGame/Button1.png");

	auto soundOn = ((String *)chnStrings->objectForKey("Sound1"))->_string;
	auto soundOff = ((String *)chnStrings->objectForKey("Sound0"))->_string;
	
	if (AudioM::hasSound()) {
		buttonSound->setTitleText(soundOn);
	} else {
		buttonSound->setTitleText(soundOff);
	}

	buttonSound->setTitleFontSize(25);
	buttonSound->setScale(1.5);
	buttonSound->setTitleFontName("fonts/hanzi.ttf");
	buttonSound->setPosition(Vec2(DESIGN_WIDTH * 0.5 - BOARD_SHIFT_X, DESIGN_HEIGHT * 0.3 - BOARD_SHIFT_Y));
	buttonSound->setAnchorPoint(Vec2(0.5, 0.5));
	buttonSound->addClickEventListener([buttonSound, soundOn, soundOff](Ref * sender) {
		if (AudioM::hasSound()) {
			buttonSound->setTitleText(soundOff);
			AudioM::turnSound(false);
		} else {
			buttonSound->setTitleText(soundOn);
			AudioM::turnSound(true);
		}
	});
	board->addChild(buttonSound, 2);


	// SwallowTouches by shadow
	auto listenerShadow = EventListenerTouchOneByOne::create();
	listenerShadow->onTouchBegan = [](Touch* touch, Event * event) {
		auto location = touch->getLocation();
		//log("touch at (%f, %f) !!!", location.x, location.y);
		return true;
	};
	listenerShadow->setSwallowTouches(true);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerShadow, this);

	appearAction();
    return true;
}

void GameMenuLayer::appearAction()
{
	// add action£¡
	auto fadeIn = FadeIn::create(0.5f);
	auto moveTo = MoveBy::create(1.0f, Vec2(-720, 0));
	auto moveEaseIn = EaseBackOut::create(moveTo->clone());

	auto shadowSeq = Sequence::create(fadeIn, nullptr);

	shadow->runAction(shadowSeq);
	board->runAction(moveEaseIn);
}

// same as the NewGameLayer::disappearAction
void GameMenuLayer::disappearAction()
{
	// add action£¡
	auto fadeOut = FadeOut::create(1.2f);
	auto moveTo = MoveBy::create(1.0f, Vec2(720, 0));
	auto moveEaseIn = EaseBackIn::create(moveTo->clone());

	auto cancleCallBack = CallFunc::create([this]() {
		this->removeFromParent();
	});

	auto shadowSeq = Sequence::create(fadeOut, cancleCallBack, nullptr);

	shadow->runAction(shadowSeq);
	board->runAction(moveEaseIn);
}

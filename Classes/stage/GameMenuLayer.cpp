#include "GameMenuLayer.h"
#include "data/GlobalData.h"
#include "ui/CocosGUI.h"
#include "MenuScene.h"
#include "SceneM.h"
#include "NewGameLayer.h"
#include "data/GlobalData.h"
using namespace cocos2d::ui;

bool GameMenuLayer::init()
{
	if (!Layer::init()) {
		return false;
	}

	// shadow
	auto shadow = Sprite::create("newGame/Black.png");
	shadow->setPosition(0, 0);
	shadow->setAnchorPoint(Vec2::ZERO);
	shadow->setOpacity(OPACITY);
	this->addChild(shadow, 0);

	// board
	auto board = Sprite::create("newGame/Board.png");
	board->setAnchorPoint(Vec2(0.5, 0.5));
	board->setPosition(DESIGN_WIDTH / 2, DESIGN_HEIGHT / 2);
	board->setScale(2);
	this->addChild(board, 1);

	// font
	auto chnStrings = Dictionary::createWithContentsOfFile("text/ItemText.xml");

	// continue game
	auto buttonContinue = Button::create("newGame/Button0.png", "newGame/Button1.png");
	buttonContinue->setTitleText(((String*)chnStrings->objectForKey("ContinueGame"))->getCString());
	buttonContinue->setTitleFontSize(25);
	buttonContinue->setScale(1.5);
	buttonContinue->setTitleFontName("fonts/hanzi.ttf");
	buttonContinue->setPosition(Vec2(DESIGN_WIDTH * 0.5, DESIGN_HEIGHT * 0.7));
	buttonContinue->setAnchorPoint(Vec2(0.5, 0.5));
	buttonContinue->addClickEventListener([buttonContinue](Ref * sender) {
		Director::getInstance()->getRunningScene()->removeChildByName("gameMenuLayer");
	});
	this->addChild(buttonContinue, 2);

	// leave game
	auto buttonLeave = Button::create("newGame/Button0.png", "newGame/Button1.png");
	buttonLeave->setTitleText(((String*)chnStrings->objectForKey("LeaveGame"))->getCString());
	buttonLeave->setTitleFontSize(25);
	buttonLeave->setScale(1.5);
	buttonLeave->setTitleFontName("fonts/hanzi.ttf");
	buttonLeave->setPosition(Vec2(DESIGN_WIDTH * 0.5, DESIGN_HEIGHT * 0.6));
	buttonLeave->setAnchorPoint(Vec2(0.5, 0.5));
	buttonLeave->addClickEventListener([](Ref * sender) {
		SceneM::getInstance()->changeScene(stMenuScene);
	});
	this->addChild(buttonLeave, 2);

	// new game
	auto buttonNew = Button::create("newGame/Button0.png", "newGame/Button1.png");
	buttonNew->setTitleText(((String*)chnStrings->objectForKey("NewGame"))->getCString());
	buttonNew->setTitleFontSize(25);
	buttonNew->setScale(1.5);
	buttonNew->setTitleFontName("fonts/hanzi.ttf");
	buttonNew->setPosition(Vec2(DESIGN_WIDTH * 0.5, DESIGN_HEIGHT * 0.5));
	buttonNew->setAnchorPoint(Vec2(0.5, 0.5));
	buttonNew->addClickEventListener([](Ref * sender) {
		auto scene = Director::getInstance()->getRunningScene();
		scene->removeChildByName("gameMenuLayer");
		scene->addChild(NewGameLayer::create(), 5, "newGameLayer");
	});
	this->addChild(buttonNew, 2);

	// bgm
	auto buttonBGM = Button::create("newGame/Button0.png", "newGame/Button1.png");
	buttonBGM->setTitleText(((String*)chnStrings->objectForKey("BGM0"))->getCString());
	buttonBGM->setTitleFontSize(25);
	buttonBGM->setScale(1.5);
	buttonBGM->setTitleFontName("fonts/hanzi.ttf");
	buttonBGM->setPosition(Vec2(DESIGN_WIDTH * 0.5, DESIGN_HEIGHT * 0.4));
	buttonBGM->setAnchorPoint(Vec2(0.5, 0.5));
	buttonBGM->addClickEventListener([](Ref * sender) {	});
	this->addChild(buttonBGM, 2);

	// sound
	auto buttonSound = Button::create("newGame/Button0.png", "newGame/Button1.png");
	buttonSound->setTitleText(((String*)chnStrings->objectForKey("Sound0"))->getCString());
	buttonSound->setTitleFontSize(25);
	buttonSound->setScale(1.5);
	buttonSound->setTitleFontName("fonts/hanzi.ttf");
	buttonSound->setPosition(Vec2(DESIGN_WIDTH * 0.5, DESIGN_HEIGHT * 0.3));
	buttonSound->setAnchorPoint(Vec2(0.5, 0.5));
	buttonSound->addClickEventListener([](Ref * sender) {});
	this->addChild(buttonSound, 2);


	// SwallowTouches by shadow
	auto listenerShadow = EventListenerTouchOneByOne::create();
	listenerShadow->onTouchBegan = [](Touch* touch, Event * event) {
		auto location = touch->getLocation();
		//log("touch at (%f, %f) !!!", location.x, location.y);
		return true;
	};
	listenerShadow->setSwallowTouches(true);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerShadow, this);
    return true;
}

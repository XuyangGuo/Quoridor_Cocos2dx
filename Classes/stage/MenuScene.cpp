#include "MenuScene.h"
#include "data/GlobalData.h"
#include "SceneM.h"
#include "ui/CocosGUI.h"
#include "myAudio/AudioM.h"
using namespace ui;

bool MenuScene::init()
{
	if (!Scene::init()) {
		return false;
	}

	// background
	auto background = Sprite::create("menu/Background.png");
	background->setPosition(DESIGN_WIDTH / 2, DESIGN_HEIGHT / 2);
	this->addChild(background);

	// menu
	auto beginItem = MenuItemImage::create("menu/Begin.png", "menu/Begin2.png", [](cocos2d::Ref * pSender) {
		SceneM::getInstance()->changeScene(stGameScene, TRANSITION_FADE); 

		// play sound
		AudioM::playSound("audio/sound/Button");
	});
	beginItem->setPosition(DESIGN_WIDTH * 0.5, DESIGN_HEIGHT * 0.48);

	auto aboutItem = MenuItemImage::create("menu/About.png", "menu/About2.png", [](cocos2d::Ref * pSender) {
		SceneM::getInstance()->changeScene(stAboutScene, PAGE_TRANSITION_FORWARD); 

		// play sound
		AudioM::playSound("audio/sound/Button");
	});
	aboutItem->setPosition(DESIGN_WIDTH * 0.5, DESIGN_HEIGHT * 0.4);

	auto quitItem = MenuItemImage::create("menu/Quit.png", "menu/Quit2.png", [](cocos2d::Ref * pSender) { 
		Director::getInstance()->end();

		// play sound
		AudioM::playSound("audio/sound/Button");
	});
	quitItem->setPosition(DESIGN_WIDTH * 0.497, DESIGN_HEIGHT * 0.32);

	// title
	auto title = Sprite::create("menu/Title.png");
	title->setPosition(DESIGN_WIDTH / 2, DESIGN_HEIGHT * 0.7);
	title->setScale(0.7);
	this->addChild(title);

	ClickPotato = 0;
	auto word1 = Button::create("menu/Word1.png", "menu/Word1.png");
	word1->setPosition(Vec2(DESIGN_WIDTH * 0.16, DESIGN_HEIGHT * 0.165));
	word1->setRotation(-22);
	word1->setScale(0.16);
	word1->addClickEventListener([this](Ref * pSender) {
		if (++ClickPotato == 7) {
			auto word2 = Sprite::create("menu/Word2.png");
			word2->setPosition(DESIGN_WIDTH * 0.9, DESIGN_HEIGHT * 0.02);
			word2->setScale(0.2);
			this->addChild(word2);
		}
	});
	this->addChild(word1);

	// add music
	AudioM::playBGM("audio/music/XingGuang");
	//AudioM::playBGM("canon.mp3");
/*
	Button * spAudio = Button::create();
	spAudio.texture
*/
	menu_selector(MenuScene::togglemenuCallBack);

	toggleSound = CCMenuItemToggle::createWithTarget(this, menu_selector(MenuScene::togglemenuCallBack),
		CCMenuItemImage::create("menu/Sound0.png", "menu/Sound0.png"),
		CCMenuItemImage::create("menu/Sound1.png", "menu/Sound1.png"),
		NULL);

	toggleSound->setSelectedIndex(AudioM::hasBgm() || AudioM::hasSound());
	toggleSound->setPosition(0.9 * DESIGN_WIDTH, 0.92 * DESIGN_HEIGHT);

	auto menu = Menu::create(beginItem, aboutItem, quitItem, toggleSound, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	return true;
}

void MenuScene::togglemenuCallBack(CCObject * pSender)
{
	AudioM::turnBgm(toggleSound->getSelectedIndex());
	AudioM::turnSound(toggleSound->getSelectedIndex());
}



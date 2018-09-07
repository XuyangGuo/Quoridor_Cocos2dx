#include "MenuScene.h"
#include "data/GlobalData.h"
#include "SceneM.h"

bool MenuScene::init()
{
	if (!Scene::init()) {
		return false;
	}

	// background
	auto background = Sprite::create("menu/Background.jpg");
	background->setPosition(DESIGN_WIDTH / 2, DESIGN_HEIGHT / 2);
	this->addChild(background);

	// menu
	auto beginItem = MenuItemImage::create("menu/Begin.png", "menu/Begin2.png",
		[](cocos2d::Ref * pSender) { SceneM::getInstance()->changeScene(stGameScene); });
	beginItem->setPosition(DESIGN_WIDTH / 2, DESIGN_HEIGHT * 3/ 4);

	auto aboutItem = MenuItemImage::create("menu/About.png", "menu/About2.png");
	aboutItem->setPosition(DESIGN_WIDTH / 2, DESIGN_HEIGHT * 2 / 4);

	auto quitItem = MenuItemImage::create("menu/Quit.png", "menu/Quit2.png", 
		[](cocos2d::Ref * pSender) { Director::getInstance()->end(); });
	quitItem->setPosition(DESIGN_WIDTH / 2, DESIGN_HEIGHT * 1 / 4);

	auto menu = Menu::create(beginItem, aboutItem, quitItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	return true;
}



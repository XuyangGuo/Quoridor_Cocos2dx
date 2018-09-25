#pragma once
#include "cocos2d.h"
USING_NS_CC;

class MenuScene : public Scene {
public:
	CREATE_FUNC(MenuScene);
	virtual bool init();
	int getClickPotato() { return ClickPotato; }

	void togglemenuCallBack(CCObject* pSender);

private:
	int ClickPotato;
	CCMenuItemToggle * toggleSound;
};

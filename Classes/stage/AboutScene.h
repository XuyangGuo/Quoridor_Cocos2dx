#pragma once
#include "cocos2d.h"
#include "data/GlobalData.h"
USING_NS_CC;

class AboutScene : public Scene {
public:
	CREATE_FUNC(AboutScene);
	virtual bool init();

private:
	Sprite * rule;
};
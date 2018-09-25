#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <string>

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

class NewGameLayer : public Layer {
public:
	virtual bool init();
	// void buttonGilClickEvent(Ref * sender);
	CREATE_FUNC(NewGameLayer);
	
private:
	bool isGilAI;
	bool isWendyAI;
	int blockNum;
	int difficulty; // 0 easy 1 hard

	Sprite * shadow;
	Sprite * board;

	void disappearAction();
};
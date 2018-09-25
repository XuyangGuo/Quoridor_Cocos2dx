#pragma once
#include "cocos2d.h"
#include "data/GlobalData.h"
USING_NS_CC;

class WinLayer : public Layer {
public:
	CREATE_FUNC(WinLayer);
	virtual bool init();
	void setWinner(bool isWendy);

private:
	void appearAction();
	void disappearAction();

	Sprite * shadow;
	Sprite * board;
};
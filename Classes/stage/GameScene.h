#pragma once
#include "cocos2d.h"
#include "game/ChessState.h"
#include <utility>
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace std;
using namespace cocos2d::ui;

enum TouchObj {
	toNULL,
	toBLOCK,
	toPLAYER,
	toPATH
};

enum TouchState {
	tsNULL,
	tsPLAYER,
	tsBLOCK,
};

class GameScene : public Scene {
public:
	CREATE_FUNC(GameScene);
	virtual bool init();
	void initialGame(bool isGilAI, bool isWendyAI, int difficulty, int blockNum);
	void onExit();
	

private:
	ChessState  * chessState;
	//pair<int, int> player[2];
	int curPlayer;
	cocos2d::Label * labelStep;
	TouchObj to;
	TouchState ts;

	// cur touch block
	pair<int, int> curTouchBlock;
	Sprite * curTouchSprite;
	const int curTouchOpacity = 120;
	const int choiceOpacity = 70;

	vector<pair<int, int> > blockChoice;

	cocos2d::Sprite * gameLayer;
	cocos2d::Sprite * spPlayer[2];
	cocos2d::Sprite * blocks[2];
	cocos2d::Sprite * buttonPlayers[2];
	cocos2d::ui::Button * buttonRegret;

	Vec2 chess2design(int i, int j);
	pair<int, int> design2chess(Vec2 location);
	void chooseBlock(bool refresh);
	void changeTS(TouchState cur_ts);
	bool blockInChoice(int i, int j);
};

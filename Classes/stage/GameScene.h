#pragma once
#include "cocos2d.h"
#include "game/ChessState.h"
#include <utility>
#include "ui/CocosGUI.h"
#include <ai/MyAI.h>
USING_NS_CC;
using namespace std;
using namespace cocos2d::ui;

enum TouchObj {
	toNULL,
	toBLOCK,
	toPLAYER,
	toPATH,
	toBUILD
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
	// chess state and ai
	ChessState  * chessState;
	MyAI * myAI;

	//pair<int, int> player[2];
	int curPlayer;
	cocos2d::Label * labelStep;
	bool isPlayer1AI;
	bool isPlayer0Wendy;

	// touch
	bool isEnd;
	TouchObj to;
	TouchState ts;

	// cur touch block
	pair<int, int> curTouchBlock;
	Sprite * curTouchSprite;
	const int curTouchOpacity = 120;
	const int choiceOpacity = 70;
	vector<pair<int, int> > blockChoice;
	bool blockInChoice(int i, int j);
	void chooseBlock(bool refresh);

	// cur build plank
	const int plankTagOffset = 200;
	pair<int, int> startTouchGrid;
	Vec2 startTouchPos;
	vector<Direction> buildDir;
	bool moveBuild(Vec2 & location, bool build=false);
	bool findInVector(vector<Direction> & v, Direction dir);
	cocos2d::ui::LoadingBar * buildBar;

	void changeTS(TouchState cur_ts);
	void inTurn(bool regret);
	void executeOp(Op op);

	cocos2d::Sprite * gameLayer;
	cocos2d::Sprite * spPlayer[2];
	cocos2d::Sprite * blocks[2];
	cocos2d::Sprite * buttonPlayers[2];
	cocos2d::ui::Button * buttonRegret;

	Vec2 chess2design(int i, int j);
	pair<int, int> design2chess(Vec2 location);
	pair<int, int> design2grid(Vec2 location);
	Vec2 grid2design(int i, int j);

	void refreshLabel(bool remove = true);

	// regret one step
	void regretOneStep();

	// add plank icon
	void addPlankIcon(int i, int j, int p);
};

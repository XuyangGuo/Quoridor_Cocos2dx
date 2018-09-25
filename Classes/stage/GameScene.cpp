#include "GameScene.h"
#include "data/GlobalData.h"
#include "NewGameLayer.h"
#include "GameMenuLayer.h"
#include "WinLayer.h"
#include "myAudio/AudioM.h"

bool GameScene::init()
{
	if (!Scene::init()) {
		return false;
	}

	// initial variable
	this->chessState = nullptr;
	this->gameLayer = nullptr;
	this->myAI = nullptr;

	// background
	auto background = Sprite::create("game/Background.png");
	background->setPosition(DESIGN_WIDTH / 2, DESIGN_HEIGHT / 2);
	this->addChild(background, 0);

	// chess block
	for (int i = 0; i < 9; ++i) {
		for (int j = 0; j < 9; ++j) {
			auto block = Sprite::create("game/Block.png");
			//block->setScale(2.2);

			block->setPosition(chess2design(i, j));
			this->addChild(block);
		}
	}

	// menu
	auto buttonMenu = Button::create("game/Menu0.png", "game/Menu1.png", "game/Menu0.png");
	// buttonMenu->setAnchorPoint(Vec2::ZERO);
	buttonMenu->setPosition(Vec2(DESIGN_WIDTH - 80, 75));
	buttonMenu->setScale(1.2);
	buttonMenu->addClickEventListener([this](Ref* pSender) {
		auto gameMenuLayer = GameMenuLayer::create();
		this->addChild(gameMenuLayer, 15, "gameMenuLayer");

		// play sound
		AudioM::playSound("audio/sound/Button");
	});
	this->addChild(buttonMenu);

	auto newGameLayer = NewGameLayer::create();
	this->addChild(newGameLayer, 15, "newGameLayer");

	return true;
}

void GameScene::initialGame(bool isGilAI, bool isWendyAI, int difficulty, int blockNum)
{
	if (chessState) {
		delete chessState;
		delete myAI;
	}
	chessState = new ChessState(blockNum);
	myAI = new MyAI(chessState, difficulty);

	if (gameLayer) {
		gameLayer->removeFromParent();
	}
	gameLayer = Sprite::create();
	this->addChild(gameLayer, 5);
	this->setPosition(Vec2::ZERO);
	
	char p_gil[50] = "game/Gil.png", p_wendy[50] = "game/Wendy.png";
	char * p_player[2];
	if (isWendyAI) {
		p_player[1] = p_wendy;
		p_player[0] = p_gil;
		curPlayer = 1;
	} else {
		p_player[0] = p_wendy;
		p_player[1] = p_gil;
		curPlayer = 0;
	}

	// add sprite
	// chessman
	for (int i = 0; i < 2; ++i) {
		spPlayer[i] = Sprite::create(p_player[i]);
		spPlayer[i]->setScale(0.17);
		gameLayer->addChild(spPlayer[i], 15);
	}
	spPlayer[0]->setPosition(chess2design(0, 4));
	spPlayer[1]->setPosition(chess2design(8, 4));

	// plank, X 75µ½420, Y 170µ½300
	for (int i = 0; i < 2; ++i) {
		blocks[i] = Sprite::create();
		blocks[i]->setTexture("game/Shadow.png");
		blocks[i]->setScale(1.2);
		gameLayer->addChild(blocks[i], 1);
	}
	blocks[0]->setPosition(DESIGN_WIDTH / 2 - 110, CHESS_HEIGHT_CENTER - 465);
	blocks[1]->setPosition(DESIGN_WIDTH / 2 + 110, CHESS_HEIGHT_CENTER + 465);
	blocks[1]->setRotation(180);

	for (int k = 0; k < 2; ++k) {
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 5; ++j) {
				if (i * 5 + j < blockNum) {
					addPlankIcon(i, j, k);
				}
			}
		}
	}

	// chess man sprite button
	for (int i = 0; i < 2; ++i) {
		buttonPlayers[i] = Sprite::create(p_player[i]);
		buttonPlayers[i]->setScale(0.3);
		gameLayer->addChild(buttonPlayers[i], 1);
	}
	buttonPlayers[0]->setPosition(Vec2(0.80 * DESIGN_WIDTH, CHESS_HEIGHT_CENTER - 460));
	buttonPlayers[1]->setPosition(Vec2(0.20 * DESIGN_WIDTH, CHESS_HEIGHT_CENTER + 460));

	if (!isWendyAI && !isGilAI) {
		spPlayer[1]->setRotation(180);
		buttonPlayers[1]->setRotation(180);
	}

	// is player1 ai
	isPlayer1AI = isGilAI || isWendyAI;
	isPlayer0Wendy = !isWendyAI;

	// step count
	refreshLabel(false);

	buttonRegret = Button::create("game/Regret0.png", "game/Regret1.png", "game/Regret2.png");
	// buttonRegret->setAnchorPoint(Vec2::ZERO);
	buttonRegret->setPosition(Vec2(0.65 * DESIGN_WIDTH, 80));
	buttonRegret->setScale(1.4);
	buttonRegret->setEnabled(false);
	buttonRegret->addClickEventListener([this](Ref * pSender) {
		regretOneStep();
		if (isPlayer1AI && curPlayer != 0) {
			regretOneStep();
		}
		buttonRegret->setEnabled(chessState->canRegret());

		// add sound
		AudioM::playSound("audio/sound/Choose");
	});
	gameLayer->addChild(buttonRegret, 1);

	// torch event
	this->ts = tsNULL;
	this->isEnd = false;

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [this](Touch* touch, Event * event) {
		auto location = touch->getLocation();
		//CCLOG("touch begin at game scene (%f, %f) !!!", location.x, location.y);

		this->to = toNULL;
		
		if (getSpriteRect(blocks[curPlayer]).containsPoint(location) && !isEnd) {
			if (chessState->getRemain(curPlayer) > 0) {
				this->to = toBLOCK;
				this->blocks[curPlayer]->setScale(1.3);
			}
		} else if (getSpriteRect(buttonPlayers[curPlayer]).containsPoint(location) && !isEnd) {
			this->to = toPLAYER;
			this->buttonPlayers[curPlayer]->setScale(0.33);
		} else if (this->ts == tsPLAYER) {
			auto p = this->design2chess(location);
			if (blockInChoice(p.first, p.second) && getSpriteRect((Sprite *)gameLayer->getChildByTag(p.first * 9 + p.second)).containsPoint(location)) {
				curTouchBlock = p;
				curTouchSprite = (Sprite *)gameLayer->getChildByTag(p.first * 9 + p.second);
				curTouchSprite->setOpacity(curTouchOpacity);
				this->to = toPATH;
			}
		} else if (this->ts == tsBLOCK) {
			auto p = this->design2grid(location);
			buildDir = chessState->getCanBuild(p.first, p.second);
			if (!buildDir.empty()) {
				this->to = toBUILD;
				startTouchGrid = p;
				startTouchPos = grid2design(p.first, p.second);
				// CCLOG("%f %f", startTouchPos.x, startTouchPos.y);

				buildBar = LoadingBar::create("game/Plank.png");
				buildBar->setPercent(0);
				gameLayer->addChild(buildBar, 10);
			}
		}

		return true;
	};

	listener->onTouchMoved = [this](Touch* touch, Event * event) {
		auto location = touch->getLocation();
		//CCLOG("touch move at game scene (%f, %f) !!!", location.x, location.y);

		if (toBLOCK == this->to) {
			if (!getSpriteRect(blocks[curPlayer]).containsPoint(location)) {
				this->blocks[curPlayer]->setScale(1.2);
			} else {
				this->blocks[curPlayer]->setScale(1.3);
			}
		} else if (toPLAYER == this->to) {
			if (getSpriteRect(buttonPlayers[curPlayer]).containsPoint(location)) {
				this->buttonPlayers[curPlayer]->setScale(0.33);
			} else {
				this->buttonPlayers[curPlayer]->setScale(0.3);
			}
		} else if (toPATH == this->to) {
			if (getSpriteRect(curTouchSprite).containsPoint(location)) {
				curTouchSprite->setOpacity(curTouchOpacity);
			} else {
				curTouchSprite->setOpacity(choiceOpacity);
			}
		} else if (toBUILD == this->to) {
			moveBuild(location, false);
		}
		
		return true;
	};

	listener->onTouchEnded = [this](Touch* touch, Event * event) {
		auto location = touch->getLocation();
		//CCLOG("touch end at game scene (%f, %f) !!!", location.x, location.y);

		if (toBLOCK == this->to) {
			if (getSpriteRect(blocks[curPlayer]).containsPoint(location)) {
				this->blocks[curPlayer]->setScale(1.2);
				if (tsBLOCK != this->ts) {
					changeTS(tsBLOCK);
				} else {
					changeTS(tsNULL);
				}

				// add sound
				AudioM::playSound("audio/sound/Choose");
			}
		} else if (toPLAYER == this->to) {
			if (getSpriteRect(buttonPlayers[curPlayer]).containsPoint(location)) {
				this->buttonPlayers[curPlayer]->setScale(0.3);
				if (tsPLAYER != this->ts) {
					changeTS(tsPLAYER);
				} else {
					changeTS(tsNULL);
				}

				// add sound
				AudioM::playSound("audio/sound/Choose");
			}
		} else if (toPATH == this->to) {
			if (getSpriteRect(curTouchSprite).containsPoint(location)) {
				executeOp(Op(EMPTY, curTouchBlock));
			}
		} else if (toBUILD == this->to) {
			moveBuild(location, true);
		}
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	// if ai first, play a step
		// ai play chess
	if (isPlayer1AI && curPlayer == 1) {
		auto delay = DelayTime::create(0.3f);
		auto callbackGetAI = CallFunc::create([this]() {
			Op ai_op = myAI->getAIOp();
			executeOp(ai_op);
		});

		auto seq = Sequence::create(delay, callbackGetAI, nullptr);
		spPlayer[curPlayer]->runAction(seq);
	}
}

void GameScene::onExit()
{
	if (chessState) {
		delete chessState;
	}
	Scene::onExit();
}

Vec2 GameScene::chess2design(int i, int j)
{
	return Vec2(DESIGN_WIDTH / 2 + (j - 4) * CHESS_STEP, CHESS_HEIGHT_CENTER + (i - 4) * CHESS_STEP);
}

pair<int, int> GameScene::design2chess(Vec2 location)
{
	return pair<int, int> (floor((location.y - CHESS_HEIGHT_CENTER) / CHESS_STEP + 4.5),
		floor((location.x - DESIGN_WIDTH / 2) / CHESS_STEP + 4.5));
}

pair<int, int> GameScene::design2grid(Vec2 location)
{
	return pair<int, int>(floor((location.y - CHESS_HEIGHT_CENTER) / CHESS_STEP) + 4,
		floor((location.x - DESIGN_WIDTH / 2) / CHESS_STEP + 4));
}

Vec2 GameScene::grid2design(int i, int j)
{
	return Vec2(DESIGN_WIDTH / 2 + (j - 3.5) * CHESS_STEP, CHESS_HEIGHT_CENTER + (i - 3.5) * CHESS_STEP);
}

void GameScene::chooseBlock(bool refresh)
{
	if (refresh) {
		blockChoice.clear();
		chessState->getReachable(curPlayer, blockChoice);
	}

	for (int k = 0; k < blockChoice.size(); ++k) {
		auto curPos = this->blockChoice[k];
		int i = curPos.first, j = curPos.second;
		if (refresh) {
			auto sp = Sprite::create("game/BlockShadow.png");
			sp->setOpacity(choiceOpacity);
			
			sp->setPosition(chess2design(i, j));
			gameLayer->addChild(sp, 5, i * 9 + j);
		} else {
			gameLayer->removeChildByTag(i * 9 + j);
		}
	}
}

bool GameScene::moveBuild(Vec2 & location, bool build)
{
	Direction hDir, vDir, resDir = NO_DIR;
	if (location.x > startTouchPos.x) {
		hDir = RIGHT;
	} else {
		hDir = LEFT;
	}

	if (location.y > startTouchPos.y) {
		vDir = UP;
	} else {
		vDir = DOWN;
	}

	if (abs(location.x - startTouchPos.x) > abs(location.y - startTouchPos.y)) {
		if (findInVector(buildDir, hDir)) {
			resDir = hDir; 
		} else if (findInVector(buildDir, vDir)) {
			resDir = vDir;
		}
	} else {
		if (findInVector(buildDir, vDir)) {
			resDir = vDir;
		} else if (findInVector(buildDir, hDir)) {
			resDir = hDir;
		}
	}

	float buildPercent = 0;
	pair<int, int> buildGrid;
	if (resDir == UP) {
		buildPercent = double((location.y - startTouchPos.y)) * 50 / CHESS_STEP;
		buildGrid = pair<int, int>(startTouchGrid.first + 1, startTouchGrid.second);
		buildBar->setRotation(90);
		buildBar->setDirection(ui::LoadingBar::Direction::RIGHT);
	} else if (resDir == DOWN) {
		buildPercent = double((startTouchPos.y - location.y)) * 50 / CHESS_STEP;
		buildGrid = pair<int, int>(startTouchGrid.first - 1, startTouchGrid.second);
		buildBar->setRotation(90);
		buildBar->setDirection(ui::LoadingBar::Direction::LEFT);
	} else if (resDir == LEFT) {
		buildPercent = double((startTouchPos.x - location.x)) * 50 / CHESS_STEP;
		buildGrid = pair<int, int>(startTouchGrid.first, startTouchGrid.second - 1);
		buildBar->setRotation(0);
		buildBar->setDirection(ui::LoadingBar::Direction::RIGHT);
	} else if (resDir == RIGHT) {
		buildPercent = double((location.x - startTouchPos.x)) * 50 / CHESS_STEP;
		buildGrid = pair<int, int>(startTouchGrid.first, startTouchGrid.second + 1);
		buildBar->setRotation(0);
		buildBar->setDirection(ui::LoadingBar::Direction::LEFT);
	}

	if (resDir != NO_DIR) {
		buildBar->setPosition(grid2design(buildGrid.first, buildGrid.second));
	}

	buildBar->setPercent(buildPercent);

	if (buildPercent > 80) {
		buildBar->setOpacity(255);
	} else {
		buildBar->setOpacity(120);
	}

	// if build, execute the step
	if (build) {
		if (buildPercent > 80) {
			if (resDir == hDir) {
				executeOp(Op(HORIZONTAL, buildGrid));
			} else {
				executeOp(Op(VERTICAL, buildGrid));
			}
		}

		buildBar->removeFromParent();
	}

	return buildPercent > 80;
}

bool GameScene::findInVector(vector<Direction>& v, Direction dir)
{
	for (int i = 0; i < v.size(); ++i) {
		if (v[i] == dir) {
			return true;
		}
	}
	return false;
}

void GameScene::changeTS(TouchState cur_ts)
{
	if (tsBLOCK == this->ts) {
		setChildrenOpacity(blocks[curPlayer], 255);
	} else if (tsPLAYER == this->ts) {
		buttonPlayers[curPlayer]->setOpacity(255);
		this->chooseBlock(false);
	}

	this->ts = cur_ts;

	if (tsBLOCK == cur_ts) {
		setChildrenOpacity(blocks[curPlayer], CHOOSE_OPACITY);
	} else if (tsPLAYER == cur_ts) {
		buttonPlayers[curPlayer]->setOpacity(CHOOSE_OPACITY);
		this->chooseBlock(true);
	}
}

bool GameScene::blockInChoice(int i, int j)
{
	if (ts == tsPLAYER) {
		for (int k = 0; k < blockChoice.size(); ++k) {
			if (blockChoice[k].first == i && blockChoice[k].second == j) {
				return true;
			}
		}
	}
	return false;
}

void GameScene::inTurn(bool  regret)
{
	changeTS(tsNULL);
	curPlayer ^= 1;
	refreshLabel(true);
	buttonRegret->setEnabled(chessState->canRegret());

	// ai play chess
	if (!isEnd && isPlayer1AI && curPlayer == 1 && !regret) {
		auto delay = DelayTime::create(0.3f);
		auto callbackGetAI = CallFunc::create([this]() {
			Op ai_op = myAI->getAIOp();
			executeOp(ai_op);
		});

		auto seq = Sequence::create(delay, callbackGetAI, nullptr);
		spPlayer[curPlayer]->runAction(seq);
	}
}

void GameScene::executeOp(Op op)
{
	chessState->setStep(op, this->curPlayer);
	// move a chess
	if (op.block == EMPTY) {
		// auto moveTo = MoveTo::create(0.25, chess2design(curTouchBlock.first, curTouchBlock.second));
		auto moveTo = MoveTo::create(0.25, chess2design(op.i, op.j));
		spPlayer[curPlayer]->runAction(moveTo);
		// spPlayer[curPlayer]->setPosition(chess2design(curTouchBlock.first, curTouchBlock.second));

		// add sound
		AudioM::playSound("audio/sound/Move");

		// judge win
		if (curPlayer == 0 && op.i == 8 || curPlayer == 1 && op.i == 0) {
			auto winLayer = WinLayer::create();
			winLayer->setWinner(isPlayer0Wendy ^ curPlayer);
			this->addChild(winLayer,15);
			isEnd = true;
		}
	} else {
		auto spPlank = Sprite::create("game/Plank.png");

		// add sound
		AudioM::playSound("audio/sound/Plank");

		spPlank->setPosition(grid2design(op.i, op.j));
		gameLayer->addChild(spPlank, 10, plankTagOffset + op.i * 9 + op.j);
		if (op.block == VERTICAL) {
			spPlank->setRotation(90);
		}
		blocks[curPlayer]->removeChildByTag(chessState->getRemain(curPlayer));
	}
	inTurn(false);
}

void GameScene::refreshLabel(bool remove)
{
	if (remove) {
		labelStep->removeFromParent();
	}
	char txt[100];
	sprintf(txt, "%d", chessState->getStep());
	labelStep = Label::createWithTTF(txt, "fonts/Marker Felt.ttf", 75);
	labelStep->setPosition(100, 69);
	labelStep->enableBold();
	gameLayer->addChild(labelStep);
}

void GameScene::regretOneStep()
{
	inTurn(true);
	Op op = chessState->setRegret(curPlayer);
	if (op.block == EMPTY) {
		spPlayer[curPlayer]->setPosition(chess2design(op.i, op.j));
	} else {
		gameLayer->removeChildByTag(plankTagOffset + op.i * 9 + op.j);
		int tag = chessState->getRemain(curPlayer) - 1;
		addPlankIcon(tag / 5, tag % 5, curPlayer);
	}

	isEnd = false;
}

void GameScene::addPlankIcon(int i, int j, int p)
{
	auto plank = Sprite::create("game/Plank.png");
	plank->setAnchorPoint(Vec2(0.5, 0.5));
	plank->setPosition(i * 120 + 54, j * 25 + 17);
	plank->setScaleX(0.7);
	blocks[p]->addChild(plank, 1, i * 5 + j);
}


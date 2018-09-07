#include "GameScene.h"
#include "data/GlobalData.h"
#include "NewGameLayer.h"
#include "GameMenuLayer.h"

bool GameScene::init()
{
	if (!Scene::init()) {
		return false;
	}

	// initial variable
	this->chessState = nullptr;
	this->gameLayer = nullptr;

	// background
	auto background = Sprite::create("game/Background.png");
	background->setPosition(DESIGN_WIDTH / 2, DESIGN_HEIGHT / 2);
	this->addChild(background, 0);

	// chess block
	for (int i = 0; i < 9; ++i) {
		for (int j = 0; j < 9; ++j) {
			auto block = Sprite::create("game/Block1.png");
			//block->setScale(2.2);

			block->setPosition(chess2design(i, j));
			this->addChild(block, 1, i * 9 + j);
		}
	}

	// menu
	auto buttonMenu = Button::create("game/Menu0.png", "game/Menu1.png", "game/Menu0.png");
	// buttonMenu->setAnchorPoint(Vec2::ZERO);
	buttonMenu->setPosition(Vec2(DESIGN_WIDTH - 80, 75));
	buttonMenu->setScale(1.2);
	buttonMenu->addClickEventListener([this](Ref* pSender) {
		auto gameMenuLayer = GameMenuLayer::create();
		this->addChild(gameMenuLayer, 5, "gameMenuLayer");
	});
	this->addChild(buttonMenu);

	auto newGameLayer = NewGameLayer::create();
	this->addChild(newGameLayer, 5, "newGameLayer");
	return true;
}

void GameScene::initialGame(bool isGilAI, bool isWendyAI, int difficulty, int blockNum)
{
	if (chessState) {
		delete chessState;
	}
	chessState = new ChessState(blockNum);

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

	//player[0].first = 0;
	//player[1].first = 8;
	//player[0].second = 4;
	//player[1].second = 4;

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
		//blocks[i] = Sprite::create("game/Shadow.png");
		blocks[i] = Sprite::create();
		blocks[i]->setTexture("game/Shadow.png");
		blocks[i]->setScale(1.2);
		//blocks[i]->setAnchorPoint(Vec2::ZERO);
		gameLayer->addChild(blocks[i], 1);
	}
	blocks[0]->setPosition(DESIGN_WIDTH / 2 - 110, CHESS_HEIGHT_CENTER - 465);
	blocks[1]->setPosition(DESIGN_WIDTH / 2 + 110, CHESS_HEIGHT_CENTER + 465);
	blocks[1]->setRotation(180);

	char stringPlank[50] = "game/Plank.png";
	auto scalePlankX = 1.1;
	auto scalePlankY = 1.2;

	for (int k = 0; k < 2; ++k) {
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 5; ++j) {
				if (i * 5 + j < blockNum) {
					auto plank = Sprite::create(stringPlank);
					plank->setAnchorPoint(Vec2(0.5, 0.5));
					plank->setPosition(i * 120 + 54, j * 25 + 17);
					plank->setScaleX(scalePlankX * 0.7);
					plank->setScaleY(scalePlankY);
					blocks[k]->addChild(plank, 1, i * 5 + j);
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

	// CCLOG("%d %d", blocks[0]->getChildrenCount(), blocks[1]->getChildrenCount());

	auto plank1 = Sprite::create(stringPlank);
	plank1->setPosition(DESIGN_WIDTH / 2 - CHESS_STEP * 2.5, CHESS_HEIGHT_CENTER + CHESS_STEP * 2.5);
	plank1->setScaleX(scalePlankX);
	plank1->setScaleY(scalePlankY);
	plank1->setAnchorPoint(Vec2(0.5, 0.5));
	gameLayer->addChild(plank1, 10);

	auto plank2 = Sprite::create(stringPlank);
	plank2->setPosition(DESIGN_WIDTH / 2 + CHESS_STEP * 0.5, CHESS_HEIGHT_CENTER - CHESS_STEP * 3.5);
	plank2->setScaleX(scalePlankX);
	plank2->setScaleY(scalePlankY);
	plank2->setAnchorPoint(Vec2(0.5, 0.5));
	plank2->setRotation(90);
	gameLayer->addChild(plank2, 10);

	// step count
	auto stepCount = Label::createWithTTF("0", "fonts/Marker Felt.ttf", 75);
	// stepCount->setAnchorPoint(Vec2::ZERO);
	stepCount->setPosition(100, 69);
	stepCount->enableBold();
	gameLayer->addChild(stepCount);

	buttonRegret = Button::create("game/Regret0.png", "game/Regret1.png", "game/Regret2.png");
	// buttonRegret->setAnchorPoint(Vec2::ZERO);
	buttonRegret->setPosition(Vec2(0.65 * DESIGN_WIDTH, 80));
	buttonRegret->setScale(1.4);
	buttonRegret->setEnabled(false);
	gameLayer->addChild(buttonRegret, 1);

	// torch event
	this->ts = tsNULL;

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [this](Touch* touch, Event * event) {
		auto location = touch->getLocation();
		//CCLOG("touch begin at game scene (%f, %f) !!!", location.x, location.y);

		this->to = toNULL;
		
		if (getSpriteRect(blocks[curPlayer]).containsPoint(location)) {
			this->to = toBLOCK;
			this->blocks[curPlayer]->setScale(1.3);
		} else if (getSpriteRect(buttonPlayers[curPlayer]).containsPoint(location)) {
			this->to = toPLAYER;
			this->buttonPlayers[curPlayer]->setScale(0.33);
		} else {
			auto p = this->design2chess(location);
			if (blockInChoice(p.first, p.second)) {
				curTouchBlock = p;
				curTouchSprite = (Sprite *)gameLayer->getChildByTag(p.first * 9 + p.second);
				curTouchSprite->setOpacity(curTouchOpacity);
				this->to = toPATH;
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
			}
		} else if (toPLAYER == this->to) {
			if (getSpriteRect(buttonPlayers[curPlayer]).containsPoint(location)) {
				this->buttonPlayers[curPlayer]->setScale(0.3);
				if (tsPLAYER != this->ts) {
					changeTS(tsPLAYER);
				} else {
					changeTS(tsNULL);
				}
			}
		} else if (toPATH == this->to) {
			if (getSpriteRect(curTouchSprite).containsPoint(location)) {
				// move a chess
				curTouchSprite->setOpacity(choiceOpacity);
				chessState->setStep(Op(EMPTY, curTouchBlock), curPlayer);
				//player[curPlayer] = curTouchBlock;
				spPlayer[curPlayer]->setPosition(chess2design(curTouchBlock.first, curTouchBlock.second));
				changeTS(tsNULL);
				curPlayer ^= 1;
			}
		}
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
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


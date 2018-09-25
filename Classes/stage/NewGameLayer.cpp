#include "NewGameLayer.h"
#include "data/GlobalData.h"
#include "SceneM.h"
#include "GameScene.h"
#include <utility>
#include "myAudio/AudioM.h"


bool NewGameLayer::init()
{
	if (!Layer::init()) {
		return false;
	}

	// shadow
	shadow = Sprite::create("newGame/Black.png");
	shadow->setPosition(0, 0);
	shadow->setAnchorPoint(Vec2::ZERO);
	this->addChild(shadow, 0);

	// board
	board = Sprite::create("newGame/Board.png");
	board->setAnchorPoint(Vec2(0.5, 0.5));
	board->setPosition(DESIGN_WIDTH / 2, DESIGN_HEIGHT / 2);
	auto a = board->getContentSize();
	// CCLOG("%f %f", 360 - a.width / 2, 640 - a.height / 2);
	shadow->addChild(board, 1);

	// gil
	auto gil = Sprite::create("newGame/Gil1.png");
	gil->setPosition(DESIGN_WIDTH * 0.35 - BOARD_SHIFT_X, DESIGN_HEIGHT * 0.6 - BOARD_SHIFT_Y);
	gil->setScale(0.25);
	gil->setAnchorPoint(Vec2(0.5, 0.5));
	board->addChild(gil, 2);

	// font
	auto chnStrings = Dictionary::createWithContentsOfFile("text/ItemText.xml");
	auto s_computer = ((String*)chnStrings->objectForKey("AI"))->_string;
	auto s_gilPlayer = ((String*)chnStrings->objectForKey("GilPlayer"))->_string;

	auto buttonGil = Button::create("newGame/Button0.png", "newGame/Button1.png");
	buttonGil->setTitleText(s_computer);
	this->isGilAI = true;
	buttonGil->setTitleFontSize(25);
	buttonGil->setScale(1.5);
	buttonGil->setTitleFontName("fonts/hanzi.ttf");
	buttonGil->setPosition(Vec2(DESIGN_WIDTH * 0.62 - BOARD_SHIFT_X, DESIGN_HEIGHT * 0.6 - BOARD_SHIFT_Y));
	buttonGil->setAnchorPoint(Vec2(0.5, 0.5));
	board->addChild(buttonGil, 2);
	

	// wendy
	auto wendy = Sprite::create("newGame/Wendy1.png");
	wendy->setPosition(DESIGN_WIDTH * 0.35 - BOARD_SHIFT_X, DESIGN_HEIGHT * 0.7 - BOARD_SHIFT_Y);
	wendy->setScale(0.25);
	wendy->setAnchorPoint(Vec2(0.5, 0.5));
	board->addChild(wendy, 2);

	auto s_wendyPlayer = ((String*)chnStrings->objectForKey("WendyPlayer"))->_string;

	auto buttonWendy = Button::create("newGame/Button0.png", "newGame/Button1.png");
	buttonWendy->setTitleText(s_wendyPlayer);
	isWendyAI = false;
	buttonWendy->setTitleFontSize(25);
	buttonWendy->setScale(1.5);
	buttonWendy->setTitleFontName("fonts/hanzi.ttf");
	buttonWendy->setPosition(Vec2(DESIGN_WIDTH * 0.62 - BOARD_SHIFT_X, DESIGN_HEIGHT * 0.7 - BOARD_SHIFT_Y));
	buttonWendy->setAnchorPoint(Vec2(0.5, 0.5));
	board->addChild(buttonWendy, 2);

	// click event of gil and wendy
	buttonGil->addClickEventListener(
	[buttonGil, buttonWendy, s_gilPlayer, s_wendyPlayer, s_computer, this](Ref * pSender) {
		if (buttonGil->getTitleText() == (s_gilPlayer)) {
			buttonGil->setTitleText(s_computer);
			this->isGilAI = true;
			if (this->isWendyAI) {
				this->isWendyAI = false;
				buttonWendy->setTitleText(s_wendyPlayer);
			}
		} else {
			buttonGil->setTitleText(s_gilPlayer);
			this->isGilAI = false;
		}

		// play sound
		AudioM::playSound("audio/sound/Button");
	});

	buttonWendy->addClickEventListener(
	[buttonGil, buttonWendy, s_gilPlayer, s_wendyPlayer, s_computer, this](Ref * pSender) {
		if (buttonWendy->getTitleText() == (s_wendyPlayer)) {
			buttonWendy->setTitleText(s_computer);
			this->isWendyAI = true;
			if (this->isGilAI) {
				this->isGilAI = false;
				buttonGil->setTitleText(s_gilPlayer);
			}
		} else {
			buttonWendy->setTitleText(s_wendyPlayer);
			this->isWendyAI = false;
		}

		// play sound
		AudioM::playSound("audio/sound/Button");
	});

	// Difficulty
	auto difficulty = Sprite::create("newGame/Difficulty.png");
	difficulty->setPosition(DESIGN_WIDTH * 0.35 - BOARD_SHIFT_X, DESIGN_HEIGHT * 0.5 - BOARD_SHIFT_Y);
	difficulty->setScale(0.25);
	difficulty->setAnchorPoint(Vec2(0.5, 0.5));
	board->addChild(difficulty, 2);

	auto s_easy = ((String*)chnStrings->objectForKey("Easy"))->_string;
	auto s_medium = ((String*)chnStrings->objectForKey("Medium"))->_string;
	auto s_hard = ((String*)chnStrings->objectForKey("Hard"))->_string;

	auto buttonDifficulty = Button::create("newGame/Button0.png", "newGame/Button1.png");
	buttonDifficulty->setTitleText(s_easy);
	this->difficulty = 0;
	buttonDifficulty->setTitleFontSize(25);
	buttonDifficulty->setScale(1.5);
	buttonDifficulty->setTitleFontName("fonts/hanzi.ttf");
	buttonDifficulty->setPosition(Vec2(DESIGN_WIDTH * 0.62 - BOARD_SHIFT_X, DESIGN_HEIGHT * 0.5 - BOARD_SHIFT_Y));
	buttonDifficulty->setAnchorPoint(Vec2(0.5, 0.5));
	board->addChild(buttonDifficulty, 2);

	buttonDifficulty->addClickEventListener([s_easy, s_medium, s_hard, buttonDifficulty, this](Ref* pSender) {
		this->difficulty = (this->difficulty + 1) % 2;
		switch (this->difficulty) {
		case 0:
			buttonDifficulty->setTitleText(s_easy);
			break;
		case 1:
			buttonDifficulty->setTitleText(s_hard);
			break;
		}
		
		// play sound
		AudioM::playSound("audio/sound/Button");
	});

	// block
	auto block = Sprite::create("newGame/Block.png");
	block->setPosition(DESIGN_WIDTH * 0.35 - BOARD_SHIFT_X, DESIGN_HEIGHT * 0.4 - BOARD_SHIFT_Y);
	block->setScale(0.25);
	block->setAnchorPoint(Vec2(0.5, 0.5));
	board->addChild(block, 2);

	auto s_5Block = ((String*)chnStrings->objectForKey("Block5"))->_string;
	auto s_10Block = ((String*)chnStrings->objectForKey("Block10"))->_string;
	auto s_15Block = ((String*)chnStrings->objectForKey("Block15"))->_string;

	auto buttonBlock = Button::create("newGame/Button0.png", "newGame/Button1.png");
	buttonBlock->setTitleText(s_10Block);
	this->blockNum = 10;
	buttonBlock->setTitleFontSize(25);
	buttonBlock->setScale(1.5);
	buttonBlock->setTitleFontName("fonts/hanzi.ttf");
	buttonBlock->setPosition(Vec2(DESIGN_WIDTH * 0.62 - BOARD_SHIFT_X, DESIGN_HEIGHT * 0.4 - BOARD_SHIFT_Y));
	buttonBlock->setAnchorPoint(Vec2(0.5, 0.5));
	board->addChild(buttonBlock, 2);

	buttonBlock->addClickEventListener([s_5Block, s_10Block, s_15Block, buttonBlock, this](Ref * pSender) {
		switch (this->blockNum) {
		case 5:
			buttonBlock->setTitleText(s_10Block);
			this->blockNum = 10;
			break;
		case 10:
			buttonBlock->setTitleText(s_15Block);
			this->blockNum = 15;
			break;
		case 15:
			buttonBlock->setTitleText(s_5Block);
			this->blockNum = 5;
			break;
		}

		// play sound
		AudioM::playSound("audio/sound/Button");
	});

	// cancle
	auto buttonCancle = Button::create("newGame/False0.png", "newGame/False1.png");
	buttonCancle->setPosition(Vec2(DESIGN_WIDTH * 0.6 - BOARD_SHIFT_X, DESIGN_HEIGHT * 0.3 - BOARD_SHIFT_Y));
	buttonCancle->setScale(0.3);
	buttonCancle->setAnchorPoint(Vec2(0.5, 0.5));

	buttonCancle->addClickEventListener([this](Ref* pSender) {
		disappearAction();

		// play sound
		AudioM::playSound("audio/sound/Button");
	});

	board->addChild(buttonCancle, 2);


	// ok
	auto buttonOk = Button::create("newGame/True0.png", "newGame/True1.png");
	buttonOk->setPosition(Vec2(DESIGN_WIDTH * 0.4 - BOARD_SHIFT_X, DESIGN_HEIGHT * 0.3 - BOARD_SHIFT_Y));
	buttonOk->setScale(0.3);
	buttonOk->setAnchorPoint(Vec2(0.5, 0.5));
	board->addChild(buttonOk, 2);

	buttonOk->addClickEventListener([this](Ref * pSender) {
		auto curScene = (GameScene *)Director::getInstance()->getRunningScene();
		curScene->initialGame(this->isGilAI, this->isWendyAI, this->difficulty, this->blockNum);

		// add action
		disappearAction();

		// add music
		AudioM::playBGM("audio/music/BaoDao");

		// play sound
		AudioM::playSound("audio/sound/Button");
	});

	// SwallowTouches by shadow
	auto listenerShadow = EventListenerTouchOneByOne::create();
	listenerShadow->onTouchBegan = [](Touch* touch, Event * event) {
		auto location = touch->getLocation();
		// log("touch at (%f, %f) !!!", location.x, location.y);
		return true;
	};
	listenerShadow->setSwallowTouches(true);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerShadow, this);
	return true;
}

void NewGameLayer::disappearAction()
{
	// add action£¡
	auto fadeOut = FadeOut::create(1.2f);
	auto moveTo = MoveBy::create(1.0f, Vec2(720, 0));
	auto moveEaseIn = EaseBackIn::create(moveTo->clone());

	auto cancleCallBack = CallFunc::create([this]() {
		this->removeFromParent();
	});

	auto shadowSeq = Sequence::create(fadeOut, cancleCallBack, nullptr);

	shadow->runAction(shadowSeq);
	board->runAction(moveEaseIn);
	//this->removeFromParent();
}

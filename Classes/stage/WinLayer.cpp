#include "WinLayer.h"
#include "ui/CocosGUI.h"
#include "myAudio/AudioM.h"
using namespace ui;

bool WinLayer::init()
{
	if (!Layer::init()) {
		return false;
	}

	// shadow
	shadow = Sprite::create("newGame/Black.png");
	shadow->setPosition(0, 0);
	shadow->setAnchorPoint(Vec2::ZERO);
	shadow->setOpacity(0);
	this->addChild(shadow, 0);

	// board
	board = Sprite::create("newGame/Board.png");
	board->setAnchorPoint(Vec2(0.5, 0.5));
	board->setPosition(DESIGN_WIDTH * 3 / 2, DESIGN_HEIGHT / 2);
	shadow->addChild(board, 1);

	// ok
	auto buttonOk = Button::create("newGame/True0.png", "newGame/True1.png");
	buttonOk->setPosition(Vec2(DESIGN_WIDTH * 0.5 - BOARD_SHIFT_X, DESIGN_HEIGHT * 0.3 - BOARD_SHIFT_Y));
	buttonOk->setScale(0.3);
	buttonOk->setAnchorPoint(Vec2(0.5, 0.5));
	board->addChild(buttonOk, 2);

	buttonOk->addClickEventListener([this](Ref* pSender) {
		disappearAction();
	});

	appearAction();

	AudioM::playBGM("audio/music/Win", false);
	return true;
}

void WinLayer::setWinner(bool isWendy)
{
	Sprite * winner, * loser;
	if (isWendy) {
		winner = Sprite::create("winLayer/Wendy1.png");
		loser = Sprite::create("winLayer/Gil0.png");
	} else {
		winner = Sprite::create("winLayer/Gil1.png");
		loser = Sprite::create("winLayer/Wendy0.png");
	}
	winner->setPosition(0.5 * DESIGN_WIDTH - BOARD_SHIFT_X, 0.65 * DESIGN_HEIGHT - BOARD_SHIFT_Y);
	loser->setPosition(0.5 * DESIGN_WIDTH - BOARD_SHIFT_X, 0.45 * DESIGN_HEIGHT - BOARD_SHIFT_Y);
	winner->setScale(0.5);
	loser->setScale(0.5);
	board->addChild(winner, 5);
	board->addChild(loser, 5);
}


void WinLayer::appearAction()
{
	// add action£¡
	auto fadeIn = FadeIn::create(2.0f);
	auto moveTo = MoveBy::create(1.0f, Vec2(-720, 0));
	auto moveEaseIn = EaseBackOut::create(moveTo->clone());
	auto delay = DelayTime::create(2.0f);

	auto shadowSeq = Sequence::create(fadeIn, nullptr);
	auto boardSeq = Sequence::create(delay, moveEaseIn, nullptr);

	shadow->runAction(shadowSeq);
	board->runAction(boardSeq);
}

// same as the NewGameLayer::disappearAction
void WinLayer::disappearAction()
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
}

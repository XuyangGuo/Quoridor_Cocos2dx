#include "SceneM.h"
#include "MenuScene.h"
#include "GameScene.h"
#include "AboutScene.h"
USING_NS_CC;

static SceneM * s_SceneM = nullptr;

SceneM * SceneM::getInstance()
{
	if (!s_SceneM) {
		s_SceneM = new (std::nothrow)SceneM();
		CCASSERT(s_SceneM, "FATAL! Not enough Memory.");
	}
	return s_SceneM;
}

void SceneM::changeScene(SceneType sceneType, TransitionType tt)
{
	Scene *scene;
	switch (sceneType) {
	case stMenuScene:
		scene = MenuScene::create();
		break;
	case stGameScene:
		scene = GameScene::create();
		break;
	case stAboutScene:
		scene = AboutScene::create();
		break;
	}

	switch (tt) {
	case TRANSITION_FADE:
		scene = TransitionFade::create(0.8f, scene);
		break;
	case TRANSITION_MOVE_IN_L:
		scene = TransitionMoveInL::create(0.5f, scene);
		break;
	case PAGE_TRANSITION_FORWARD:
		if (((MenuScene *)Director::getInstance()->getRunningScene())->getClickPotato() > 6) {
			scene = TransitionPageTurn::create(4.0f, scene, false);
		} else {
			scene = TransitionPageTurn::create(1.0f, scene, false);
		}
		break;
	case PAGE_TRANSITION_BACKWARD:
		scene = TransitionPageTurn::create(0.5f, scene, true);
		break;
	}
	
	auto dr = Director::getInstance();
	if (dr->getRunningScene()) {
		dr->replaceScene(scene);
	} else {
		dr->runWithScene(scene);
	}
}

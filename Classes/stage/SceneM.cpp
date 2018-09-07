#include "SceneM.h"
#include "MenuScene.h"
#include "GameScene.h"
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

void SceneM::changeScene(SceneType sceneType)
{
	Scene *scene;
	switch (sceneType) {
	case stMenuScene:
		scene = MenuScene::create();
		break;
	case stGameScene:
		scene = GameScene::create();
		break;
	}
	
	auto dr = Director::getInstance();
	if (dr->getRunningScene()) {
		dr->replaceScene(scene);
	} else {
		dr->runWithScene(scene);
	}
}

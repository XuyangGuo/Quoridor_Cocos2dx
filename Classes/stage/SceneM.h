#pragma once
#include "cocos2d.h"
USING_NS_CC;

enum SceneType {
	stMenuScene,
	stGameScene,
	// stAboutScene,
};

class SceneM {
public:
	static SceneM * getInstance();
	// change scene
	void changeScene(SceneType sceneType);
};

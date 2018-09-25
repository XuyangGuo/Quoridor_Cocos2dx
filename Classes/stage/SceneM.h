#pragma once
#include "cocos2d.h"
USING_NS_CC;

enum SceneType {
	stMenuScene,
	stGameScene,
	stAboutScene,
};

enum TransitionType {
	PAGE_TRANSITION_FORWARD,
	PAGE_TRANSITION_BACKWARD,
	TRANSITION_FADE,
	TRANSITION_MOVE_IN_L,
	EMPTY_TRANSITION
};

class SceneM {
public:
	static SceneM * getInstance();
	// change scene
	void changeScene(SceneType sceneType,  TransitionType tt = EMPTY_TRANSITION);
};

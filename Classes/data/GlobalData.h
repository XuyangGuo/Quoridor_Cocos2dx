#pragma once
#include "cocos2d.h"
USING_NS_CC;

const auto DESIGN_WIDTH = 720;
const auto DESIGN_HEIGHT = 1280;

const int CHESS_STEP = 0.85 * DESIGN_WIDTH / 8;
const int CHESS_HEIGHT_CENTER = 0.55 * DESIGN_HEIGHT;

// const int OPACITY = 255;
const int CHOOSE_OPACITY = 120;

const int BOARD_SHIFT_X = 95;
const int BOARD_SHIFT_Y = 251;

Rect getSpriteRect(Sprite * sprite);
void setChildrenOpacity(Sprite * sprite, int opacity);
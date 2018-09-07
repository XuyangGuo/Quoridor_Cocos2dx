#include "GlobalData.h"

Rect getSpriteRect(Sprite * sprite)
{
	auto width = sprite->getContentSize().width * sprite->getScaleX();
	auto height = sprite->getContentSize().height * sprite->getScaleY();
	auto pos = sprite->getPosition();
	auto anchor = sprite->getAnchorPoint();
	return Rect(pos.x - anchor.x * width, pos.y - anchor.y * height, width, height);
}

void setChildrenOpacity(Sprite * sprite, int opacity)
{
	auto children = sprite->getChildren();
	for (auto it = children.begin(); it != children.end(); ++it) {
		(*it)->setOpacity(opacity);
	}
}
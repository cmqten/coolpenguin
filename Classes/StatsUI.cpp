#include "StatsUI.h"

using namespace cocos2d;

void StatsUI::onEnter() {
    Node::onEnter();
    // Background and border
    Color4B iceBlue(0xd4, 0xf0, 0xff, 0xff);
    Color4B oceanBlue(0, 0x77, 0xbe, 0xff);
    auto bg = LayerColor::create(iceBlue, 320, 640);
    auto border = DrawNode::create();
    border->drawSolidRect(Vec2(0, 0), Vec2(320, 20), Color4F(oceanBlue));
    border->drawSolidRect(Vec2(0, 620), Vec2(320, 640), Color4F(oceanBlue));
    border->drawSolidRect(Vec2(0, 20), Vec2(20, 620), Color4F(oceanBlue));
    border->drawSolidRect(Vec2(300, 20), Vec2(320, 620), Color4F(oceanBlue));
    this->addChild(bg, -2);
    this->addChild(border, -2);
}

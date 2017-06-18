#include "StatsUI.h"

using namespace cocos2d;

void StatsUI::onEnter() {
    Node::onEnter();
    // Background and border
    Color4B iceBlue(0xd4, 0xf0, 0xff, 0xff);
    Color4B oceanBlue(0, 0x77, 0xbe, 0xff);
    auto bg = LayerColor::create(iceBlue, 320, 640);
    auto border = DrawNode::create(20.0f);
    border->drawLine(Vec2(0, 11), Vec2(321, 11), Color4F(oceanBlue));
    border->drawLine(Vec2(0, 629), Vec2(321, 629), Color4F(oceanBlue));
    border->drawLine(Vec2(310, 0), Vec2(310, 640), Color4F(oceanBlue));
    border->drawLine(Vec2(11, 0), Vec2(11, 640), Color4F(oceanBlue));
    this->addChild(bg, -2);
    this->addChild(border, -2);
}

#include "StatsUI.h"
#include <cstdlib>

using namespace cocos2d;

void StatsUI::onEnter() {
    Node::onEnter();

    // Background and border
    Color4B iceBlue(0xd4, 0xf0, 0xff, 0xff);
    Color4B oceanBlue(0, 0x77, 0xbe, 0xff);
    auto bgAndBorder = DrawNode::create();
    bgAndBorder->drawSolidRect(Vec2(0, 0), Vec2(320, 640), Color4F(oceanBlue));
    bgAndBorder->drawSolidRect(Vec2(20, 20), Vec2(300, 620), Color4F(iceBlue));
    this->addChild(bgAndBorder, -2, "bgAndBorder");

    // Listener for stat updates
    this->getEventDispatcher()->addCustomEventListener(UPDATE_STATS,
        [this, bgAndBorder](EventCustom* event) {
            // Random event for now, hides stats ui if fish is less than
            // ice cream
            struct GameStats* stats = (struct GameStats*)event->getUserData();
            bgAndBorder->setVisible(stats->fishCount >= stats->iceCreamCount);
            free(stats);
        });
}



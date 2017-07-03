#include "GameUI.h"
#include <string>
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace std;

void GameUI::incrementTime(float delta) {
    auto timerDisplay = (Label*)getChildByName("timer_display");
    timerDisplay->setString(to_string(++_time));
    getEventDispatcher()->dispatchCustomEvent(TIMER_TICK, (void*)&_time);
}

void GameUI::onEnter() {
    Node::onEnter();

    // Background and border
    Color4B iceBlue(0xd4, 0xf0, 0xff, 0xff);
    Color4B oceanBlue(0, 0x77, 0xbe, 0xff);
    auto bgAndBorder = DrawNode::create();
    bgAndBorder->drawSolidRect(Vec2(0, 0), Vec2(320, 640), Color4F(oceanBlue));
    bgAndBorder->drawSolidRect(Vec2(16, 16), Vec2(304, 624), Color4F(iceBlue));
    addChild(bgAndBorder, -2, "bgAndBorder");

    /* Lambda for creating labels to remove reptitive code */
    auto createLabel = [this](string text, float size, Vec2& position,
        string name) {
        auto label = Label::createWithTTF(TTFConfig(
            "fonts/BPdotsUnicaseSquareBold.ttf", size), text);
        label->setColor(Color3B::BLACK);
        label->setAnchorPoint(Vec2(0, 0));
        label->setPosition(position);
        this->addChild(label, 0, name);
    };

    createLabel("TIME", 40.0f, Vec2(24, 588), "timer_label");
    createLabel("0", 40.0f, Vec2(24, 560), "timer_display");
    createLabel("SCORE", 40.0f, Vec2(24, 504), "score_label");
    createLabel("0", 40.0f, Vec2(24, 476), "score_display");
    createLabel("STATUS", 40.0f, Vec2(24, 420), "status_label");
    createLabel("SQUEAKY CLEAN", 30.0f, Vec2(24, 392), "status_display");

    // Timer increment
    schedule(SEL_SCHEDULE(&GameUI::incrementTime), 1.0f);

    // Status display

    // Listener for time updates
    getEventDispatcher()->addCustomEventListener(TIMER_TICK, 
        [this](EventCustom* event) {
            auto timerDisplay = (Label*)this->getChildByName("timer_display");
            timerDisplay->setString(to_string(*(int*)event->getUserData()));
        });

    // Listener for stat updates
    getEventDispatcher()->addCustomEventListener(UPDATE_STATS,
        [this, bgAndBorder](EventCustom* event) {
            // Random event for now, hides stats ui if fish is less than
            // ice cream
            GameStats* stats = (GameStats*)event->getUserData();
        });
}

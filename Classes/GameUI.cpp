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
    auto createLabel = [this](string text, float size, Vec2 position,
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

            // Updates the status display
            auto statusDisplay = (Label*)getChildByName("status_display");
            switch (stats->fishCount - stats->iceCreamCount) {
                case 0:
                    statusDisplay->setColor(Color3B::BLACK);
                    statusDisplay->setString("CLEAN");
                    break;

                case 1: 
                case 2:
                case 3:
                    statusDisplay->setColor(Color3B(0x2b, 0x86, 0xa4));
                    statusDisplay->setString("DIRTY");
                    break;

                case 4:
                case 5:
                case 6:
                    statusDisplay->setColor(Color3B(0x2b, 0x86, 0xa4));
                    statusDisplay->setString("VERY DIRTY");
                    break;

                case 7:
                case 8:
                case 9:
                    statusDisplay->setColor(Color3B(0x2b, 0x86, 0xa4));
                    statusDisplay->setString("SUPER DIRTY");
                    break;

                case -1:
                case -2:
                case -3:
                    statusDisplay->setColor(Color3B(0xf3, 0x7e, 0xf4));
                    statusDisplay->setString("DIRTY");
                    break;

                case -4:
                case -5:
                case -6:
                    statusDisplay->setColor(Color3B(0xf3, 0x7e, 0xf4));
                    statusDisplay->setString("VERY DIRTY");
                    break;

                case -7:
                case -8:
                case -9:
                    statusDisplay->setColor(Color3B(0xf3, 0x7e, 0xf4));
                    statusDisplay->setString("SUPER DIRTY");
                    break;

                default:
                    statusDisplay->setColor(Color3B::RED);
                    statusDisplay->setString("CLOGGED");
                    break;
            }
        });
}

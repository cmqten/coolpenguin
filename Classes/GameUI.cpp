#include "GameUI.h"
#include <cstdlib>
#include <string>
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace std;

GameUI* GameUI::_instance = nullptr;

GameUI* GameUI::getInstance() {
    if (!_instance) _instance = GameUI::create();
    return _instance;
}

void GameUI::incrementTime(float delta) {
    auto timerDisplay = (Label*)getChildByName("timer_display");
    timerDisplay->setString(to_string(++_time));
    getEventDispatcher()->dispatchCustomEvent(TIMER_TICK, (void*)&_time);
}

void GameUI::updateCannonStats(int fishShot, int iceCreamShot, int fishReserve,
    int iceCreamReserve) {
    auto statusDisplay = (Label*)getChildByName("status_display");

    // Sets the color, blue if more fish, pink if more ice cream, black if equal
    if (fishShot > iceCreamShot) 
        statusDisplay->setColor(Color3B(0x2b, 0x86, 0xa4));

    else if (fishShot < iceCreamShot) 
        statusDisplay->setColor(Color3B(0xf3, 0x7e, 0xf4));

    else statusDisplay->setColor(Color3B::BLACK);

    // Updates the status
    switch (abs(fishShot - iceCreamShot)) {
        case 0:
            if (!fishShot && !iceCreamShot)
                statusDisplay->setString("SQUEAKY CLEAN");
            else statusDisplay->setString("CLEAN");
            break;

        case 1: case 2: case 3:
            statusDisplay->setString("DIRTY");
            break;

        case 4: case 5: case 6:
            statusDisplay->setString("VERY DIRTY");
            break;

        case 7: case 8: case 9:
            statusDisplay->setString("SUPER DIRTY");
            break;

        default:
            statusDisplay->setColor(Color3B::RED);
            statusDisplay->setString("CLOGGED");
            break;
    }

    // Updates reserve
    auto fishDisplay = (Label*)getChildByName("fish_display");
    fishDisplay->setString(to_string(fishReserve));
    auto iceCreamDisplay = (Label*)getChildByName("ice_cream_display");
    iceCreamDisplay->setString(to_string(iceCreamReserve));
}

void GameUI::updateScore(int points) {
    _score += points;
    ((Label*)getChildByName("score_display"))->setString(to_string(_score));
}

bool GameUI::init() {
    if (!Layer::init()) return false;

    // Background and border
    Color4B iceBlue(0xd4, 0xf0, 0xff, 0xff);
    Color4B oceanBlue(0, 0x77, 0xbe, 0xff);
    auto bgAndBorder = DrawNode::create();
    bgAndBorder->drawSolidRect(Vec2(0, 0), Vec2(320, 640), Color4F(oceanBlue));
    bgAndBorder->drawSolidRect(Vec2(16, 16), Vec2(304, 624), Color4F(iceBlue));
    addChild(bgAndBorder, -2, "bgAndBorder");

    // Lambda for creating labels to remove reptitive code 
    auto createLabel = [this](string text, float size, Vec2 position,
        string name) {
        auto label = Label::createWithTTF(TTFConfig(
            "fonts/BPdotsUnicaseSquareBold.ttf", size), text);
        label->setColor(Color3B::BLACK);
        label->setAnchorPoint(Vec2(0, 0));
        label->setPosition(position);
        this->addChild(label, 0, name);
    };

    _time = 0;
    createLabel("TIME", 40.0f, Vec2(24, 588), "timer_label");
    createLabel("0", 40.0f, Vec2(24, 560), "timer_display");

    _score = 0;
    createLabel("SCORE", 40.0f, Vec2(24, 504), "score_label");
    createLabel(to_string(_score), 40.0f, Vec2(24, 476), "score_display");

    createLabel("STATUS", 40.0f, Vec2(24, 420), "status_label");
    createLabel("", 30.0f, Vec2(24, 392), "status_display");
    createLabel("RESERVE", 40.0f, Vec2(24, 336), "reserve_label");

    // Fish reserve display and count
    addChild([]()->Sprite* {
        auto fishReserve = Sprite::create("img/fish_reserve.png");
        fishReserve->setAnchorPoint(Vec2(0, 0));
        fishReserve->setPosition(24, 272);
        return fishReserve;
    }());
    createLabel("0", 40.0f, Vec2(88, 284), "fish_display");

    addChild([]()->Sprite* {
        auto iceCreamReserve = Sprite::create("img/ice_cream_reserve.png");
        iceCreamReserve->setAnchorPoint(Vec2(0, 0));
        iceCreamReserve->setPosition(24, 208);
        return iceCreamReserve;
    }());
    createLabel("0", 40.0f, Vec2(88, 220), "ice_cream_display");

    // Timer increment
    schedule(SEL_SCHEDULE(&GameUI::incrementTime), 1.0f);
    
    return true;
}

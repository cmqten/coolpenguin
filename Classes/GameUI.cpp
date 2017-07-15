#include "GameUI.h"
#include <cstdlib>
#include <string>
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace std;

void GameUI::incrementTime(float delta) {
    auto timerDisplay = (Label*)getChildByName("timer_display");
    if (--_time < 0) return;
    timerDisplay->setString(to_string(_time));
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

void GameUI::reset() {
    _score = 0;
    getChildByName<Label*>("score_display")->setString("0");

    _time = 120;
    unschedule(SEL_SCHEDULE(&GameUI::incrementTime));
    getChildByName<Label*>("timer_display")->setString("120");
    schedule(SEL_SCHEDULE(&GameUI::incrementTime), 1.0f);
}

bool GameUI::init() {
    if (!Layer::init()) return false;

    // Background and border
    Color4B green(0x0, 0xaa, 0x61, 0xff);
    Color4B lightGreen(0x6f, 0xff, 0x79, 0xff);
    auto bg = DrawNode::create();
    bg->drawSolidRect(Vec2(0, 0), Vec2(320, 640), Color4F(green));
    bg->drawSolidRect(Vec2(20, 20), Vec2(300, 620), Color4F(lightGreen));
    addChild(bg, -2, "bg");

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

    _time = 120;
    createLabel("TIME", 40.0f, Vec2(24, 588), "timer_label");
    createLabel("120", 40.0f, Vec2(24, 560), "timer_display");

    _score = 0;
    createLabel("SCORE", 40.0f, Vec2(24, 504), "score_label");
    createLabel(to_string(_score), 40.0f, Vec2(24, 476), "score_display");

    createLabel("STATUS", 40.0f, Vec2(24, 420), "status_label");
    createLabel("", 30.0f, Vec2(24, 392), "status_display");
    createLabel("RESERVE", 40.0f, Vec2(24, 336), "reserve_label");

    // Fish reserve display and count
    addChild([]()->Sprite* {
        auto fishReserve = Sprite::createWithSpriteFrameName("img/fish_r.png");
        fishReserve->setFlippedY(true);
        fishReserve->setRotation(90.0f);
        fishReserve->setAnchorPoint(Vec2(0, 0));
        fishReserve->setPosition(24, 336);
        return fishReserve;
    }());
    createLabel("0", 40.0f, Vec2(88, 284), "fish_display");

    // Ice cream reserve display and count
    addChild([]()->Sprite* {
        auto iceCreamReserve = Sprite::createWithSpriteFrameName(
            "img/ice_cream_r.png");
        iceCreamReserve->setAnchorPoint(Vec2(0, 0));
        iceCreamReserve->setPosition(24, 208);
        return iceCreamReserve;
    }());
    createLabel("0", 40.0f, Vec2(88, 220), "ice_cream_display");

    // Reset button
    auto resetButton = ui::Button::create();
    resetButton->setContentSize(Size(320, 128));
    resetButton->loadTextures("img/reset_up.png", "img/reset_down.png",
        "img/reset_disable.png", ui::Widget::TextureResType::PLIST);
    resetButton->setScale(0.5f);
    resetButton->setAnchorPoint(Vec2(0, 0));
    resetButton->setPosition(Vec2(80, 32));
    resetButton->addClickEventListener([this](Ref* ref) {
        auto resettableParent = dynamic_cast<IReset*>(this->getParent());
        if (resettableParent) resettableParent->reset();
    });
    addChild(resetButton);

    // Menu button
    auto menuButton = ui::Button::create();
    menuButton->setContentSize(Size(320, 128));
    menuButton->loadTextures("img/menu_up.png", "img/menu_down.png",
        "img/menu_disable.png", ui::Widget::TextureResType::PLIST);
    menuButton->setScale(0.5f);
    menuButton->setAnchorPoint(Vec2(0, 0));
    menuButton->setPosition(Vec2(80, 112));
    menuButton->addClickEventListener([](Ref* ref) {
        Director::getInstance()->popScene();
    });
    //menuButton->setBright(false);
    //menuButton->setTouchEnabled(false);
    addChild(menuButton);

    // Timer increment
    schedule(SEL_SCHEDULE(&GameUI::incrementTime), 1.0f);
    
    return true;
}

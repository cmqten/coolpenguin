#include <cmath>
#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocostudio::timeline;
using namespace std;

Scene* HelloWorld::createScene() {
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}

bool HelloWorld::init() {
    if (!Layer::init()) return false;
    auto bg = Sprite::create("white.png");
    bg->setPosition(480, 320);
    this->addChild(bg);

    auto penguin = Sprite::create("penguin_glasses.png");
    penguin->setPosition(480, 320);
    this->addChild(penguin);

    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseMove = [this, penguin](cocos2d::EventMouse* event) {
        float mouseVecX = event->getCursorX() - 480;
        float mouseVecY = event->getCursorY() - 320;
        float pengVecX = 0; float pengVecY = 20;
        float dot = mouseVecX * pengVecX + mouseVecY * pengVecY;
        float det = mouseVecX * pengVecY - mouseVecY * pengVecX;
        penguin->setRotation(atan2(det, dot) * 180 / M_PI);
    };

    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(
        mouseListener, this);
}

void update(float delta) {

}

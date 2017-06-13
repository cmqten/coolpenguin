#include <cmath>
#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocostudio::timeline;
using namespace std;

Scene* HelloWorld::createScene() {
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
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

    auto keyListener = EventListenerKeyboard::create();
    keyListener->onKeyPressed = [this, penguin](EventKeyboard::KeyCode keyCode,
        Event* event) {
        if (keyCode == EventKeyboard::KeyCode::KEY_SPACE) {
            auto fish = Sprite::create("fish.png");
            fish->setPosition(Vec2(480, 320));
            auto fbody = PhysicsBody::createBox(Size(36, 60));
            fish->setPhysicsBody(fbody);
            fish->setRotation(penguin->getRotation());
            float rad = (90 - penguin->getRotation()) * M_PI / 180;
            fbody->setVelocity(Vec2(5000 * cosf(rad), 5000 * sinf(rad)));
            this->addChild(fish);
        }
    };

    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(
        mouseListener, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(
        keyListener, this);

    return true;
}

void update(float delta) {

}

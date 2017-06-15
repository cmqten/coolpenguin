#include "PenguinGame.h"
#include <cmath>
#include "cocostudio/CocoStudio.h"
#include "TNodeReader.h"

#define RAD(deg) (deg) * M_PI / 180
#define DEG(rad) (rad) * 180 / M_PI

using namespace cocos2d;
using namespace std;

Scene* PenguinGame::createScene() {
    // Registers custom loaders
    CSLoader::getInstance()->registReaderObject("PenguinGameReader",
        (ObjectFactory::Instance)TNodeReader<PenguinGame>::getInstance);

    auto scene = Scene::createWithPhysics();
    auto layer = PenguinGame::create();
    scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
#ifdef _DEBUG
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
#endif
    scene->addChild(layer);
    return scene;
}

void PenguinGame::onEnter() {
    Node::onEnter();
    // background
    auto bg = Sprite::create("white.png");
    bg->setPosition(480, 320);
    this->addChild(bg, -2);

    // cannon
    auto cannon = Sprite::create("cannon.png");
    cannon->setAnchorPoint(Vec2(0.5, 0.25));
    cannon->setPosition(480, 320);
    this->addChild(cannon, 0, 0);

    // mouse listener
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseMove = [this, cannon](EventMouse* event) {
        float mouseVecX = event->getCursorX() - 480;
        float mouseVecY = event->getCursorY() - 320;
        float cannonVecX = 0;
        float cannonVecY = 10;
        float dot = mouseVecX * cannonVecX + mouseVecY * cannonVecY;
        float det = mouseVecX * cannonVecY - mouseVecY * cannonVecX;
        cannon->setRotation(DEG(atan2f(det, dot)));
    };
    this->getEventDispatcher()->
        addEventListenerWithSceneGraphPriority(mouseListener, this);

    // keyboard listener
    auto keyListener = EventListenerKeyboard::create();
    keyListener->onKeyPressed = CC_CALLBACK_2(PenguinGame::onKeyPressed, this);
    this->getEventDispatcher()->
        addEventListenerWithSceneGraphPriority(keyListener, this);
}

void PenguinGame::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
    switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_J: {
            auto fish = Sprite::create("fish_r.png");
            fish->setPosition(Vec2(480, 320));
            auto fbody = PhysicsBody::createBox(Size(12, 12));
            fish->setPhysicsBody(fbody);
            fish->setRotation(this->getChildByTag(0)->getRotation());
            float rad = RAD(90 - this->getChildByTag(0)->getRotation());
            fbody->setVelocity(Vec2(500 * cosf(rad), 500 * sinf(rad)));
            this->addChild(fish, -1);
			break;
        }


        case EventKeyboard::KeyCode::KEY_K: {
            auto ic = Sprite::create("ice_cream_r.png");
            ic->setPosition(Vec2(480, 320));
            auto icbody = PhysicsBody::createBox(Size(12, 12));
            ic->setPhysicsBody(icbody);
            ic->setRotation(this->getChildByTag(0)->getRotation());
            float rad = RAD(90 - this->getChildByTag(0)->getRotation());
            icbody->setVelocity(Vec2(500 * cosf(rad), 500 * sinf(rad)));
            this->addChild(ic, -1);
            break;
        }
    }
}

void PenguinGame::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) {
}

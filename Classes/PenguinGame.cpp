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
    CSLoader::getInstance()->registReaderObject("CannonReader",
        (ObjectFactory::Instance)TNodeReader<Cannon>::getInstance);

    auto scene = Scene::createWithPhysics();
    auto layer = CSLoader::createNode("csb/penguingame.csb");
    scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
#ifdef _DEBUG
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
#endif
    scene->addChild(layer);
    return scene;
}

void PenguinGame::onEnter() {
    Node::onEnter();
    this->getChildByName("bg")->setZOrder(-2);

    // cannon
    this->_cannon = dynamic_cast<Cannon*>(this->getChildByName("cannon"));

    // mouse listener
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseMove = CC_CALLBACK_1(PenguinGame::onMouseMove, this);
    this->getEventDispatcher()->
        addEventListenerWithSceneGraphPriority(mouseListener, this);

    // keyboard listener
    auto keyListener = EventListenerKeyboard::create();
    keyListener->onKeyPressed = CC_CALLBACK_2(PenguinGame::onKeyPressed, this);
    this->getEventDispatcher()->
        addEventListenerWithSceneGraphPriority(keyListener, this);
}

void PenguinGame::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
    this->_cannon->onKeyPressed(keyCode, event);
}

void PenguinGame::onMouseMove(EventMouse* event) {
    this->_cannon->onMouseMove(event);
}

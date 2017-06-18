#include "PenguinGame.h"
#include "StatsUI.h"
#include "TNodeReader.h"

using namespace cocos2d;
using namespace std;

Scene* PenguinGame::createScene() {
    // Registers custom loaders
    CSLoader::getInstance()->registReaderObject("PenguinGameReader",
        (ObjectFactory::Instance)TNodeReader<PenguinGame>::getInstance);
    CSLoader::getInstance()->registReaderObject("CannonReader",
        (ObjectFactory::Instance)TNodeReader<Cannon>::getInstance);
    CSLoader::getInstance()->registReaderObject("StatsUIReader",
        (ObjectFactory::Instance)TNodeReader<StatsUI>::getInstance);

    auto scene = Scene::createWithPhysics();
    auto layer = CSLoader::createNode("csb/penguingame.csb");
    scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
#ifdef _DEBUG
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
#endif
    scene->addChild(layer);
    return scene;
}

void PenguinGame::onEnter() {
    Node::onEnter();
    // White background
    auto bg = LayerColor::create(Color4B::WHITE, 640, 640);
    bg->setPosition(320, 0);
    this->addChild(bg, -2);

    // cannon
    this->_cannon = dynamic_cast<Cannon*>(this->getChildByName("cannon"));

    // border
    auto boundaryNode = Node::create();
    boundaryNode->setPhysicsBody([]()->PhysicsBody* {
        auto body = PhysicsBody::createEdgeBox(Size(700, 700),
            PHYSICSBODY_MATERIAL_DEFAULT, 10);
        body->getShapes().at(0)->setSensor(true);
        body->setDynamic(false);
        body->setCategoryBitmask(0x1);
        body->setCollisionBitmask(0x0);
        body->setContactTestBitmask(0x2);
        return body;
    }());
    boundaryNode->setPosition(640, 320);
    this->addChild(boundaryNode);

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

    // contact listener
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = 
        CC_CALLBACK_1(PenguinGame::onContactBegin, this);
    this->getEventDispatcher()->
        addEventListenerWithSceneGraphPriority(contactListener, this);
}

void PenguinGame::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
    this->_cannon->onKeyPressed(keyCode, event);
}

void PenguinGame::onMouseMove(EventMouse* event) {
    this->_cannon->onMouseMove(event);
}

bool PenguinGame::onContactBegin(PhysicsContact& contact) {
    IPhysics* entityA = dynamic_cast<IPhysics*>(contact.getShapeA()->getBody()
        ->getOwner());
    IPhysics* entityB = dynamic_cast<IPhysics*>(contact.getShapeB()->getBody()
        ->getOwner());
    if (entityA) entityA->onContactBegin(contact);
    if (entityB) entityB->onContactBegin(contact);
    /* Always returns false because this game doesn't involve any physics 
    collisions at all, just contacts. */
    return false;
}

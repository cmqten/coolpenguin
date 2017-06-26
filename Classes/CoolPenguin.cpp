#include "CoolPenguin.h"
#include "Penguin.h"
#include "PenguinSpawner.h"
#include "StatsUI.h"
#include "TNodeReader.h"

using namespace cocos2d;
using namespace std;

Scene* CoolPenguin::createScene() {
    // Registers custom loaders
    CSLoader::getInstance()->registReaderObject("CoolPenguinReader",
        (ObjectFactory::Instance)TNodeReader<CoolPenguin>::getInstance);
    CSLoader::getInstance()->registReaderObject("CannonReader",
        (ObjectFactory::Instance)TNodeReader<Cannon>::getInstance);
    CSLoader::getInstance()->registReaderObject("StatsUIReader",
        (ObjectFactory::Instance)TNodeReader<StatsUI>::getInstance);

    auto scene = Scene::createWithPhysics();
    auto layer = CSLoader::createNode("csb/coolpenguin.csb");
    scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
//#ifdef _DEBUG
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
//#endif
    scene->addChild(layer);
    return scene;
}

void CoolPenguin::onEnter() {
    Node::onEnter();

    // white background
    auto bg = LayerColor::create(Color4B::WHITE, 640, 640);
    bg->setPosition(320, 0);
    addChild(bg, -2);

    // cannon
    _cannon = dynamic_cast<Cannon*>(this->getChildByName("cannon"));

    // mouse listener
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseMove = CC_CALLBACK_1(CoolPenguin::onMouseMove, this);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(
        mouseListener, this);

    // keyboard listener
    auto keyListener = EventListenerKeyboard::create();
    keyListener->onKeyPressed = CC_CALLBACK_2(CoolPenguin::onKeyPressed, this);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(
        keyListener, this);

    // contact listener
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(
        CoolPenguin::onContactBegin, this);

    getEventDispatcher()->addEventListenerWithSceneGraphPriority(
        contactListener, this);

    /*
    auto penguin = Penguin::create();
    penguin->setPosition(640, 400);
    this->addChild(penguin, 0, "penguin");
    */
    auto spawner = PenguinSpawner::create();
    addChild(spawner);
    spawner->setPosition(640, 688);
}

void CoolPenguin::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
    _cannon->onKeyPressed(keyCode, event);
}

void CoolPenguin::onMouseMove(EventMouse* event) {
    _cannon->onMouseMove(event);
}

bool CoolPenguin::onContactBegin(PhysicsContact& contact) {
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

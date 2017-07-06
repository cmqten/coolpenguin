#include "CoolPenguin.h"
#include "Penguin.h"
#include "PenguinSpawner.h"
#include "GameUI.h"
#include "TNodeReader.h"

using namespace cocos2d;
using namespace std;

Scene* CoolPenguin::createScene() {
    // Registers custom loaders
    CSLoader::getInstance()->registReaderObject("CoolPenguinReader",
        (ObjectFactory::Instance)TNodeReader<CoolPenguin>::getInstance);
    CSLoader::getInstance()->registReaderObject("CannonReader",
        (ObjectFactory::Instance)TNodeReader<Cannon>::getInstance);
    CSLoader::getInstance()->registReaderObject("PenguinReader",
        (ObjectFactory::Instance)TNodeReader<Penguin>::getInstance);

    auto scene = Scene::createWithPhysics();
    auto layer = CoolPenguin::create();
    scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
//#ifdef _DEBUG
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
//#endif
    scene->addChild(layer);
    return scene;
}

void CoolPenguin::onEnter() {
    Layer::onEnter();

    // white background
    auto bg = LayerColor::create(Color4B::WHITE, 640, 640);
    bg->setPosition(320, 0);
    addChild(bg, -2);

    // ui
    auto ui = GameUI::getInstance();
    ui->setAnchorPoint(Vec2(0, 0));
    ui->setPosition(0, 0);
    addChild(ui);

    // cannon
    auto cannon = Cannon::getInstance();
    cannon->setPosition(640, 64);
    cannon->updateUI();
    addChild(cannon);

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
    
    // spawner
    auto spawner = PenguinSpawner::create();
    addChild(spawner);
    spawner->setPosition(640, 704);
}

void CoolPenguin::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
    Cannon::getInstance()->onKeyPressed(keyCode, event);
}

void CoolPenguin::onMouseMove(EventMouse* event) {
    Cannon::getInstance()->onMouseMove(event);
}

bool CoolPenguin::onContactBegin(PhysicsContact& contact) {
    IContact* entityA = dynamic_cast<IContact*>(contact.getShapeA()->getBody()
        ->getOwner());

    IContact* entityB = dynamic_cast<IContact*>(contact.getShapeB()->getBody()
        ->getOwner());

    if (entityA) entityA->onContactBegin(contact);
    if (entityB) entityB->onContactBegin(contact);

    /* Always returns false because this game doesn't involve any physics 
    collisions at all, just contacts. */
    return false;
}

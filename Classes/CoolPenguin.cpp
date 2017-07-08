#include "CoolPenguin.h"
#include "HelperPenguin.h"
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
    CSLoader::getInstance()->registReaderObject("HelperPenguinReader",
        (ObjectFactory::Instance)TNodeReader<HelperPenguin>::getInstance);

    auto scene = Scene::createWithPhysics();
    auto layer = CoolPenguin::create();
    scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
//#ifdef _DEBUG
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
//#endif
    scene->addChild(layer);
    return scene;
}

CoolPenguin::CoolPenguin() : _paused(false) {}

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

    // spawner
    auto spawner = PenguinSpawner::create();
    addChild(spawner, -2);
    spawner->setPosition(640, 704);

    // cannon
    auto cannon = Cannon::getInstance();
    cannon->setPosition(640, 64);
    addChild(cannon);

    // helper penguin
    auto helperPenguin = HelperPenguin::getInstance();
    helperPenguin->setPosition(896, 64);
    addChild(helperPenguin);

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
}

void CoolPenguin::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
    Cannon::getInstance()->onKeyPressed(keyCode, event);

    // Pauses the game
    if (keyCode == EventKeyboard::KeyCode::KEY_SPACE) {
        if (!_paused) Director::getInstance()->stopAnimation();
        else Director::getInstance()->startAnimation();
        _paused = !_paused;
    }
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

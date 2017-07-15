#include "CoolPenguin.h"
#include "Cannon.h"
#include "HelperPenguin.h"
#include "HelpScreen.h"
#include "Penguin.h"
#include "PenguinSpawner.h"
#include "GameUI.h"
#include "TNodeReader.h"

using namespace cocos2d;
using namespace std;

Scene* CoolPenguin::createScene() {
    auto scene = Scene::createWithPhysics();
    auto layer = CoolPenguin::create();
    scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
    scene->addChild(layer);
    return scene;
}

CoolPenguin::CoolPenguin() : _paused(false) {}

void CoolPenguin::reset() {
    for (Node* child : getChildren()) {
        auto resettable = dynamic_cast<IReset*>(child);
        if (resettable) resettable->reset();
    }
}

bool CoolPenguin::init() {
    if (!Layer::init()) return false;

    // white background
    auto bg = LayerColor::create(Color4B::WHITE, 640, 640);
    bg->setPosition(320, 0);
    addChild(bg, -2);

    // backdrop
    auto backdrop = Sprite::create("img/backdrop.png");
    backdrop->setAnchorPoint(Vec2(0, 0));
    backdrop->setPosition(0, 0);
    addChild(backdrop, -2);

    // ui
    auto ui = GameUI::create();
    ui->setAnchorPoint(Vec2(0, 0));
    ui->setPosition(0, 0);
    addChild(ui, 1, "ui");

    // spawner
    auto spawner = PenguinSpawner::create();
    spawner->setPosition(640, 704);
    addChild(spawner, 0, "spawner");

    // cannon
    auto cannon = (Cannon*)CSLoader::createNode("csb/cannon.csb");
    cannon->setPosition(640, 64);
    addChild(cannon, 3, "cannon");

    // helper penguin
    auto helperPenguin = (HelperPenguin*)CSLoader::createNode(
        "csb/helperpenguin.csb");
    helperPenguin->setStartPosition(Vec2(896, 64));
    addChild(helperPenguin, 4, "helper");

    // mouse listener
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseMove = [cannon](EventMouse* event) {
        cannon->onMouseMove(event);
    };

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

    return true;
}

void CoolPenguin::onEnter() {
    Node::onEnter();
    reset();
}

void CoolPenguin::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
    getChildByName<Cannon*>("cannon")->onKeyPressed(keyCode, event);

    // Pauses the game
    if (keyCode == EventKeyboard::KeyCode::KEY_SPACE) {
        if (!_paused) Director::getInstance()->stopAnimation();
        else Director::getInstance()->startAnimation();
        _paused = !_paused;
    }

    else if (keyCode == EventKeyboard::KeyCode::KEY_6) reset();
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

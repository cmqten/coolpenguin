#include "Penguin.h"
#include <cstdint>
#include <random>
#include "GameUI.h"

using namespace cocos2d;
using namespace std;

Penguin::Penguin() : IAnimated("csb/penguin.csb") {
    /* Creates the speech bubbles to be set visible when a random request is
    generated */
    auto createSpeechBubble = [this](string path, string name) {
        auto speechBubble = Sprite::create(path);
        speechBubble->setVisible(false);
        speechBubble->setPosition(48, 64);
        this->addChild(speechBubble, 0, name);
    };
    createSpeechBubble("img/sp_fr.png", "sp_fr"); // regular fish
    createSpeechBubble("img/sp_fi.png", "sp_fi"); // ice cream covered fish
    createSpeechBubble("img/sp_icr.png", "sp_icr"); // regular ice cream
    createSpeechBubble("img/sp_icf.png", "sp_icf"); // fish flavored ice cream
}

void Penguin::closeRequest() {
    switch (_request) {
        case Projectile::ProjectileType::FISH:
            getChildByName("sp_fr")->setVisible(false);
            break;

        case Projectile::ProjectileType::FISHI:
            getChildByName("sp_fi")->setVisible(false);
            break;

        case Projectile::ProjectileType::ICECREAM:
            getChildByName("sp_icr")->setVisible(false);
            break;

        case Projectile::ProjectileType::ICECREAMF:
            getChildByName("sp_icf")->setVisible(false);
            break;
    }
}

void Penguin::generateRequest() {
    /* First 20 seconds of the game, only regular fish and regular ice cream 
    are generated */
    int choice = GameUI::getInstance()->getGameTime() >= 20 ? 3 : 1;
    switch (rand() & choice) {
        case 0:
            _request = Projectile::ProjectileType::FISH;
            getChildByName("sp_fr")->setVisible(true);
            break;

        case 1:
            _request = Projectile::ProjectileType::ICECREAM;
            getChildByName("sp_icr")->setVisible(true);
            break;

        case 2:
            _request = Projectile::ProjectileType::FISHI;
            getChildByName("sp_fi")->setVisible(true);
            break;

        case 3:
            _request = Projectile::ProjectileType::ICECREAMF;
            getChildByName("sp_icf")->setVisible(true);
            break;

        default: break;
    }
}

void Penguin::prepareForSpawn(int wait) {
    _waitTime = wait;
    _state = State::SPAWN;
}

void Penguin::waddleIn() {
    if (_state != State::SPAWN) return; // Can only waddle in after spawning

    auto waddleInMove = MoveBy::create(1.5f, Vec2(0, -160));
    auto waddleInFunc = CallFunc::create([this]() {
        /* Penguin waits for its request after waddling in, contact bitmask
        is set to projectile's category */        
        animate("idle", true, true);
        this->_state = State::RECV;
        this->generateRequest();
        this->schedule(SEL_SCHEDULE(&Penguin::waitForRequest), 1.0f);
        this->getPhysicsBody()->setContactTestBitmask(0x2);
    });

    // Sets state to waddle in and runs animations
    _state = State::WADDLEIN;
    animate("waddle", true, true);
    runAction(Sequence::create(waddleInMove, waddleInFunc, nullptr));
}

void Penguin::waddleOut() {
    if (_state != State::RECV) return; // Can only waddle out after receiving

    auto waddleOutMove = MoveBy::create(1.5f, Vec2(0, 160));
    auto waddleOutFunc = CallFunc::create([this]() {
        // Dispatches an event to tell listeners that penguin has returned
        this->_state = State::DESPAWN;
        this->_eventDispatcher->dispatchCustomEvent(PENGUIN_DONE, (void*)this);
    });

    /* Closes request by stopping contact detection, unscheduling request timer, 
    and setting the speech bubble invisible */
    this->getPhysicsBody()->setContactTestBitmask(0x0);
    unschedule(SEL_SCHEDULE(&Penguin::waitForRequest));
    closeRequest();

    // Sets state to waddle out and runs animations
    _state = State::WADDLEOUT;
    animate("waddle_back", true, true);
    runAction(Sequence::create(waddleOutMove, waddleOutFunc, nullptr));
}

void Penguin::waitForRequest(float delta) {
    _waitTime--;
    if (!_waitTime) waddleOut();
}

void Penguin::onEnter() {
    Node::onEnter();

    // Contact test is set later when penguin actually needs to detect contact
    setPhysicsBody([]()->PhysicsBody* {
        auto body = PhysicsBody::createBox(Size(36, 96));
        body->getShapes().at(0)->setSensor(true);
        body->setCategoryBitmask(0x4);
        body->setCollisionBitmask(0x0);
        body->setContactTestBitmask(0x0);
        return body;
    }());
}

bool Penguin::onContactBegin(cocos2d::PhysicsContact& contact) {
    if (_state != State::RECV) return false;
    // Detects projectile type and updates points accordingly
    Node* penguin = contact.getShapeA()->getBody()->getOwner();
    Node* proj = contact.getShapeB()->getBody()->getOwner();

    if (penguin != this) { // Swapping pointers
        penguin = (Node*)((uintptr_t)penguin ^ (uintptr_t)proj);
        proj = (Node*)((uintptr_t)penguin ^ (uintptr_t)proj);
        penguin = (Node*)((uintptr_t)penguin ^ (uintptr_t)proj);
    }

    // Calculates points and removes projectile from game
    auto points = ((Projectile*)proj)->getType() == _request ? 2 : -1;
    getEventDispatcher()->dispatchCustomEvent(UPDATE_SCORE, (void*)&points);
    proj->removeFromParentAndCleanup(true);
    waddleOut();
    return false;
}

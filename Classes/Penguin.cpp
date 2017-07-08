#include "Penguin.h"
#include <cstdint>
#include <random>
#include "GameUI.h"

using namespace cocos2d;
using namespace std;

Penguin::Penguin() : IAnimated("csb/penguin.csb") {
    /* Creates the speech bubbles to be set visible when a random request is
    generated */
    _speechBubbles = new Sprite*[4];

    auto createSpeechBubble = [this](string path)->Sprite* {
        auto speechBubble = Sprite::create(path);
        speechBubble->setPosition(48, 64);
        speechBubble->retain();
        return speechBubble;
    };

    _speechBubbles[0] = createSpeechBubble("img/sp_fr.png"); // regular fish

    // regular ice cream
    _speechBubbles[1] = createSpeechBubble("img/sp_icr.png");

    // ice cream covered fish
    _speechBubbles[2] = createSpeechBubble("img/sp_fi.png");

    // fish flavored ice cream
    _speechBubbles[3] = createSpeechBubble("img/sp_icf.png");
}

Penguin::~Penguin() {
    for (int i = 0; i < 4; i++) _speechBubbles[i]->release();
    delete _speechBubbles;
}

void Penguin::closeRequest() {
    getChildByName("spBubble")->removeFromParentAndCleanup(true);
}

void Penguin::generateRequest() {
    Projectile::ProjectileType projTypes[] = {
        Projectile::ProjectileType::FISH, 
        Projectile::ProjectileType::ICECREAM,
        Projectile::ProjectileType::FISHI,
        Projectile::ProjectileType::ICECREAMF
    };

    /* First 20 seconds of the game, only regular fish and regular ice cream
    are generated */
    int choice = rand() & (GameUI::getInstance()->getGameTime() >= 20 ? 3 : 1);
    _request = projTypes[choice];
    addChild(_speechBubbles[choice], 0, "spBubble");
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
    getPhysicsBody()->setContactTestBitmask(0x0);
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

    if (penguin != this) { // Swap pointers if required
        penguin = (Node*)((uintptr_t)penguin ^ (uintptr_t)proj);
        proj = (Node*)((uintptr_t)penguin ^ (uintptr_t)proj);
        penguin = (Node*)((uintptr_t)penguin ^ (uintptr_t)proj);
    }

    /* Calculates points and removes projectile from game. 2 for a correct shot,
    -1 for a wrong shot */ 
    auto points = ((Projectile*)proj)->getType() == _request ? 2 : -1;
    GameUI::getInstance()->updateScore(points);
    proj->removeFromParentAndCleanup(true);
    waddleOut();
    return false;
}

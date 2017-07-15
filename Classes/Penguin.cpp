#include "Penguin.h"
#include <cstdint>
#include <random>
#include "GameUI.h"

using namespace cocos2d;
using namespace std;

Penguin::Penguin() : IAnimated("csb/penguin.csb") {
    /* Creates the speech bubbles to be set visible when a random request is
    generated */
    _requestBubbles = new Sprite*[4];
    _reactionBubbles = new Sprite*[2];

    auto createSpeechBubble = [this](string path)->Sprite* {
        auto speechBubble = Sprite::createWithSpriteFrameName(path);
        speechBubble->setPosition(48, 64);
        speechBubble->retain();
        return speechBubble;
    };

    /* regular fish, regular ice cream, ice cream fish, and fish flavored ice
    cream in that specific order */
    _requestBubbles[0] = createSpeechBubble("img/sp_fr.png"); 
    _requestBubbles[1] = createSpeechBubble("img/sp_icr.png");
    _requestBubbles[2] = createSpeechBubble("img/sp_fi.png");
    _requestBubbles[3] = createSpeechBubble("img/sp_icf.png");

    // happy and sad reactions
    _reactionBubbles[0] = createSpeechBubble("img/sp_happy.png");
    _reactionBubbles[1] = createSpeechBubble("img/sp_sad.png");
}

Penguin::~Penguin() {
    for (int i = 0; i < 4; i++) _requestBubbles[i]->release();
    for (int i = 0; i < 2; i++) _reactionBubbles[i]->release();
    delete[] _requestBubbles;
    delete[] _reactionBubbles;
}

void Penguin::closeRequest() {
    getChildByName("requestBubble")->removeFromParentAndCleanup(true);
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
    int choice = rand() & (getParent()->getParent()->getChildByName<GameUI*>(
        "ui")->getGameTime() <= 100 ? 3 : 1);
    _request = projTypes[choice];
    addChild(_requestBubbles[choice], 0, "requestBubble");
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

void Penguin::waddleOut(bool received) {
    if (_state != State::RECV) return; // Can only waddle out after receiving

    auto waddleOutMove = MoveBy::create(1.5f, Vec2(0, 160));
    auto waddleOutFunc = CallFunc::create([this]() {
        // Dispatches an event to tell listeners that penguin has returned
        this->_state = State::DESPAWN;
        this->removeChildByName("reactionBubble");
        this->_eventDispatcher->dispatchCustomEvent(PENGUIN_DONE, (void*)this);
    });

    /* Closes request by stopping contact detection, unscheduling request timer, 
    and setting the speech bubble invisible */
    getPhysicsBody()->setContactTestBitmask(0x0);
    unschedule(SEL_SCHEDULE(&Penguin::waitForRequest));
    closeRequest();

    // Displays happy reaction if received, sad reaction otherwise
    if (received) addChild(_reactionBubbles[0], 0, "reactionBubble");
    else addChild(_reactionBubbles[1], 0, "reactionBubble");

    // Sets state to waddle out and runs animations
    _state = State::WADDLEOUT;
    animate("waddle_back", true, true);
    runAction(Sequence::create(waddleOutMove, waddleOutFunc, nullptr));
}

void Penguin::waitForRequest(float delta) {
    _waitTime--;
    if (!_waitTime) waddleOut(false);
}

void Penguin::reset() {
    stopAllActions();

    auto requestBubble = getChildByName("requestBubble");
    if (requestBubble) requestBubble->removeFromParent();

    auto reactionBubble = getChildByName("reactionBubble");
    if (reactionBubble) reactionBubble->removeFromParent();

    _state = State::SPAWN;
    unscheduleAllCallbacks();
}

bool Penguin::init() {
    if (!Node::init()) return false;

    // Contact test is set later when penguin actually needs to detect contact
    setPhysicsBody([]()->PhysicsBody* {
        auto body = PhysicsBody::createBox(Size(36, 96));
        body->getShapes().at(0)->setSensor(true);
        body->setCategoryBitmask(0x4);
        body->setCollisionBitmask(0x0);
        body->setContactTestBitmask(0x0);
        return body;
    }());

    return true;
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
    auto pts = ((Projectile*)proj)->getType() == _request ? 2 : -1;
    getParent()->getParent()->getChildByName<GameUI*>("ui")->updateScore(pts);
    proj->removeFromParentAndCleanup(true);
    waddleOut(pts == 2);
    return false;
}

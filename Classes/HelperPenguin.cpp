#include "HelperPenguin.h"
#include <cmath>
#include <cassert>

using namespace cocos2d;
using namespace std;

HelperPenguin* HelperPenguin::_instance = nullptr;

HelperPenguin* HelperPenguin::getInstance() {
    if (!_instance) _instance = (HelperPenguin*)CSLoader::createNode(
        "csb/helperpenguin.csb");
    return _instance;
}

HelperPenguin::HelperPenguin() : IAnimated("csb/helperpenguin.csb"),
    _state(State::IDLE) {}

void HelperPenguin::cleanCannon(Vec2 pos) {
    // Can't do anything if helper penguin is busy
    if (_state != State::IDLE) return;

    float time = fabs((pos.x - getPositionX()) / WALK_SPEED);
    auto helperWalkToCannon = MoveTo::create(time, pos);
    auto helperArrive = CallFunc::create([this] {
        /* Sets actual helper penguin invisble. Gives the illusion that the 
        penguin went inside the cannon without actually going inside. */
        this->setVisible(false);
        this->stopAllActions();
        this->getEventDispatcher()->dispatchCustomEvent(HELPER_EVENT,
            (void*)&this->_state);
    });

    // Helper walks towards the cannon
    _state = State::CLEAN;
    ((Sprite*)getChildByName("sprite"))->setFlippedX(true);
    animate("waddle_side_clean", true, true);
    runAction(Sequence::create(helperWalkToCannon, helperArrive, nullptr));
}

void HelperPenguin::gather(Vec2 pos) {
    // Can't do anything if helper penguin is busy
    if (_state != State::IDLE) return;

    /**
     * Sequence of actions: 
     * - Helper goes off screen to "gather" stuff
     * - Helper changes direction and walks back to the cannon
     * - Helper lets cannon know that stuff has been gathered
     * - Helper changes direction and walkd back to original position
     * - Helper is now idle
     */
    auto helperWalkOffScreen = MoveBy::create(680 / WALK_SPEED, Vec2(680, 0));

    auto helperFlip = CallFunc::create([this]() {
        auto sprite = (Sprite*)this->getChildByName("sprite");
        sprite->setFlippedX(!sprite->isFlippedX());
    });

    auto helperWalkToCannon = MoveTo::create((1576 - (pos.x)) / WALK_SPEED, 
        pos);

    auto helperReloadCannon = CallFunc::create([this]() {
        this->getEventDispatcher()->dispatchCustomEvent(HELPER_EVENT, 
            (void*)&this->_state);
    });

    auto helperWalkBack = MoveTo::create((896-pos.x)/WALK_SPEED, Vec2(896, 64));

    auto helperReturned = CallFunc::create([this] {
        /* Helper can take commands now */
        animate("idle", true, true);
        this->_state = State::IDLE;
    });

    _state = State::GATHER;
    animate("waddle_side", true, true);
    runAction(Sequence::create(helperWalkOffScreen, helperFlip, 
        helperWalkToCannon, helperReloadCannon, helperFlip, helperWalkBack, 
        helperReturned, nullptr));
}

void HelperPenguin::returnFromClean(cocos2d::Vec2 pos) {
    if (_state != State::CLEAN) return;

    float time = (896 - pos.x) / WALK_SPEED;
    auto helperWalkBack = MoveTo::create(time, Vec2(896, 64));
    auto helperReturned = CallFunc::create([this] {
        /* Helper can take commands now */
        animate("idle", true, true);
        this->_state = State::IDLE;
    });

    // Sets helper to visible again and waddles back
    setVisible(true);
    ((Sprite*)getChildByName("sprite"))->setFlippedX(false);
    animate("waddle_side_clean", true, true);
    runAction(Sequence::create(helperWalkBack, helperReturned, nullptr));
}

void HelperPenguin::onEnter() {
    Node::onEnter();
    animate("idle", true, true);
}

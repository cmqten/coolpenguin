#include "Cannon.h"
#include <cstdlib>
#include "GameUI.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;
using namespace std;

Cannon::Cannon() : IAnimated("csb/cannon.csb"), _enabled(true), 
    _fishReserve(10), _fishShotCount(0), _iceCreamReserve(10), 
    _iceCreamShotCount(0) {}

void Cannon::updateUI() {
    CannonStats cannonStats = { _fishShotCount, _iceCreamShotCount,
        _fishReserve, _iceCreamReserve };
    _eventDispatcher->dispatchCustomEvent(UPDATE_CANNON, (void*)&cannonStats);
}

bool Cannon::init() {
    if (!Node::init()) return false;
    SimpleAudioEngine::getInstance()->preloadEffect("sfx/cannon_shoot.wav");

    _timeline->setAnimationEndCallFunc("shoot", [this]() {
        /* Enables cannon after shoot animation is done, prevents rapid fire. 
        If more than 10 of the some projectile has been shot, cannon gets 
        clogged. */
        _enabled = abs(_fishShotCount - _iceCreamShotCount) < 10;
    });

    return true;
}

void Cannon::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
    switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_J:
        case EventKeyboard::KeyCode::KEY_A:
            shoot(Projectile::ProjectileType::FISH);
            break;

        case EventKeyboard::KeyCode::KEY_K:
        case EventKeyboard::KeyCode::KEY_S:
            shoot(Projectile::ProjectileType::ICECREAM);
            break;

        case EventKeyboard::KeyCode::KEY_D:
        case EventKeyboard::KeyCode::KEY_L:
            clean();
            break;

        default: break;
    }
}

void Cannon::onMouseMove(EventMouse* event) {
    rotate(event->getCursorX(), event->getCursorY());
}

void Cannon::clean() {
    _fishShotCount = 0;
    _iceCreamShotCount = 0;
    _enabled = true;
    updateUI();
}

void Cannon::rotate(float x, float y) {
    /* Trigonometry magic to determine angle bless MAT223 
    (and stack overflow). */
    float cursorVecX = x - getPositionX();
    float cursorVecY = y - getPositionY();
    setRotation(CC_RADIANS_TO_DEGREES(atan2f(cursorVecX, cursorVecY)));
}

void Cannon::shoot(Projectile::ProjectileType projType) {
    /**
     * Shoot mechanics:
     * - Can specify whether to shoot a regular fish or regular ice cream
     * - If more ice cream have been shot, cannon is "ice cream dirty", so all
     * regular fish become ice cream covered fish
     * - If more fish have been shot, cannon is "fish dirty", so all ice cream
     * become fish flavored ice cream
     * - If equal amounts of fish and ice cream have been shot, cannon is clean
     * - Can only specify to shoot a regular fish or regular ice cream
     */
    if (!_enabled) return;

    Projectile* proj = nullptr;
    
    switch (projType) {
        /**
         * - Check if the reserve for the specified projectile is not empty
         * - Check which kind of projectile have been shot more, then determine
         *   what kind of projectile comes out of the cannon 
         * - Decrement the reserve after shooting
         */
        case Projectile::ProjectileType::FISH:
            if (!_fishReserve) return;
            proj = _fishShotCount >= _iceCreamShotCount ? 
                Projectile::create(projType) :
                Projectile::create(Projectile::ProjectileType::FISHI);
            _fishShotCount++;
            _fishReserve--;
            break;

        case Projectile::ProjectileType::ICECREAM:
            if (!_iceCreamReserve) return;
            proj = _iceCreamShotCount >= _fishShotCount ? 
                Projectile::create(projType) :
                Projectile::create(Projectile::ProjectileType::ICECREAMF);
            _iceCreamShotCount++;
            _iceCreamReserve--;
            break;

        case Projectile::ProjectileType::FISHI:
            if (!_fishReserve) return;
            proj = Projectile::create(projType);
            _fishShotCount++;
            _fishReserve--;
            break;

        case Projectile::ProjectileType::ICECREAMF:
            if (!_iceCreamReserve) return;
            proj = Projectile::create(projType);
            _iceCreamShotCount++;
            _fishReserve--;
            break;
    }

    if (!proj) return;

    // Shoots projectile
    proj->setPosition(getPositionX(), getPositionY());
    proj->setRotation(getRotation());
    proj->launch(90 - getRotation());
    this->getParent()->addChild(proj, -1);

    // Cannon effects
    SimpleAudioEngine::getInstance()->playEffect("sfx/cannon_shoot.wav");
    animate("shoot", false, true);
    _enabled = false; // Prevents rapid fire, enabled after animation

    // Update UI with current cannon stats
    updateUI();
}

#include "Cannon.h"
#include "SimpleAudioEngine.h"
#include "StatsUI.h"

using namespace cocos2d;
using namespace CocosDenshion;
using namespace std;

Cannon::Cannon() : IAnimated("csb/cannon.csb"), _enabled(true), _fishCount(0),
    _iceCreamCount(0) {}

bool Cannon::init() {
    if (!Node::init()) return false;
    SimpleAudioEngine::getInstance()->preloadEffect("sfx/cannon_shoot.wav");

    _timeline->setAnimationEndCallFunc("shoot", [this]() {
        // Enables cannon after shoot animation is done, prevents rapid fire
        _enabled = true;
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

        default:
            break;
    }
}

void Cannon::onMouseMove(EventMouse* event) {
    rotate(event->getCursorX(), event->getCursorY());
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

    Projectile* proj;
    
    switch (projType) {
        case Projectile::ProjectileType::FISH:
            proj = _fishCount >= _iceCreamCount ? Projectile::create(projType) :
                Projectile::create(Projectile::ProjectileType::FISHI);
            _fishCount++;
            break;

        case Projectile::ProjectileType::ICECREAM:
            proj = _iceCreamCount >= _fishCount ? Projectile::create(projType) :
                Projectile::create(Projectile::ProjectileType::ICECREAMF);
            _iceCreamCount++;
            break;

        case Projectile::ProjectileType::FISHI:
            proj = Projectile::create(projType);
            _fishCount++;
            break;

        case Projectile::ProjectileType::ICECREAMF:
            proj = Projectile::create(projType);
            _iceCreamCount++;
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

    // Update stats
    GameStats* stats = new GameStats();
    stats->fishCount = _fishCount;
    stats->iceCreamCount = _iceCreamCount;
    getEventDispatcher()->dispatchCustomEvent(UPDATE_STATS, stats);
}

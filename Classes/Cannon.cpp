#include "Cannon.h"
#include "SimpleAudioEngine.h"
#include "StatsUI.h"

using namespace cocos2d;
using namespace CocosDenshion;
using namespace std;

Cannon::Cannon() : IAnimated("csb/cannon.csb"), fishCount(0), iceCreamCount(0), 
    _enabled(true) {}

bool Cannon::init() {
    if (!Node::init()) return false;
    SimpleAudioEngine::getInstance()->preloadEffect("sfx/cannon_shoot.wav");

    this->timeline->setAnimationEndCallFunc("shoot", [this]() {
        // Enables cannon after shoot animation is done, prevents rapid fire
        this->enableCannon();
    });
    return true;
}

void Cannon::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
    switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_J:
        case EventKeyboard::KeyCode::KEY_A:
            this->shoot(Projectile::ProjectileType::FISH);
            break;

        case EventKeyboard::KeyCode::KEY_K:
        case EventKeyboard::KeyCode::KEY_S:
            this->shoot(Projectile::ProjectileType::ICECREAM);
            break;

        default:
            break;
    }
}

void Cannon::onMouseMove(EventMouse* event) {
    this->rotate(event->getCursorX(), event->getCursorY());
}

void Cannon::rotate(float x, float y) {
    /* Trigonometry magic to determine angle bless MAT223 
    (and stack overflow). */
    float cursorVecX = x - this->getPositionX();
    float cursorVecY = y - this->getPositionY();
    this->setRotation(CC_RADIANS_TO_DEGREES(atan2f(cursorVecX, cursorVecY)));
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
            proj = fishCount >= iceCreamCount ? Projectile::create(projType) :
                Projectile::create(Projectile::ProjectileType::FISHI);
            this->fishCount++;
            break;

        case Projectile::ProjectileType::ICECREAM:
            proj = iceCreamCount >= fishCount ? Projectile::create(projType) :
                Projectile::create(Projectile::ProjectileType::ICECREAMF);
            this->iceCreamCount++;
            break;

        case Projectile::ProjectileType::FISHI:
            proj = Projectile::create(projType);
            this->fishCount++;
            break;

        case Projectile::ProjectileType::ICECREAMF:
            proj = Projectile::create(projType);
            this->iceCreamCount++;
            break;
    }

    if (!proj) return;

    // Shoots projectile
    proj->setPosition(this->getPositionX(), this->getPositionY());
    proj->setRotation(this->getRotation());
    proj->launch(90 - this->getRotation());
    this->getParent()->addChild(proj, -1);

    // Cannon effects
    SimpleAudioEngine::getInstance()->playEffect("sfx/cannon_shoot.wav");
    animate("shoot", false, true);
    this->disableCannon(); // Prevents rapid fire, enabled after animation

    // Update stats
    GameStats* stats = new GameStats();
    stats->fishCount = this->fishCount;
    stats->iceCreamCount = this->iceCreamCount;
    this->getEventDispatcher()->dispatchCustomEvent(UPDATE_STATS, stats);
}

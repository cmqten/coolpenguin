#include "Cannon.h"
#include <cmath>
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;
using namespace std;

Cannon::Cannon() : GameEntity("csb/cannon.csb"), fishCount(0), 
    iceCreamCount(0), _enabled(true) {}

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
    this->setRotation(DEG(atan2f(cursorVecX, cursorVecY)));
}

void Cannon::shoot(Projectile::ProjectileType projType) {
    if (!_enabled) return;
    Projectile* proj;
    switch (projType) {
        case Projectile::ProjectileType::FISH:
#ifdef _DEBUG
            CCLOG("fish");
#endif // _DEBUG
            proj = Projectile::createFish();
            this->fishCount++;
            break;

        case Projectile::ProjectileType::ICECREAM:
#ifdef _DEBUG
            CCLOG("ice cream");
#endif // _DEBUG
            proj = Projectile::createIceCream();
            this->iceCreamCount++;
            break;

        default:
#ifdef _DEBUG
            CCLOG("null");
#endif // _DEBUG
            proj = nullptr;
            break;
    }
    if (!proj) return;
    proj->setPosition(
        60 * cosf(RAD(90 - this->getRotation())) + this->getPositionX(),
        60 * sinf(RAD(90 - this->getRotation())) + this->getPositionY());
    proj->setRotation(this->getRotation());
    this->getParent()->addChild(proj, -1);
    proj->launch(90 - this->getRotation());
    SimpleAudioEngine::getInstance()->playEffect("sfx/cannon_shoot.wav");
    this->animate("shoot", false);
    this->disableCannon(); // Prevents rapid fire, enabled after animation
}

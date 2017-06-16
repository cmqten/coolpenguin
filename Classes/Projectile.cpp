#include "Projectile.h"
#include <cmath>

using namespace cocos2d;
using namespace std;

Projectile* Projectile::createFish() {
    auto proj = new (nothrow) Projectile("img/fish_r.png",
        []()->PhysicsBody* {
        /* Lambda so more details to the PhysicsBody can be added later */
        return PhysicsBody::createBox(Size(12, 12));
    }(), FISH);

    if (!proj || !proj->init()) {
        CC_SAFE_DELETE(proj);
        return nullptr;
    }
    proj->autorelease();
    return proj;
}

Projectile* Projectile::createIceCream() {
    auto proj = new (nothrow) Projectile("img/ice_cream_r.png", 
        []()->PhysicsBody* { 
            /* Lambda so more details to the PhysicsBody can be added later */
            return PhysicsBody::createBox(Size(12, 12));
        }(), ICECREAM);

    if (!proj || !proj->init()) {
        CC_SAFE_DELETE(proj);
        return nullptr;
    }
    proj->autorelease();
    return proj;
}

Projectile::Projectile(string path, PhysicsBody* pBody, ProjectileType type,
    float velocity) : GameEntity(pBody), _velocity(velocity), _type(type) {
    if (path != "") { // Creates projectile's sprite and adds it as a child
        this->_sprite = Sprite::create(path);
        this->addChild(this->_sprite);
    }
    else this->_sprite = nullptr;
}

void Projectile::launch(float angle) const {
    /* To launch the projectile at the specified velocity and angle, the 
    x and y components are calculated using math magic */
    this->getPhysicsBody()->setVelocity(Vec2(_velocity * cosf(RAD(angle)), 
        _velocity * sinf(RAD(angle))));
}

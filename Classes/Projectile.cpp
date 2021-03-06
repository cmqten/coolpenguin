#include "Projectile.h"

using namespace cocos2d;
using namespace std;

Projectile* Projectile::create(ProjectileType projType) {
    auto pBody = []()->PhysicsBody* { // Physics body for collision dectection
        auto body = PhysicsBody::createBox(Size(12, 12));
        body->getShapes().at(0)->setSensor(true);
        body->setCategoryBitmask(0x2);
        body->setCollisionBitmask(0x0);
        body->setContactTestBitmask(0x4);
        return body;
    }();

    Projectile* proj = nullptr;

    switch (projType) {
        case ProjectileType::FISH: // regular fish
            proj = new (nothrow) Projectile("img/fish_r.png", pBody,
                ProjectileType::FISH);
            break;

        case ProjectileType::FISHI: // ice-cream covered fish
            proj = new (nothrow) Projectile("img/fish_i.png", pBody, 
                ProjectileType::FISHI);
            break;

        case ProjectileType::ICECREAM: // regular ice cream
            proj = new (nothrow) Projectile("img/ice_cream_r.png", pBody,
                ProjectileType::ICECREAM);
            break;

        case ProjectileType::ICECREAMF: // fish-flavored ice cream
            proj = new (nothrow) Projectile("img/ice_cream_f.png", pBody,
                ProjectileType::ICECREAMF);
            break;
    }

    if (proj && proj->init()) {
        proj->autorelease();
        return proj;
    }
    CC_SAFE_DELETE(proj);
    return nullptr;
}

Projectile::Projectile(string path, PhysicsBody* pBody, ProjectileType type,
    float velocity) : _velocity(velocity), _type(type) {
    if (path != "") { // Creates projectile's sprite and adds it as a child
        _sprite = Sprite::createWithSpriteFrameName(path);
        addChild(_sprite);
    }
    else _sprite = nullptr;
    setPhysicsBody(pBody);
}

void Projectile::launch(float rotation, Vec2 pos) {
    /* To launch the projectile at the specified velocity and angle, the 
    x and y components are calculated using math magic */
    setRotation(rotation);
    setPosition(pos);
    float xComp = _velocity * cosf(CC_DEGREES_TO_RADIANS(90 - rotation));
    float yComp = _velocity * sinf(CC_DEGREES_TO_RADIANS(90 - rotation));
    getPhysicsBody()->setVelocity(Vec2(xComp, yComp));
}

void Projectile::reset() {
    getPhysicsBody()->setContactTestBitmask(0x0);
    setVisible(false);
}

bool Projectile::init() {
    if (!Node::init()) return false;
    retain();

    // Periodically checks if the projectile is out of bounds and deletes it
    unscheduleUpdate();
    schedule(schedule_selector(Projectile::update), 1.0f);

    return true;
}

void Projectile::update(float delta) {
    // Does an out of bounds check and destroys this Node
    Node::update(delta);
    float x = getPositionX();
    float y = getPositionY();
    float sceneX = getScene()->getContentSize().width;
    float sceneY = getScene()->getContentSize().height;
    
    if (x > sceneX || x < 0 || y > sceneY || y < 0) {
        release();
        removeFromParentAndCleanup(true);
    }  
}

bool Projectile::onContactBegin(cocos2d::PhysicsContact& contact) {
    release();
    return false;
}

#include "Penguin.h"
#include <random>

using namespace cocos2d;
using namespace std;

Penguin::Penguin() : _state(State::SPAWN) {
    switch (rand() & 3) {
        case 0: _request = Projectile::ProjectileType::FISH; break;
        case 1: _request = Projectile::ProjectileType::FISHI; break;
        case 2: _request = Projectile::ProjectileType::ICECREAM; break;
        case 3: _request = Projectile::ProjectileType::ICECREAMF; break;
        default: _request = Projectile::ProjectileType::FISH; break;
    }
}

void Penguin::waddleIn() {
    // Penguin waddles in the current state if SPAWN, then sets state to RECV
    // once the penguin finishes waddling in
    if (_state != State::SPAWN) return;

    _state = State::WADDLEIN;
    auto waddleInMove = MoveBy::create(2, Vec2(0, -100));
    auto waddleInFunc = CallFunc::create([this]() {
        this->_state = State::RECV;
    });
    runAction(Sequence::create(waddleInMove, waddleInFunc, nullptr));
}

void Penguin::waddleOut() {
    // Penguin waddles out if if the penguin is currently receiving an item, 
    // then sets state to DESPAWN once the penguin finishes waddling out
    if (_state != State::RECV) return;

    _state = State::WADDLEOUT;
    auto waddleOutMove = MoveBy::create(2, Vec2(0, 100));
    auto waddleOutFunc = CallFunc::create([this]() {
        this->_state = State::DESPAWN;
    });
    this->runAction(Sequence::create(waddleOutMove, waddleOutFunc, nullptr));
}

void Penguin::onEnter() {
    Node::onEnter();

    setPhysicsBody([]()->PhysicsBody* {
        auto body = PhysicsBody::createBox(Size(36, 96));
        body->getShapes().at(0)->setSensor(true);
        body->setCategoryBitmask(0x4);
        body->setCollisionBitmask(0x0);
        body->setContactTestBitmask(0x2);
        return body;
    }());

    _sprite = Sprite::create("img/penguin_glasses.png");
    addChild(_sprite);
    retain();
}

bool Penguin::onContactBegin(cocos2d::PhysicsContact& contact) {
    if (_state != State::RECV) return false;
    waddleOut();
    return false;
}

#include "Penguin.h"
#include <random>

using namespace cocos2d;
using namespace std;

Penguin::Penguin() : IAnimated("csb/penguin.csb"), _state(State::SPAWN) {
    Sprite* spBubble = nullptr;

    switch (rand() & 3) {
        case 0: 
            _request = Projectile::ProjectileType::FISH; 
            spBubble = Sprite::create("img/sp_fr.png");
            break;

        case 1: 
            _request = Projectile::ProjectileType::FISHI; 
            spBubble = Sprite::create("img/sp_fi.png");
            break;

        case 2: 
            _request = Projectile::ProjectileType::ICECREAM;
            spBubble = Sprite::create("img/sp_icr.png");
            break;

        case 3: 
            _request = Projectile::ProjectileType::ICECREAMF; 
            spBubble = Sprite::create("img/sp_icf.png");
            break;

        default: 
            _request = Projectile::ProjectileType::FISH; 
            spBubble = Sprite::create("img/sp_fr.png");
            break;
    }

    spBubble->setVisible(false);
    spBubble->setPosition(48, 64);
    addChild(spBubble, 0, "request");
}

void Penguin::waddleIn() {
    // Penguin waddles in the current state if SPAWN, then sets state to RECV
    // once the penguin finishes waddling in
    if (_state != State::SPAWN) return;

    _state = State::WADDLEIN;
    auto waddleInMove = MoveBy::create(2, Vec2(0, -208));
    auto waddleInFunc = CallFunc::create([this]() {
        animate("idle", true, true);
        this->_state = State::RECV;
        this->getChildByName("request")->setVisible(true);
    });
    animate("waddle", true, true);
    runAction(Sequence::create(waddleInMove, waddleInFunc, nullptr));
}

void Penguin::waddleOut() {
    // Penguin waddles out if if the penguin is currently receiving an item, 
    // then sets state to DESPAWN once the penguin finishes waddling out
    if (_state != State::RECV) return;

    _state = State::WADDLEOUT;
    auto waddleOutMove = MoveBy::create(2, Vec2(0, 208));
    auto waddleOutFunc = CallFunc::create([this]() {
        this->_state = State::DESPAWN;
    });
    getChildByName("request")->setVisible(false);
    animate("waddle_back", true, true);
    runAction(Sequence::create(waddleOutMove, waddleOutFunc, nullptr));
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
}

bool Penguin::onContactBegin(cocos2d::PhysicsContact& contact) {
    if (_state != State::RECV) return false;
    waddleOut();
    return false;
}

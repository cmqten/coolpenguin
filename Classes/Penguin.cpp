#include "Penguin.h"
#include <random>

using namespace cocos2d;
using namespace std;

Penguin::Penguin() : IAnimated("csb/penguin.csb"), _state(State::SPAWN) {
    /* Creates the speech bubbles to be set visible when a random request is
    generated */
    auto createSpeechBubble = [this](string path, string name) {
        auto speechBubble = Sprite::create(path);
        speechBubble->setVisible(false);
        speechBubble->setPosition(48, 64);
        this->addChild(speechBubble, 0, name);
    };
    createSpeechBubble("img/sp_fr.png", "sp_fr"); // regular fish
    createSpeechBubble("img/sp_fi.png", "sp_fi"); // ice cream covered fish
    createSpeechBubble("img/sp_icr.png", "sp_icr"); // regular ice cream
    createSpeechBubble("img/sp_icf.png", "sp_icf"); // fish flavored ice cream
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
        this->generateRequest();
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

    closeRequest();
    animate("waddle_back", true, true);
    runAction(Sequence::create(waddleOutMove, waddleOutFunc, nullptr));
}

void Penguin::generateRequest() {
    switch (rand() & 3) {
        case 0: 
            _request = Projectile::ProjectileType::FISH;
            getChildByName("sp_fr")->setVisible(true);
            break;

        case 1:
            _request = Projectile::ProjectileType::FISHI;
            getChildByName("sp_fi")->setVisible(true);
            break;

        case 2:
            _request = Projectile::ProjectileType::ICECREAM;
            getChildByName("sp_icr")->setVisible(true);
            break;

        case 3:
            _request = Projectile::ProjectileType::ICECREAMF;
            getChildByName("sp_icf")->setVisible(true);
            break;
    }
}

void Penguin::closeRequest() {
    switch (_request) {
        case Projectile::ProjectileType::FISH:
            getChildByName("sp_fr")->setVisible(false);
            break;

        case Projectile::ProjectileType::FISHI:
            getChildByName("sp_fi")->setVisible(false);
            break;

        case Projectile::ProjectileType::ICECREAM:
            getChildByName("sp_icr")->setVisible(false);
            break;

        case Projectile::ProjectileType::ICECREAMF:
            getChildByName("sp_icf")->setVisible(false);
            break;
    }
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

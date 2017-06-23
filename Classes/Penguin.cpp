#include "Penguin.h"
#include <chrono>
#include <random>
#include <thread>

using namespace cocos2d;
using namespace std;

Penguin::Penguin() : _state(State::SPAWN) {
    int gen = rand() % 4;
    CCLOG("%d", gen);
    switch (rand() % 4) {
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
    if (this->_state != State::SPAWN) return;
    this->_state = State::WADDLEIN;
    auto waddleInMove = MoveBy::create(2, Vec2(0, -100));
    auto waddleInFunc = CallFunc::create([this]() {
        this->_state = State::RECV;
    });
    this->runAction(Sequence::create(waddleInMove, waddleInFunc, nullptr));
}

void Penguin::waddleOut() {
    // Penguin waddles out if if the penguin is currently receiving an item, 
    // then sets state to DESPAWN once the penguin finishes waddling out
    if (this->_state != State::RECV) return;
    this->_state = State::WADDLEOUT;
    auto waddleOutMove = MoveBy::create(2, Vec2(0, 100));
    auto waddleOutFunc = CallFunc::create([this]() {
        this->_state = State::DESPAWN;
    });
    this->runAction(Sequence::create(waddleOutMove, waddleOutFunc, nullptr));
}

void Penguin::actionDispatcher() {
    int sec = 0;
    auto scheduler = Director::getInstance()->getScheduler();

    // Waddles the penguin in
    scheduler->performFunctionInCocosThread([this]() {
        this->waddleIn();
    });

    // Waits for the penguin to finish waddling in, then starts the timer
    while (this->_state != State::RECV);
    while (sec < 10) {
        this_thread::sleep_for(chrono::seconds(1));
        sec++;
        if (this->_state != State::RECV) break;
    }

    // The main thread may not have called waddleOut() when the timer runs
    // out because the penguin never received an item, so this thread
    // dispatches a waddleOut()
    if (this->_state == State::RECV) {
        scheduler->performFunctionInCocosThread([this]() {
            this->waddleOut();
        });
    }

    // Waits for the penguin to finish waddling out, then deletes it
    while (this->_state != State::DESPAWN);
    scheduler->performFunctionInCocosThread([this]() {
        this->removeFromParentAndCleanup(true);
        this->release();
    });
}

void Penguin::onEnter() {
    Node::onEnter();

    this->setPhysicsBody([]()->PhysicsBody* {
        auto body = PhysicsBody::createBox(Size(36, 96));
        body->getShapes().at(0)->setSensor(true);
        body->setCategoryBitmask(0x4);
        body->setCollisionBitmask(0x0);
        body->setContactTestBitmask(0x2);
        return body;
    }());

    this->_sprite = Sprite::create("img/penguin_glasses.png");
    this->addChild(_sprite);
    this->retain();

    // This thread dispatches actions to the main thread. Separation required
    // because this contains a timer.
    thread threadActionDispatcher(&Penguin::actionDispatcher, this);
    threadActionDispatcher.detach();
}

bool Penguin::onContactBegin(cocos2d::PhysicsContact& contact) {
    if (this->_state != State::RECV) return false;
    this->waddleOut();
    return false;
}

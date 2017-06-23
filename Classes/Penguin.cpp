#include "Penguin.h"
#include <chrono>
#include <random>
#include <thread>

using namespace cocos2d;
using namespace std;

Penguin::Penguin() : _receiving(false), _returned(false) {
    int gen = rand() % 4;
    CCLOG("%d", gen);
    switch (rand() % 4) {
        case 0: _request = Projectile::ProjectileType::FISH; break;
        case 1: _request = Projectile::ProjectileType::FISHI; break;
        case 2: _request = Projectile::ProjectileType::ICECREAM; break;
        case 3: _request = Projectile::ProjectileType::ICECREAMF; break;
        default: _request = Projectile::ProjectileType::FISH; break;
    }

    this->setPhysicsBody([]()->PhysicsBody* {
        auto body = PhysicsBody::createBox(Size(36, 96));
        body->getFirstShape()->setSensor(true);
        body->setCategoryBitmask(0x4);
        body->setCollisionBitmask(0x0);
        body->setContactTestBitmask(0x2);
        return body;
    }());
}

void Penguin::onEnter() {
    Node::onEnter();

    this->_sprite = Sprite::create("img/penguin_glasses.png");
    this->addChild(_sprite);

    this->retain();

    auto waddleIn = MoveBy::create(2, Vec2(0, -50));
    auto waddleInCallFunc = CallFunc::create([this](){
        thread timer([this](){
            this->_receiving = true;
            CCLOG("timer start");
            int sec = 0;
            while (sec < 10) {
                this_thread::sleep_for(chrono::seconds(1));
                sec++;
                if (!this->_receiving) break;
            }
            while (!this->_returned);
            this->removeFromParentAndCleanup(true);
            this->release();
            CCLOG("thread exit");
        });
        timer.detach();
    });
    this->runAction(Sequence::create(waddleIn, waddleInCallFunc, nullptr));
}

bool Penguin::onContactBegin(cocos2d::PhysicsContact& contact) {
    if (!_receiving) return false;
    this->_receiving = false;
    this->stopAllActions();
    auto waddleOut = MoveBy::create(2, Vec2(0, 50));
    auto waddleOutCallFunc = CallFunc::create([this](){
        this->_returned = true;
    });
    this->runAction(Sequence::create(waddleOut, waddleOutCallFunc, nullptr));
    return false;
}

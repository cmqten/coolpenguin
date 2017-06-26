#ifndef COOLPENGUIN_PENGUIN_H
#define COOLPENGUIN_PENGUIN_H

#include "cocos2d.h"
#include "IPhysics.h"
#include "Projectile.h"

class Penguin : public cocos2d::Node, public IPhysics {
public:
    enum class State {SPAWN, WADDLEIN, RECV, WADDLEOUT, DESPAWN};

    Penguin();

    CREATE_FUNC(Penguin);

    /* Plays waddle in animation sequence and calls any callbacks */
    void waddleIn();

    /* Plays waddle out animation sequence and calls any callbacks */
    void waddleOut();

    virtual void onEnter() override;

    virtual bool onContactBegin(cocos2d::PhysicsContact& contact) override;

    friend class PenguinSpawner;

private:
    cocos2d::Sprite* _sprite;

    Projectile::ProjectileType _request;
    
    State _state;
};

#endif //COOLPENGUIN_PENGUIN_H

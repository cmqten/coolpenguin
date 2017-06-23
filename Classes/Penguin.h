#ifndef COOLPENGUIN_PENGUIN_H
#define COOLPENGUIN_PENGUIN_H

#include "GameEntity.h"
#include "IPhysics.h"
#include "Projectile.h"

class Penguin : public GameEntity, public IPhysics {
public:
    Penguin();

    CREATE_FUNC(Penguin);

    virtual void onEnter() override;

    /* Called when two objects begin contact */
    virtual bool onContactBegin(cocos2d::PhysicsContact& contact);

    /* Called after two objects begin contact and before processing collision */
    virtual bool onContactPreSolve(cocos2d::PhysicsContact& contact,
        cocos2d::PhysicsContactPreSolve& solve) { return false; };

    /**
     * Called after processing collision, and for as long as the two objects are
     * colliding
     */
    virtual void onContactPostSolve(cocos2d::PhysicsContact& contact,
        const cocos2d::PhysicsContactPostSolve& solve) {};

    /* Called after two objects separate */
    virtual void onContactSeparate(cocos2d::PhysicsContact& contact) {};

private:
    cocos2d::Sprite* _sprite;

    Projectile::ProjectileType _request;

    bool _receiving;

    bool _returned;
};

#endif //COOLPENGUIN_PENGUIN_H

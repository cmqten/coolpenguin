#ifndef __PROJECTILE_H__
#define __PROJECTILE_H__

#include "GameEntity.h"

#define RAD(deg) (deg) * M_PI / 180
#define DEG(rad) (rad) * 180 / M_PI

class Projectile : public GameEntity {
public:
    static enum ProjectileType {FISH, ICECREAM};

    /* Creates a fish projectile */
    static Projectile* createFish();

    /* Creates an ice cream projectile */
    static Projectile* createIceCream();

    /** 
    Launches this projectile towards the specified angle
    @param angle : angle in degrees
    */
    void launch(float angle) const;

    virtual void updateEntity(float delta) override {};

protected:
    /**
    Constructor for Projectile class
    @param path : path to sprite image
    @param pBody : this projectile's physics body
    @param type : projectile type defined in the ProjectileType enum
    @param veloctty : this projectile's velocity after launch
    */
    Projectile(std::string path, cocos2d::PhysicsBody* pBody,
        ProjectileType type, float velocity = 1000);
     
private:
    /* Sprite for this projectile */
    cocos2d::Sprite* _sprite;

    /**
    Stores the type of this projectile. More type can be added later in the
    enumerator.
    */
    Projectile::ProjectileType _type;

    /**
    Launch velocity. Same regardless of the launch angle with the help of 
    linear algebra magic.
    */
    const float _velocity;
};

#endif // !__PROJECTILE_H__

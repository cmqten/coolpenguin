#ifndef __PROJECTILE_H__
#define __PROJECTILE_H__

#include "GameEntity.h"
#include "IPhysics.h"

class Projectile : public GameEntity, public IPhysics {
public:
    enum class ProjectileType {FISH, FISHI, ICECREAM, ICECREAMF};

    /* Creates a fish projectile */
    static Projectile* createFish();

    /* Creates a fish covered with ice cream projectile */
    static Projectile* createFishI();

    /* Creates an ice cream projectile */
    static Projectile* createIceCream();

    /* Creates an fish flavored ice cream projectile */
    static Projectile* createIceCreamF();

    /**
     * Launches this projectile towards the specified angle
     * @param angle : angle in degrees
     */
    void launch(float angle) const;

    virtual bool onContactBegin(cocos2d::PhysicsContact& contact);

    virtual bool onContactPreSolve(cocos2d::PhysicsContact& contact,
        cocos2d::PhysicsContactPreSolve& solve) { return false; };

    virtual void onContactPostSolve(cocos2d::PhysicsContact& contact,
        const cocos2d::PhysicsContactPostSolve& solve) {};

    virtual void onContactSeparate(cocos2d::PhysicsContact& contact) {};

protected:
    /**
     * Constructor for Projectile class
     * @param path : path to sprite image
     * @param pBody : this projectile's physics body
     * @param type : projectile type defined in the ProjectileType enum
     * @param velocity : this projectile's velocity after launch
     */
    Projectile(std::string path, cocos2d::PhysicsBody* pBody,
        ProjectileType type, float velocity = 1000);
     
private:
    /* Sprite for this projectile */
    cocos2d::Sprite* _sprite;

    /**
     * Stores the type of this projectile. More type can be added later in the
     * enumerator.
     */
    Projectile::ProjectileType _type;

    /**
     * Launch velocity. Same regardless of the launch angle with the help of
     * linear algebra magic.
     */
    const float _velocity;
};

#endif // !__PROJECTILE_H__

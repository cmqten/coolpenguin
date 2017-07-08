#ifndef __PROJECTILE_H__
#define __PROJECTILE_H__

#include "cocos2d.h"
#include "IContact.h"

#define DEFAULT_PROJ_VELOCITY 1000.0f

/**
 * Cannon projectile
 */
class Projectile : public cocos2d::Node, public IContact {
public:
    enum class ProjectileType {FISH, FISHI, ICECREAM, ICECREAMF};

    /**
     * Creates a projectile of the specified type
     * @param projType : projectile type, definied in the ProjectileType enum
     */
    static Projectile* create(ProjectileType projType);

    /**
     * Launches this projectile towards the specified angle
     * @param rotation : rotation angle
     * @param pos : start position
     */
    void launch(float rotation, cocos2d::Vec2 pos);

    ProjectileType getType() { return _type; };

    virtual void onEnter() override;

    virtual void update(float delta) override;

    virtual bool onContactBegin(cocos2d::PhysicsContact& contact) override;

protected:
    /**
     * Constructor for Projectile class
     * @param path : path to sprite image
     * @param pBody : this projectile's physics body
     * @param type : projectile type defined in the ProjectileType enum
     * @param velocity : this projectile's velocity after launch
     */
    Projectile(std::string path, cocos2d::PhysicsBody* pBody,
        ProjectileType type, float velocity = DEFAULT_PROJ_VELOCITY);

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

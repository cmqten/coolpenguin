#ifndef __CANNON_H__
#define __CANNON_H__

#include "cocos2d.h"
#include "IAnimated.h"
#include "Projectile.h"

/**
 * Cannon that shoots fish and ice cream
 */
class Cannon : public cocos2d::Node, public IAnimated {
public:
    Cannon();

    CREATE_FUNC(Cannon);

    /* Called right after this object is created */
    virtual bool init() override;

    /* Callback function for when a key is pressed */
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, 
        cocos2d::Event* event);

    /* Callback function for when the mouse is moved */
    void onMouseMove(cocos2d::EventMouse* event);

    bool isEnabled() { return _enabled; };

protected:
    /* Number of times fish has been shot out of the cannon */
    int _fishCount;

    /* Number of times ice cream has been shot out of the cannon */
    int _iceCreamCount;

    /* Determines whether cannon can shoot or not */
    bool _enabled;

    /* Rotates the cannon based on the cursor's position in the x and y axes. */
    void rotate(float x, float y);

    /** 
     * Shoots a projectile.
     * @param projType : an enumerator defined in the Projectile class, type of
     * projectile
     */
    void shoot(Projectile::ProjectileType projType);
};

#endif // !__CANNON_H__

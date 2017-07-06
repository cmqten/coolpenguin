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
    CREATE_FUNC(Cannon);

    static Cannon* getInstance();

    bool isEnabled() { return _enabled; };

    /**
    * Dispatches an event that updates the UI of the status of the cannon and
    * the amount of projectiles.
    */
    void updateUI();

    /* Called right after this object is created */
    virtual bool init() override;

    /* Callback function for when a key is pressed */
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, 
        cocos2d::Event* event);

    /* Callback function for when the mouse is moved */
    void onMouseMove(cocos2d::EventMouse* event);

protected:
    Cannon();

    static Cannon* _instance;

    /* Determines whether cannon can shoot or not */
    bool _enabled;

    /* Amount of fish in the reserve */
    int _fishReserve;

    /* Number of times fish has been shot out of the cannon */
    int _fishShot;

    /* Amount of ice cream in the reserve */
    int _iceCreamReserve;

    /* Number of times ice cream has been shot out of the cannon */
    int _iceCreamShot;

    /* Cleans the cannon */
    void clean();

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

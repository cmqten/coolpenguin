#ifndef __CANNON_H__
#define __CANNON_H__

#include "GameEntity.h"
#include "Projectile.h"

class Cannon : public GameEntity {
public:
    Cannon();

    CREATE_FUNC(Cannon);

    virtual bool init() override;

    virtual void updateEntity(float delta) {};

    /* Callback function for when a key is pressed */
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, 
        cocos2d::Event* event);

    /* Callback function for when the mouse is moved */
    void onMouseMove(cocos2d::EventMouse* event);

    bool isEnabled() { return _enabled; };

protected:
    /* Number of times fish has been shot out of the cannon */
    int fishCount;

    /* Number of times ice cream has been shot out of the cannon */
    int iceCreamCount;

    /* Rotates the cannon based on the cursor's position in the x and y axes.*/
    void rotate(float x, float y);

    /** 
    Shoots a projectile.
    @param projType : an enumerator defined in the Projectile class, type of
    projectile
    */
    void shoot(Projectile::ProjectileType projType);

    /* Enables cannon for shooting */
    void enableCannon() { _enabled = true; };

    /* Disables cannon for shooting */
    void disableCannon() { _enabled = false; };
    
private:
    /* Determines whether cannon can shoot or not */
    bool _enabled;
};

#endif // !__CANNON_H__

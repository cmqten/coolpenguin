#ifndef __COOL_PENGUIN_H__
#define __COOL_PENGUIN_H__

#include "cocos2d.h"
#include "Cannon.h"
#include "IPhysics.h"

/**
 * Main game
 */
class CoolPenguin : public cocos2d::Layer, public IPhysics {
public:
    static cocos2d::Scene* createScene();

    CREATE_FUNC(CoolPenguin);

    /* Called once when this object enters the running game */
    virtual void onEnter() override;

    /* Called when a key is pressed */
    virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, 
        cocos2d::Event* event) override;

    /* Called when the mouse is moved */
    void onMouseMove(cocos2d::EventMouse* event);

    /**
     * Called when two objects begin contact. Return true to process collision,
     * false otherwise.
     */
    virtual bool onContactBegin(cocos2d::PhysicsContact& contact) override;

private:
    Cannon* _cannon;
};

#endif

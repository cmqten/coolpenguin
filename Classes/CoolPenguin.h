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
    virtual bool onContactBegin(cocos2d::PhysicsContact& contact);

    /**
     * Called after two objects begin contact and before processing collision.
     * Returns false because this game doesn't require collisions.
     */
    virtual bool onContactPreSolve(cocos2d::PhysicsContact& contact,
        cocos2d::PhysicsContactPreSolve& solve) { return false; };

    /* Called after processing collision */
    virtual void onContactPostSolve(cocos2d::PhysicsContact& contact,
        const cocos2d::PhysicsContactPostSolve& solve) {};

    /* Called after two objects separate */
    virtual void onContactSeparate(cocos2d::PhysicsContact& contact) {};

private:
    Cannon* _cannon;
};

#endif

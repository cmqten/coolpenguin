#ifndef __PENGUIN_GAME_H__
#define __PENGUIN_GAME_H__

#include "cocos2d.h"
#include "Cannon.h"
#include "IPhysics.h"

/**
 * Main game
 */
class PenguinGame : public cocos2d::Layer, public IPhysics {
public:
    static cocos2d::Scene* createScene();

    CREATE_FUNC(PenguinGame);

    virtual void onEnter() override;

    virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, 
        cocos2d::Event* event) override;

    void onMouseMove(cocos2d::EventMouse* event);

    virtual bool onContactBegin(cocos2d::PhysicsContact& contact);

    virtual bool onContactPreSolve(cocos2d::PhysicsContact& contact,
        cocos2d::PhysicsContactPreSolve& solve) { return false; };

    virtual void onContactPostSolve(cocos2d::PhysicsContact& contact,
        const cocos2d::PhysicsContactPostSolve& solve) {};

    virtual void onContactSeparate(cocos2d::PhysicsContact& contact) {};

private:
    Cannon* _cannon;
};

#endif // !__PENGUIN_GAME_H__

#ifndef __COOL_PENGUIN_H__
#define __COOL_PENGUIN_H__

#include "cocos2d.h"
#include "IContact.h"
#include "IReset.h"

/**
 * Main game.
 */
class CoolPenguin : public cocos2d::Layer, public IContact, public IReset {
public:
    static cocos2d::Scene* createScene();

    CoolPenguin();

    CREATE_FUNC(CoolPenguin);

    virtual void reset();

    /* Called once when this object enters the running game */
    virtual bool init() override;

    virtual void onEnter() override;

    /* Called when a key is pressed */
    virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, 
        cocos2d::Event* event) override;

    /**
     * Called when two objects begin contact. Return true to process collision,
     * false otherwise.
     */
    virtual bool onContactBegin(cocos2d::PhysicsContact& contact) override;

protected:
    bool _paused;
};

#endif

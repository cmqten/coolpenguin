#ifndef __HELPER_PENGUIN_H__
#define __HELPER_PENGUIN_H__

#include "cocos2d.h"
#include "IAnimated.h"
#include "IReset.h"

#define HELPER_EVENT "helper_penguin"
#define WALK_SPEED 384.0f

/**
 * Helper penguin. This penguin cleans the cannon and fetches projectiles for 
 * you. Doesn't really do much than provide visuals.
 */
class HelperPenguin : public cocos2d::Node, public IAnimated, public IReset {
public:
    enum class State {CLEAN, GATHER, IDLE};

    static HelperPenguin* getInstance();

    CREATE_FUNC(HelperPenguin);

    inline HelperPenguin::State getState() { return _state; };

    /**
     * Tells helper penguin to go inside the cannon to clean it.
     * @param pos : cannon position
     */
    void cleanCannon(cocos2d::Vec2 pos);

    /**
     * Tells helper to gather a random amount of ice cream and fish. 
     * @param pos : cannon position
     */
    void gather(cocos2d::Vec2 pos);

    /**
     * Tells helper penguin to return to its original position after cleaning
     * the cannon.
     * @param pos : cannon position
     */
    void returnFromClean(cocos2d::Vec2 pos);

    /* Sets the helper penguin's start position */
    void setStartPosition(cocos2d::Vec2 startPos);

    virtual void reset();

    virtual void onEnter() override;

protected:
    HelperPenguin();

    static HelperPenguin* _instance;

    HelperPenguin::State _state;

};

#endif // !__HELPER_PENGUIN_H__

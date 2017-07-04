#ifndef __GAME_UI_H__
#define __GAME_UI_H__

#include "cocos2d.h"

#define UPDATE_CANNON "update_stats"
#define TIMER_TICK "timer_tick"

/**
 * Sidebar that displays current game stats such as time, score, etc...
 */
class GameUI : public cocos2d::Layer {
public:
    CREATE_FUNC(GameUI);

    /* Increments the time every second */
    void incrementTime(float delta);

    /**
     * Listener for cannon update events
     * @param event : event object, contains information about event
     */
    void updateCannonStats(cocos2d::EventCustom* event);

    virtual bool init() override;

private:
    /* Amount of time that has passed */
    long _time;
};

/**
 * To be used by the cannon, cannon fills out the fields, then dispatches an
 * UPDATE_CANNON event containing a pointer to this struct.
 */
struct CannonStats {
    int fishShotCount;
    int iceCreamShotCount;
    int fishReserve;
    int iceCreamReserve;
};

#endif
#ifndef __GAME_UI_H__
#define __GAME_UI_H__

#include "cocos2d.h"

#define UPDATE_STATS "update_stats"
#define TIMER_TICK "timer_tick"

/**
 * Sidebar that displays current game stats such as time, score, etc...
 */
class GameUI : public cocos2d::Layer {
public:
    CREATE_FUNC(GameUI);

    /* Increments the time every second */
    void incrementTime(float delta);

    virtual void onEnter() override;

private:
    /* Amount of time that has passed */
    long _time;
};

/**
 * To be used by the cannon, cannon fills out the fields, then dispatches an
 * UPDATE_STATS event containing a pointer to this struct.
 */
struct GameStats {
    int fishCount;
    int iceCreamCount;
};

#endif